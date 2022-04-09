#pragma once
#include "VST2_Header.h"
#define sign(x)  (signbit(x) ?  -1 : 1)

struct SOFTCLIPPING_PARAMETER
{
	f32* fClippingRatio;
	f32* fDriveLevel;
};

class CSoftClipper
{
public:
	void ProcessNew(PROCESS_DATA* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& ClippingRatio = *CurrentParams->fClippingRatio;
		f32* OutCh = nullptr;
		f32 TempX = 0.;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				f32 In = processData->Out[o][i];
				f32 boostVolume = (FLT_EPSILON + OutCh[i] + (DriveLevel * 3.14159265358979323846f * 2));
				OutCh[i] *= boostVolume;
				f32 range = max(ClippingRatio * 100.f, 1.f);
 				f32 temp = pow(fabs(OutCh[i]), range);
				temp = fastatan(temp);
				OutCh[i] = sign(OutCh[i]) * pow(temp, (1 / range));
				//OutCh[i] /= boostVolume;
				OutCh[i] = In * (1.0f - DriveLevel) + OutCh[i] * DriveLevel;
			}
		}
	}

	void Process(PROCESS_DATA* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& ClippingRatio = *CurrentParams->fClippingRatio;
		f32* OutCh = nullptr;
		f32 TempX = 0.;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				f32 In = processData->Out[o][i];
				f32 boostVolume = (FLT_EPSILON + OutCh[i] + (DriveLevel * 3.14159265358979323846f * 2));
				OutCh[i] *= boostVolume;
				f32 range = max(ClippingRatio * 100.f, 1.f);
				f32 temp = pow(fabs(OutCh[i]), range);
				temp = atan(temp);
				OutCh[i] = sign(OutCh[i]) * pow(temp, (1 / range));
				//OutCh[i] /= boostVolume;
				OutCh[i] = In * (1.0f - DriveLevel) + OutCh[i] * DriveLevel;
			}
		}
	}

	void ProcessDouble(PROCESS_DATA_DOUBLE* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& ClippingRatio = *CurrentParams->fClippingRatio;
		f64* OutCh = nullptr;
		f64 TempX = 0.;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				OutCh[i] += OutCh[i] * DriveLevel * 3.14159265358979323846;
				OutCh[i] = sign(OutCh[i]) * pow(fastatan(pow(fabs(OutCh[i]), ClippingRatio)), (1 / ClippingRatio));
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (SOFTCLIPPING_PARAMETER*)pParam;
	}

	SOFTCLIPPING_PARAMETER* CurrentParams;
	i32	SamplesAdd;
};
