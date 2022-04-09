#pragma once
#include "VST2_Header.h"

#define ROUND(f) ((f32)((f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5)))
#define ROUNDD(f) ((f64)((f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5)))

struct BITCRUSHER_PARAMETER
{
	f32* fSampleRate;
	f32* fGlobalRate;
	f32* fCurve;
	f32* fBits;
	f32* fDriveLevel;
};

class CBitcrusher
{
public:
	BITCRUSHER_PARAMETER* CurrentParams;
	i32	SamplesAdd[8];

	void Process(PROCESS_DATA* processData)
	{
		f32& BitLevel = *CurrentParams->fBits;
		f32& CurrentSampleRate = *CurrentParams->fGlobalRate;
		f32& SampleRateLevel = *CurrentParams->fSampleRate;
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& CurveLevel = *CurrentParams->fCurve;
		f32 TempX = 0.;
		f32* OutCh = nullptr;

		for (u32 o = 0; o < processData->ChannelsCount; o++) 
		{
			// bit shift
			OutCh = processData->Out[o];
			int max = int(BitLevel * BitLevel) - 1;
			int step = CurrentSampleRate / SampleRateLevel;
			if (!step) step = 1;
			f32 FirstSample = 0;
			f32 expX = 0;
			f32 temp = 0;
			for (u32 i = 0; i < processData->SamplesCount; i++)
			{
				f32 boostValue = (DriveLevel * 3.14159265358979323846f + 1);
				TempX = OutCh[i];

				TempX *= boostValue;

				f32 expX = expf(TempX);
				f32 temp = 1 + expf(sqrtf(fabsf(TempX)) * CurveLevel);

				OutCh[i] = (expX - expf(-TempX * temp)) / (expX + expf(-TempX));

				float DestructorValue = CurveLevel * (powf(2, logf(temp)));
				OutCh[i] /= boostValue;
				//OutCh[i] = sign(OutCh[i]) * (fabsf(OutCh[i]) - DestructorValue);
				OutCh[i] = sinf(sinf(OutCh[i]));		// +1dB limiting
			}
		}
	}

	void ProcessDouble(PROCESS_DATA_DOUBLE* processData)
	{
		f32& BitLevel = *CurrentParams->fBits;
		f32& CurrentSampleRate = *CurrentParams->fGlobalRate;
		f32& SampleRateLevel = *CurrentParams->fSampleRate;
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& CurveLevel = *CurrentParams->fCurve;
		f32 TempX = 0.;
		f64* OutCh = nullptr;

		for (u32 o = 0; o < processData->ChannelsCount; o++)
		{
			// bit shift
			OutCh = processData->Out[o];
			int max = pow(2, BitLevel) - 1;
			int step = CurrentSampleRate / SampleRateLevel;
			if (!step) step = 1;
			f64 FirstSample = 0;
			f64 expX = 0;
			f64 temp = 0;

			FirstSample = ROUNDD((OutCh[0] + 1.0) * max) / max - 1.0;

			for (u32 i = 0; i < processData->SamplesCount; i++)
			{
				TempX = OutCh[i] * (DriveLevel * 3.14159265358979323846 + 1);

				expX = exp(TempX);
				temp = 1 + exp(sqrt(fabs(TempX)) * CurveLevel);
				OutCh[i] = sin((expX - exp(-TempX * temp)) / (expX + exp(-TempX)));

				if (SamplesAdd[o] == 0)
				{
					FirstSample = ROUNDD((OutCh[i] + 1.0) * max) / max - 1.0;
				}

				// Каждый раз перед процессом не забыть занулять это хуету, а то пиздой биткрашер пойдет
				SamplesAdd[o] = (SamplesAdd[o] + 1) % step;
				OutCh[i] = FirstSample;
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (BITCRUSHER_PARAMETER*)pParam;
	}
};
