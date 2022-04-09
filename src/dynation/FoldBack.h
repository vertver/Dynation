#pragma once
#include "VST2_Header.h"

struct FOLDBACK_PARAMETER
{
	f32* fDriveLevel;
};

namespace dummy
{
	template <class _Ty>
	inline bool isnormal(_Ty _X)
	{
		return fpclassify(_X) == FP_NORMAL;
	}
}

class CFoldBack
{
public:
	void Process(PROCESS_DATA* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32* OutCh = nullptr;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				f32 In = processData->Out[o][i];
				f32 drv = fabs(OutCh[i]);

				f32 DriveLevel2 = DriveLevel * 0.25f;

				//if (drv > 1.f - DriveLevel)
				{
					drv = (drv - DriveLevel2) * sign(OutCh[i]);
					OutCh[i] = fabs(fabs(fmod(OutCh[i] - drv, drv * 4)) - drv * 2) - drv;
				}

				OutCh[i] = sin(OutCh[i]);
				if (!dummy::isnormal(OutCh[i])) {
					OutCh[i] = In;
				}

				OutCh[i] = In * (1.0f - DriveLevel) + OutCh[i] * DriveLevel;
			}
		}
	}

	void ProcessDouble(PROCESS_DATA_DOUBLE* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f64* OutCh = nullptr;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				f32 drv = fabs(OutCh[i]);

				if (drv > 1.f - DriveLevel)
				{
					drv = (drv - DriveLevel) * sign(OutCh[i]);
					OutCh[i] = fabs(fabs(fmod(OutCh[i] - drv, drv * 4)) - drv * 2) - drv;
				}

				OutCh[i] = sin(OutCh[i]);
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (FOLDBACK_PARAMETER*)pParam;
	}

	FOLDBACK_PARAMETER* CurrentParams;
};
