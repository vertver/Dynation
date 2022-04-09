#pragma once
#include "VST2_Header.h"

struct SOFTDISTORTION_PARAMETER
{
	f32* fCurve;
	f32* fDriveLevel;
};

class CSoftDist
{
public:
	void Process(PROCESS_DATA* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& CurveLevel = *CurrentParams->fCurve;
		f32* OutCh = nullptr;

		f32 a = sin(((DriveLevel * 70.0f) / 101.0f) * (3.14159265358979323846f / 2.0f));
		f32 k = 2.0f * a / (1.0f - a);

		for (size_t o = 0; o < processData->ChannelsCount; o++) {
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++) {

				auto stfun = [](f32 In, f32 k, f32 CurveLevel, f32 DriveLevel) -> f32 {
					// dist 1
					f32 Temp2 = (1.0f + k) * (In) / (1.0f + k * fabsf(In)) ;
					
					// dist 2
					auto sttubefun = [](f64 x) -> f64 {
						x = sign(x) * ((2.0 - fabsf(x)) * fabsf(x));

						if (x < -1.0f) {
							return -1.0f;
						} else if (x < -0.08905) { // -0.399 -0.379
							//In -= 0.003f;
							f32 Temp6		= (fabs(x) - 0.032847);
							f32 Temp6_pow12 = pow((1.0 - Temp6), 12.0);
							f32 Temp7		= 1.0 - Temp6_pow12 + Temp6 * (1.0 / 3.0);
							return -(0.01 + (3.0 / 4.0) * Temp7);
						} else if (x < 0.320018) {
							return -6.152* x * x + x * 3.9375;
						} else {
							return 0.630035;
						}
					};

					f32 Temp4 = (sttubefun(In) * (1.0f - CurveLevel) + Temp2 * CurveLevel)*0.9f;

					return In * (1.0f - DriveLevel) + Temp4 * DriveLevel;
				};

				f32 Temp1 = OutCh[i];
				OutCh[i] = stfun(Temp1, k, CurveLevel, DriveLevel) ;

				if (OutCh[i] > 0.9999999)  OutCh[i] = 0.9999999;
				if (OutCh[i] < -0.9999999) OutCh[i] = -0.9999999;
			}
		}
	}

	void ProcessDouble(PROCESS_DATA_DOUBLE* processData)
	{
		f32& DriveLevel = *CurrentParams->fDriveLevel;
		f32& CurveLevel = *CurrentParams->fCurve;
		f64* OutCh = nullptr;
		f64 TempX = 0.;

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			for (u32 i = 0; i < processData->SamplesCount; i++)
			{
				TempX = OutCh[i] * (DriveLevel * 3.14159265358979323846 + 1);

				f64 expX = exp(TempX);
				f64 temp = 1 + exp(sqrt(fabs(TempX)) * CurveLevel);

				OutCh[i] = (expX - exp(-TempX * temp)) / (expX + exp(-TempX));

				OutCh[i] /= (3.14159265358979323846 / 1.5f);
				OutCh[i] = sin(sin(OutCh[i]) * 1.4f);		// +1dB limiting
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (SOFTDISTORTION_PARAMETER*)pParam;
	}

	SOFTDISTORTION_PARAMETER* CurrentParams;
	i32	SamplesAdd;
};
