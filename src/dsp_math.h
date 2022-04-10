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