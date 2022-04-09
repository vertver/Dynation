/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "Header.h"

void			PluginDistortion::SetPluginDelay(i32 DelayTime) {
    Plugin_Base->setInitialDelay(DelayTime);
    CurrentDelay = DelayTime;
}

i32			PluginDistortion::GetPluginDelay()
{
    
    return 0;
}

void            PluginDistortion::Process(PROCESS_DATA* data) {
	// ByPass
	if (ByPass || data->InChannelsCount == 0 || data->OutChannelsCount == 0) {
		size_t NowOutChannels = GetOutputCount();
		size_t NowInChannels  = GetInputCount();

		for (size_t i = 0; i < min(NowOutChannels, NowInChannels); i++) {
			memcpy(data->Out[i], data->In[i], data->SamplesCount * sizeof(f32));
		}
		for (size_t i = min(NowOutChannels, NowInChannels); i < max(NowOutChannels, NowInChannels); i++) {
			memset(data->Out[i], 0, data->SamplesCount * sizeof(f32));
		}
		return;
	}
	
	// Smool setting koef
	f32 SmoolScale = 0.05 * (CurrentSampleRate / 44100.0); // 50 ms smool

	// over settings
	u32 OversamplingCount       = OversampleCount;
    f32 OversamplingSampleRate  = CurrentSampleRate * OversamplingCount;

	// Resuze buffers
	BufferA.Resize(data->InChannelsCount, 0, data->SamplesCount);
	BufferC.Resize(data->InChannelsCount, 0, data->SamplesCount * OversamplingCount);

    // EnvelopIn (data.In)
	for (u32 chNum = 0; chNum < data->InChannelsCount; chNum++) {
		auto chEnv  = &InputEnvelops[chNum];
		auto chdata = data->In[chNum];
		for (u32 i = 0; i < data->SamplesCount; i++) chEnv->NextSample(chdata[i]);
	}
	
    // BufferA   <- data.In * InVolume
	for (u32 i = 0; i < data->SamplesCount; i++) {
		InVolumeResult += (((InVolume) - InVolumeResult)) * SmoolScale;

		for (u32 chNum = 0; chNum < data->InChannelsCount; chNum++) {
			f32 Value = data->In[chNum][i];
			if (!std::isnormal(Value)) Value = 0.0f;
			BufferA.In[chNum][i] = Value * InVolumeResult;
		}
	}

    // PreComp  BufferA
	if (!Compressor_Post) {
		for (u32 i = 0; i < BufferA.SamplesCount; i++) {
			//
			f32 ValueIn = 0.0f;
			for (u32 o = 0; o < BufferA.InChannelsCount; o++) ValueIn += BufferA.In[o][i];
			ValueIn /= (f32)BufferA.InChannelsCount;
			
			//
			f32 ScaleOutput = 0.0f;
			f32 RmsIn = CompressorPunch.ProcessSample(&ValueIn, &ScaleOutput);

			//
			for (u32 o = 0; o < BufferA.InChannelsCount; o++) BufferA.In[o][i] *= ScaleOutput;
			
			//
			Metter_Update(RmsIn, RmsIn * ScaleOutput, ScaleOutput);
		}
	}

    // Tilt     BufferA
	for (u32 chNum = 0; chNum < BufferA.InChannelsCount; chNum++) {
		auto TiltEQCur	= &TiltEQ[chNum];
		auto chdata		= BufferA.In[chNum];
		for (u32 i = 0; i < BufferA.SamplesCount; i++) {
			chdata[i] = TiltEQCur->NextSample(chdata[i]);
		}
	}

	// oversampling (BufferA -> BufferC) { 
	PROCESS_DATA* BufferOversampling = &BufferA;
	if (OversamplingCount > 1) {
		BufferOversampling = &BufferC;
		//.. todo add
	}

    //    Sat      (BufferC <- BufferC)
	switch (TypeDistortion) {
	case 0: { // Tube Distortion
		f32 TubeCurvenessLevel	= CurvenessLevel;
		f32 TubeDriveLevel		= DriveLevel;
		
		f32 SaveLoada, SaveLoadk;
		SatAlg_TubeTriodeInit(0.5f, SaveLoada, SaveLoadk);

		for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
			TubeDriveLevel = DriveLevelResult; // rebegin

			auto chdata	= BufferOversampling->In[chNum];
			for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
				TubeDriveLevel += ((DriveLevel - TubeDriveLevel)) * SmoolScale;
				
				f32 TubeDriveLevel1 = TubeDriveLevel + 1.0f;
				
				f32 Temp	= SatAlg_TubeTriode(chdata[i] * 0.25f);
				f32 Temp2	= SatAlg_TubeTriode(SatAlg_FailureTubeTriode(Temp, SaveLoada, SaveLoadk)* 0.25f);
				Temp		+= (Temp2 - Temp) * TubeCurvenessLevel;
				
				chdata[i]	+= (Temp - chdata[i]) * TubeDriveLevel;
			}
		}

		DriveLevelResult = TubeDriveLevel;
	}
	break; 
	case 1: { // Sincrusher
		f32 SatDriveLevel		= DriveLevel;
		f32 SatCurvenessLevel	= CurvenessLevel;
		
		for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
			SatDriveLevel = DriveLevelResult;
			
			auto chdata = BufferOversampling->In[chNum];
			for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
				SatDriveLevel += ((DriveLevel - SatDriveLevel)) * SmoolScale;
				
				f32 SatDriveLevel1Fix = SatDriveLevel * 2.0f + 1.0f;
				
				f32 Temp = SatAlg_Sincrusher(chdata[i], SatDriveLevel1Fix * 3.14159265358979323846f * 0.5f);

				f32 Temp2 = SatAlg_Sincrusher( Temp * 3.14159265358979323846f, SatDriveLevel1Fix * 3.14159265358979323846f * 0.5f);

				Temp += (Temp2 - Temp) * SatCurvenessLevel;

				Temp *= 2.0f;

				chdata[i] += (Temp - chdata[i]) * SatDriveLevel;
			}
		}

		DriveLevelResult = SatDriveLevel;
	}
	break;
	case 2: { // Hard Distortion
		f32 SatDriveLevel		= DriveLevel;
		f32 SatCurvenessLevel	= CurvenessLevel;

		for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
			SatDriveLevel = DriveLevelResult;

			auto chdata = BufferOversampling->In[chNum];
			for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
				SatDriveLevel += ((DriveLevel - SatDriveLevel)) * SmoolScale;

				f32 SatDriveLevel1Fix = SatDriveLevel * 2.0 + 1.0f;

				f32 Temp = SatAlg_HardClipDrive(chdata[i], SatDriveLevel1Fix);

				f32 Temp2 = SatAlg_Sincrusher(Temp * 3.14159265358979323846f, SatDriveLevel1Fix * 3.14159265358979323846f * 0.5f);
				
				Temp += (Temp2 - Temp) * SatCurvenessLevel * 0.8f;

				Temp *= 2.0f;

				chdata[i] += (Temp - chdata[i]) * SatDriveLevel;
			}
		}

		DriveLevelResult = SatDriveLevel;
	}
	break;
	case 3: // F-Back Distortion
	{
		f32 SatDriveLevel = DriveLevel;
		f32 SatCurvenessLevel = CurvenessLevel;

		for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
			SatDriveLevel = DriveLevelResult;

			auto chdata = BufferOversampling->In[chNum];
			for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
				SatDriveLevel += ((DriveLevel - SatDriveLevel)) * SmoolScale;

				f32 SatDriveLevel1Fix = SatDriveLevel * 1.0 + 1.0f;

				f32 Temp  = SatAlg_FoldbackDrive(chdata[i], SatDriveLevel1Fix);

				f32 Temp2 = SatAlg_SoftClip2Drive(Temp, SatDriveLevel1Fix, 0.85f);

				Temp2 += (Temp - Temp2) * SatCurvenessLevel * 0.8f;

				chdata[i] = Temp2 * SatDriveLevel1Fix;
			}
		}

		DriveLevelResult = SatDriveLevel;
	}
	break;
	case 4: // "Soft Clipper"
	{
		f32 SatDriveLevel = DriveLevel;
		f32 SatCurvenessLevel = CurvenessLevel;

		for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
			SatDriveLevel = DriveLevelResult;

			auto chdata = BufferOversampling->In[chNum];
			for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
				SatDriveLevel += ((DriveLevel - SatDriveLevel)) * SmoolScale;

				f32 SatDriveLevel1Fix = SatDriveLevel * 2.0 + 1.0f;

				f32 Temp	= SatAlg_SoftClipDrive(chdata[i], SatDriveLevel1Fix);
				f32 Temp2	= SatAlg_Sincrusher(Temp * 3.14159265358979323846f, SatDriveLevel1Fix);

				Temp += (Temp2 - Temp) * SatCurvenessLevel * 0.8f;

				Temp *= 2.0f;

				chdata[i] = Temp;
			}
		}

		DriveLevelResult = SatDriveLevel;
	}
	break;
	case 5: // "Soft Clip-p-per (alt)"  ??????????????????????????????????
	{
	/*CSoftClipper sinCrusher = {};
	SOFTCLIPPING_PARAMETER sinCrush = {};
	sinCrush.fDriveLevel = &DriveLevel;
	sinCrush.fClippingRatio = &CurvenessLevel;
	sinCrusher.SetEffectParameter(&sinCrush);
	sinCrusher.Process(data);*/
	}
	break;
	default:
		break;
	}

	///*for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
	//	auto chdata = BufferOversampling->In[chNum];
	//	auto ADCFailureValue = ADCFailure_Value[chNum];
	//	for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
	//		chdata[i] = SatAlg_ADCFailure(chdata[i], ADCFailureValue, DriveLevel);
	//	}
	//	ADCFailure_Value[chNum] = ADCFailureValue;
	//}*/

    //    BitCr    (BufferC <- BufferC)
	if (BitLevel < 32) {
		u32 BitCrushing_ValuePow;
		SatAlg_BitCrushingInit(BitLevel, BitCrushing_ValuePow);
		for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
			auto chdata = BufferOversampling->In[chNum];
			for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
				chdata[i] = SatAlg_BitCrushing(chdata[i], BitCrushing_ValuePow);
			}
		}
	}

    //    RateRe   (BufferC <- BufferC)
	if (SampleRateLevel < CurrentSampleRate) {
		u32 SampleRateDivNeadRate = round(OversamplingSampleRate / (SampleRateLevel * OversamplingCount));
		for (u32 chNum = 0; chNum < BufferOversampling->InChannelsCount; chNum++) {
			auto chdata = BufferOversampling->In[chNum];
			auto RateReducerValue = RateReducer_Value[chNum];
			auto RateReducerValue2 = RateReducer_Value2[chNum];
			for (u32 i = 0; i < BufferOversampling->SamplesCount; i++) {
				chdata[i] = SatAlg_RateReducer(chdata[i], RateReducerValue, RateReducerValue2, SampleRateDivNeadRate);
			}
			RateReducer_Value[chNum] = RateReducerValue;
			RateReducer_Value2[chNum] = RateReducerValue2;
		}
	}
	
	// oversampling (BufferC -> BufferA) }
	if (OversamplingCount > 1) {
		//.. todo add
	}

    // PostComp BufferA
	if (Compressor_Post) {
		for (u32 i = 0; i < BufferA.SamplesCount; i++) {
			//
			f32 ValueIn = 0.0f;
			for (u32 o = 0; o < BufferA.InChannelsCount; o++) ValueIn += BufferA.In[o][i];
			ValueIn /= (f32)BufferA.InChannelsCount;
			
			//
			f32 ScaleOutput = 0.0f;
			f32 RmsIn = CompressorPunch.ProcessSample(&ValueIn, &ScaleOutput);

			//
			for (u32 o = 0; o < BufferA.InChannelsCount; o++) BufferA.In[o][i] *= ScaleOutput;
			
			//
			Metter_Update(RmsIn, RmsIn * ScaleOutput, ScaleOutput);
		}
	}
    
    // BufferA  <- FixClip(DryWet(BufferA * OutVolume, data.In))
	for (u32 i = 0; i < data->SamplesCount; i++) {
		OutVolumeResult   += SmoolScale * (OutVolume - OutVolumeResult);
		DryWetLevelResult += SmoolScale * (DryWetLevel - DryWetLevelResult);

		for (u32 chNum = 0; chNum < data->InChannelsCount; chNum++) {
			f32 ValueOriginal	= data->In[chNum][i];
			f32 Value			= BufferA.In[chNum][i];
			
			Value *= OutVolumeResult;
			Value = DryWetLevelResult * Value + (1.0f - DryWetLevelResult) * ValueOriginal;
			Value = SatAlg_HardClip(Value, 1.0f);
			if (!std::isnormal(Value)) Value = 0.0f;

			BufferA.In[chNum][i] = Value;
		}
	}

	// data.Out <- BufferA
	{ 
		u32 RealOutChannelsCount = min(data->InChannelsCount, data->OutChannelsCount);

		for (u32 chNum = 0; chNum < RealOutChannelsCount; chNum++) {
			memcpy(data->Out[chNum], BufferA.In[chNum], data->SamplesCount * sizeof(f32));
		}
		
		for (size_t i = RealOutChannelsCount; i < data->OutChannelsCount; i++) {
			memset(data->Out[i], 0, data->SamplesCount * sizeof(f32));
		}
	}
    
    // EnvelopOut (data.Out)
	for (u32 chNum = 0; chNum < data->OutChannelsCount; chNum++) {
		auto chEnv = &OutputEnvelops[chNum];
		auto chdata = data->Out[chNum];
		for (u32 i = 0; i < data->SamplesCount; i++) chEnv->NextSample(chdata[i]);
	}
}

void            PluginDistortion::ProcessDouble(PROCESS_DATA_DOUBLE* data) {
    // Not Used!!!!!!
}
