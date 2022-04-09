#pragma once
#include "VST2_Header.h"
#define sign(x)  (signbit(x) ?  -1 : 1)

#define ROUND(f) ((f32)((f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5)))
#define ROUNDD(f) ((f64)((f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5)))

struct SINCRUSHER_PARAMETER
{
	f32* fClippingRatio;
	f32* fSampleRate;
	f32* fGlobalRate;
	f32* fBits;
	f32* fDriveLevel;
};

class CSincrusher 
{
public:
	void Process(PROCESS_DATA* processData) 
	{
		f32& BitLevel = *CurrentParams->fBits;
		f32& ClippingRatio = *CurrentParams->fClippingRatio;
		f32& CurrentSampleRate = *CurrentParams->fGlobalRate;
		f32& SampleRateLevel = *CurrentParams->fSampleRate;
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32* OutCh = nullptr;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				f32 In = processData->Out[o][i];
				f32 boostVolume = 1.0f + (DriveLevel * 3.14159265358979323846f * 2);
				OutCh[i] *= boostVolume;
				OutCh[i] = sin(OutCh[i]);
				OutCh[i] /= boostVolume;
				OutCh[i] = In * (1.0f - DriveLevel) + OutCh[i] * DriveLevel;
			}
		
		}
	}

	void ProcessDouble(PROCESS_DATA_DOUBLE* processData)
	{
		f32& BitLevel = *CurrentParams->fBits;
		f32& CurrentSampleRate = *CurrentParams->fGlobalRate;
		f32& SampleRateLevel = *CurrentParams->fSampleRate;
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f64* OutCh = nullptr;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			// bit shift
			int max = pow(2, BitLevel) - 1;
			int step = CurrentSampleRate / SampleRateLevel;
			if (!step) step = 1;
			f64 FirstSample = 0;

			FirstSample = ROUNDD((OutCh[0] + 1.0) * max) /  max - 1.0;

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				// sin distortion
				OutCh[i] = (sin(OutCh[i] * ((DriveLevel * 3.14159265358979323846) + 1)));	// если привысить число пи - сигнал будет уходить в противофазу

				if (SamplesAdd[o] == 0)
				{
					FirstSample = ROUNDD((OutCh[i] + 1.0) * max) / max - 1.0;
				}

				SamplesAdd[o] = (SamplesAdd[o] + 1) % step;
				OutCh[i] = FirstSample;
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (SINCRUSHER_PARAMETER*)pParam;
	}

	SINCRUSHER_PARAMETER* CurrentParams;
	i32	SamplesAdd[8];
};
