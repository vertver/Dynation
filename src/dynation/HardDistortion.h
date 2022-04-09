#pragma once
#include "VST2_Header.h"

struct HARDDISTORTION_PARAMETER
{
	f32* fDriveLevel;
};

class CHardDist
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
				f32 boostLevel =( 3.14159265358979323846f + 4) * DriveLevel;
				if (boostLevel < 1) {
					boostLevel = 1;
				}

				OutCh[i] *= boostLevel;
				//OutCh[i] = OutCh[i] * (fabs(OutCh[i]) + DriveLevel) / ((OutCh[i] * OutCh[i]) + (DriveLevel - 1) * fabs(OutCh[i]) + 1);
				if (OutCh[i] > 0.9999999) {
					OutCh[i] = 0.9999999;
				}

				if (OutCh[i] < -0.9999999) {
					OutCh[i] = -0.9999999;
				}


				//OutCh[i] /= boostLevel;
				OutCh[i] = sin(OutCh[i]);		// +1dB limiting
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
				f32 boostValue = DriveLevel * 3.14159265358979323846 + 1 ;
				OutCh[i] *= boostValue;
				OutCh[i] = OutCh[i] * (fabs(OutCh[i]) + DriveLevel) / ((OutCh[i] * OutCh[i]) + (DriveLevel - 1) * (fabs(OutCh[i]) + 1));
				OutCh[i] /= boostValue;
				OutCh[i] = sin(OutCh[i]);		// +1dB limiting
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (HARDDISTORTION_PARAMETER*)pParam;
	}

	HARDDISTORTION_PARAMETER* CurrentParams;
	i32	SamplesAdd;
};
