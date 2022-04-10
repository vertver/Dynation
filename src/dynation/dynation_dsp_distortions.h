/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

/***************************************************************
* 
* Soft Clipers
* 
****************************************************************/
auto SoftClipFirst = [](auto Input, auto Drive) 
{
	auto Value = Input * Drive;
	Value = static_cast<auto>(fasttanh2(static_cast<double>(Value)));
	Value *= (1.0 / Drive);
	return Value;
};

auto SoftClipSecond = [](auto Input, auto Drive, auto A) 
{
	auto Value = Input * Drive;
	auto ValueAbs = fabs(Value);

	if (ValueAbs > 1.0) {
		ValueAbs = (A + 1.0) * 0.5;
	}
	else if (ValueAbs > A) {
		ValueAbs = A + (ValueAbs - A) /
			(1.0 + pow((ValueAbs - A) / (1.0 - A), 2.0));
	}

	Value = ValueAbs * signf(Value);
	Value *= (1.0 / Drive);
	return Value;
};

/***************************************************************
*
* Hard Clipers
*
****************************************************************/
auto HardClip = [](auto Input, auto Threshold) -> auto 
{
	if (Input > Threshold) return Threshold;
	if (Input < -Threshold) return -Threshold;
	return Input;
};

auto HardClipCurve = [](auto Input, auto Drive, auto Curve) -> auto
{
	auto Value = In * Drive;
	auto One = static_cast<auto>(1.0);
	auto Zero = static_cast<auto>(0.0);
	auto Pi = static_cast<auto>(3.14159265358979323846);

	if (Value > One) {
		Value = One - sin((Value - One) * Curve * Pi * static_cast<auto>(0.1));
		if (Value < Zero) Value = Zero;
	}
	if (Value < -One) {
		Value = -One - sin((Value - -One) * Curve * Pi * static_cast<auto>(0.1));
		if (Value > Zero) Value = Zero;
	}

	Value *= (One / Drive);
	return Value;
};


auto HardDistortion = [](auto Input, auto Drive) -> auto
{
	auto Value = In * Drive;
	if (Value > static_cast<auto>(1.0))  Value = static_cast<auto>(1.0);
	if (Value < static_cast<auto>(-1.0)) Value = static_cast<auto>(-1.0);
	Value *= (static_cast<auto>(1.0) / Drive);
	return Value;
};

/***************************************************************
*
* Faldback distortion
*
****************************************************************/
auto FoldbackDistortion = [](auto Input, auto Drive) -> auto
{
	auto Value = Input * Drive;
	const auto Threshold = static_cast<auto>(1.0);
	if (Value > Threshold || Value < -Threshold) {
		Value = static_cast<auto>(fabs(
			fabs(fmod(static_cast<double>(Value - Threshold, Threshold * static_cast<auto>(4.0))))
			- Threshold * static_cast<auto>(2.0)
		)) - Threshold;
	}
	Value *= (1.0 / Drive);
	return Value;
};

/***************************************************************
*
* Tube
*
****************************************************************/
auto TubeDistortionReset = [](auto Drive, auto& Alpha, auto& Beta)
{
	Alpha = sin(((Drive * 30.0) / 101.0) * (3.14159265358979323846 / 2.0));
	Beta = 2.0 * Alpha / (1.0 - Alpha);
};

auto TubeFailure = [](auto Input, auto& Alpha, auto& Beta) 
{
	return (1.0 + Beta) * (Input) / (1.0 + Beta * fabs(Input)) * 0.9f;
};

auto TubeTriode = [](auto Input) -> auto
{
	auto x = (auto)In;

	// First tube
	x = sign(x) * ((2.0 - fabs(x)) * fabs(x));

	// Second Tube
	if (x < -1.0) {
		x = -1.0;
	} else if (x < -0.08905) {
		auto SixOrder = (fabs(x) - 0.032847);
		auto SevenOrder = 1.0 - pow((1.0 - SixOrder), 12.0) + SixOrder * (1.0 / 3.0);
		x = -(0.01 + (3.0 / 4.0) * SevenOrder);
	} else if (x < 0.320018) {
		x = -6.152 * x * x + x * 3.9375;
	} else {
		x = 0.630035;
	}

	return x;
};

/***************************************************************
*
* Sincrusher
*
****************************************************************/
auto Sincrusher = [](auto Input, auto Drive)
{
	// The simplest one, but not so ordinary. I like it!
	auto Value = In;
	Value *= Drive;
	Value = sin(Value);
	Value /= Drive;
	return Value;
};
