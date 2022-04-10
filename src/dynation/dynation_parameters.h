#pragma once

typedef std::variant<
	std::monostate,
	bool,
	float,
	int16_t,

	DistortionType,
	CompressorStatus,
	CompressorMode
	//GainLevel
	//QRatio,
	//CompressorRatio,
	//LinearHertz,
	//Log10Hertz,
	//LogTime,
	//FilterSlopeValue
> DynationParamType;
using DynationContainer = ParametersContainer<PARAMETERS_COUNT, DynationParamType>;

using StateType = DynationState;

