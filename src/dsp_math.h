/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once
#include <cmath>

// linear -> dB conversion
inline double lin2dB(double lin)
{
	return std::log(lin) * 8.6858896380650365530225783783321;		// 20 / ln( 10 )
}

// dB -> linear conversion
inline double dB2lin(double dB)
{
	return std::exp(dB * 0.11512925464970228420089957273422);		// ln( 10 ) / 20
}

inline
double
fasttanh2(double x) {
	double ax = fabs(x);
	double x2 = x * x;

	return(x * (2.45550750702956 + 2.45550750702956 * ax +
		(0.893229853513558 + 0.821226666969744 * ax) * x2) /
		(2.44506634652299 + (2.44506634652299 + x2) *
			fabs(x + 0.814642734961073 * x * ax)));
}

inline double fastatan(double x)
{
	return (x / (1.0 + 0.28 * (x * x)));
}

inline float fastatanf(float x)
{
	return (x / (1.0f + 0.28f * (x * x)));
}

inline float lerp(float min, float max, float t)
{
	return (1 - t) * max + t * min;
}

inline float rlerp(float max, float min, float t)
{
	return (t - min) / (max - min);
}

inline double sincurve(double x)
{
	return 1 - std::cos((x * 3.1415926535897932384) / 2);
}

inline double rsincurve(double x)
{
	return std::acos(1 - x) * 2 / 3.1415926535897932384;
}

inline double rlog10(double x)
{
	return std::pow(10, x);
}

inline float signf(float x)
{
	return (std::signbit(x) ? -1.f : 1.f);
}

inline float param_to_log(float x, float mn, float mx)
{
	return (exp(log(mn) + (x) * (log(mx) - log(mn))));
}

inline float log_to_param(float x, float mn, float mx)
{
	return  ((log(x) - log(mn)) / (log(mx) - log(mn)));
}

inline float param_to_lin(float x, float mn, float mx)
{
	return ((mn) + (x) * ((mx) - (mn)));
}

inline float lin_to_param(float x, float mn, float mx)
{
	return (((x) - (mn)) / ((mx) - (mn)));
}
