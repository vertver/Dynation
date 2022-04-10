/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

/***************************************************************
*
* Bitshifter
*
****************************************************************/
auto BitcrusherReset = [](auto DepthBits, auto& ValuePow) 
{
	ValuePow = static_cast<auto>(pow(2.0, static_cast<double>(DepthBits))) - 1;
};

auto Bitcrusher = [](auto Input, auto& ValuePow)
{
	double Step = (Input + 1.0) * static_cast<double>(ValuePow);
	double RoundedStep = ((Step > 0.0) ? floor(Step + 0.5) : ceil(Step - 0.5));
	return RoundedStep / static_cast<double>(ValuePow) - 1.0;
};

/***************************************************************
*
* Downshifter
*
****************************************************************/
auto Downshifter = [](auto Input, auto& SaveLoadValue, int32_t& SaveLoadValue2, int32_t SampleRateDivNeadRate)
{
	if (SaveLoadValue2 == 0) {
		SaveLoadValue = Input;
	}

	SaveLoadValue2++;
	if (SaveLoadValue2 >= SampleRateDivNeadRate) {
		SaveLoadValue2 = 0;
	}

	return SaveLoadValue;
};

/***************************************************************
*
* ADC Failure
*
****************************************************************/
auto ADCFailure = [](auto Input, auto& SaveLoadValue, auto Error) {
	if (Error == 0.0) {
		return Input;
	}

	auto RandPercent = (auto)(rand() % (100 + 1)) / 100.0;
	if (RandPercent < Error) {
		return SaveLoadValue;
	}

	SaveLoadValue = Input;
	return Input;
};
