#pragma once
#include "VST2_Header.h"

struct TILTEQ_PARAMETER
{
	f32* fEqLevel;
	f32* fCurrentSampleRate;
};

class CTiltEq
{
public:
	void Process(PROCESS_DATA* processData)
	{
		// Tilt eq 1000gz +- 18db
		f32&	EQLevel = *CurrentParams->fEqLevel;
		f32&	CurrentSampleRate = *CurrentParams->fCurrentSampleRate;
		f32		Amp = 6 / log(2);
		f32		sr3 = 3 * CurrentSampleRate;
		f32		g1 = 0;
		f32		g2 = 0;
		f32*	OutCh = nullptr;
		if (EQLevel > 0)
		{
			g1 = -5/*gfactor*/ * EQLevel * 6.0f;
			g2 = EQLevel * 6.0f;
		}
		else
		{
			g1 = -EQLevel * 6.0f;
			g2 = 5/*gfactor*/ * EQLevel * 6.0f;
		}

		f32		lgain = exp(g1 / Amp) - 1;
		f32		hgain = exp(g2 / Amp) - 1;
		f32*	llp_out = nullptr;		// делаем переменную для указателя, чтобы динамически установить позицию

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			switch (o)
			{
			case 0:
				llp_out = &_1_out;
				break;
			case 1:
				llp_out = &_2_out;
				break;
			case 2:
				llp_out = &_3_out;
				break;
			case 3:
				llp_out = &_4_out;
				break;
			case 4:
				llp_out = &_5_out;
				break;
			case 5:
				llp_out = &_6_out;
				break;
			case 6:
				llp_out = &_7_out;
				break;
			case 7:
				llp_out = &_8_out;
				break;
			default:
				break;
			}

			f32& p_out = *llp_out;

			f32 Omega = 2.0f * 3.14159265358979323846f * (f32)1000;
			f32 n = 1 / (sr3 + Omega);
			f32 a0 = 2 * Omega * n;
			f32 b1 = (sr3 - Omega) * n;

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				p_out = a0 * OutCh[i] + b1 * p_out;
				OutCh[i] = OutCh[i] + lgain * p_out + hgain * (OutCh[i] - p_out);
				if (OutCh[i] >= 6.0f) OutCh[i] = 6.0f;
				if (OutCh[i] <= -6.0f) OutCh[i] = -6.0f;
			}
		}
	}

	void ProcessDouble(PROCESS_DATA_DOUBLE* processData)
	{
		// Tilt eq 1000gz +- 18db
		f32&	EQLevel = *CurrentParams->fEqLevel;

		f32&	CurrentSampleRate = *CurrentParams->fCurrentSampleRate;
		f32		Amp = 6 / log(2);
		f32		sr3 = 3 * CurrentSampleRate;
		f32		g1 = 0;
		f32		g2 = 0;
		f64*	OutCh = nullptr;
		if (EQLevel > 0)
		{
			g1 = -5/*gfactor*/ * EQLevel * 6.0;
			g2 = EQLevel * 6.0;
		}
		else
		{
			g1 = -EQLevel * 6.0;
			g2 = 5/*gfactor*/ * EQLevel * 6.0;
		}

		f32		lgain = exp(g1 / Amp) - 1;
		f32		hgain = exp(g2 / Amp) - 1;
		f32*	llp_out = nullptr;		// делаем переменную для указателя, чтобы динамически установить позицию

		for (size_t o = 0; o < processData->ChannelsCount; o++)
		{
			OutCh = processData->Out[o];

			switch (o)
			{
			case 0:
				llp_out = &_1_out;
				break;
			case 1:
				llp_out = &_2_out;
				break;
			case 2:
				llp_out = &_3_out;
				break;
			case 3:
				llp_out = &_4_out;
				break;
			case 4:
				llp_out = &_5_out;
				break;
			case 5:
				llp_out = &_6_out;
				break;
			case 6:
				llp_out = &_7_out;
				break;
			case 7:
				llp_out = &_8_out;
				break;
			default:
				break;
			}

			f32& p_out = *llp_out;

			f32 Omega = 2.0 * 3.14159265358979323846 * 1000.0;
			f32 n = 1 / (sr3 + Omega);
			f32 a0 = 2 * Omega * n;
			f32 b1 = (sr3 - Omega) * n;

			for (u32 i = 0; i < processData->SamplesCount; i++) {
				p_out = a0 * OutCh[i] + b1 * p_out;
				OutCh[i] = OutCh[i] + lgain * p_out + hgain * (OutCh[i] - p_out);
				if (OutCh[i] >= 6.0f) OutCh[i] = 6.0f;
				if (OutCh[i] <= -6.0f) OutCh[i] = -6.0f;
			}
		}
	}

	void SetEffectParameter(void* pParam)
	{
		CurrentParams = (TILTEQ_PARAMETER*)pParam;
	}

	void Reset()
	{
		SamplesAdd = 0;
		_1_out = 0;
		_2_out = 0;
		_3_out = 0;
		_4_out = 0;
		_5_out = 0;
		_6_out = 0;
		_7_out = 0;
		_8_out = 0;
	}

	TILTEQ_PARAMETER* CurrentParams;
	i32	SamplesAdd;
	f32		        _1_out = 0;
	f32		        _2_out = 0;
	f32		        _3_out = 0;
	f32		        _4_out = 0;
	f32		        _5_out = 0;
	f32		        _6_out = 0;
	f32		        _7_out = 0;
	f32		        _8_out = 0;
};
