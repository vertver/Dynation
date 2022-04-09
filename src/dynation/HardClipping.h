#pragma once
#include "VST2_Header.h"
#define sign(x)  (signbit(x) ?  -1 : 1)

struct HARDCLIPPING_PARAMETER
{
	f32* fClippingRatio;
	f32* fDriveLevel;
};

class CHardClipper
{
public:
	void Process(PROCESS_DATA* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& ClippingRatio = *CurrentParams->fClippingRatio;
		f32* OutCh = nullptr;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			f32 LocalClipping = ClippingRatio / 10;
			for (u32 i = 0; i < processData->SamplesCount; i++) {
				OutCh[i] += OutCh[i] * DriveLevel * 3.14159265358979323846f;
				OutCh[i] = ((1 / LocalClipping) * fastatan(OutCh[i] * LocalClipping)) * LocalClipping;
			}
		}
	}

	void ProcessDouble(PROCESS_DATA_DOUBLE* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& ClippingRatio = *CurrentParams->fClippingRatio;
		f64* OutCh = nullptr;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			f64 LocalClipping = ClippingRatio / 10;
			for (u32 i = 0; i < processData->SamplesCount; i++) {
				float boostValue = (DriveLevel * 3.14159265358979323846 * 2.f) + 1.f;
				OutCh[i] *= boostValue;
				OutCh[i] = ((1 / LocalClipping) * fastatan(OutCh[i] * LocalClipping)) * LocalClipping;
				OutCh[i] /= boostValue;
				OutCh[i] /= LocalClipping * LocalClipping;
				OutCh[i] /= (1 / LocalClipping);
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (HARDCLIPPING_PARAMETER*)pParam;
	}

	HARDCLIPPING_PARAMETER* CurrentParams;
	i32	SamplesAdd;
};
