/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "dynation.h"

bool 
CreatePluginInterface(BasePlugin*& OutPlugin, BaseView*& OutView)
{
	OutPlugin = reinterpret_cast<BasePlugin*>(new DynationPlugin(new CStateStorage(new StateType())));
#ifdef PLUGIN_UI_SUPPORT
	OutView = reinterpret_cast<BaseView*>(new DynationView(OutPlugin));
#endif

	return true;
}

DynationPlugin::DynationPlugin(CStateStorage* InGainState) 
	: State(InGainState)
{
	StateType& ThisState = *State->State();

	/*
		This stuff has become so huge that it will now have to write a huge description.

		What you see in front of you is an initializer list with the arguments needed to 
		create plugin parameters. These include:

		1. parameter type and default value for it. To do this, we use std::variant with a 
		subset of types in C++

		2. A structure with information and data. The first member of the structure is the
		parameter name, it is a string. The second member of the parameter is a raw pointer 
		to memory where the main variable is stored, which will be changed after the delegate 
		call.

		3. The delegate, which is called before the value of the variable is changed. It is 
		copyable because it can be in more than one location. Also, keep in mind that this 
		delegate should preferably point to a lambda that is inside the parent class.
	*/
	using ParameterPair = std::tuple<DynationParamType, ParameterStruct, fu2::function<void(DynationParamType)>>;
	const std::initializer_list<ParameterPair> List = {
		{ DistortionType::NoneType,			{ "Distortion type",	static_cast<void*>(&ThisState.DistortType)						}, 
			[this](auto Input) 
			{
				// If we've received that current distortion type is not valid - reset
				// all stuff and prepare for new data.
				auto NewType = std::get<DistortionType>(Input);
				if (NewType != State->State()->DistortType) {
					State->State()->DistortType = NewType;
					ResetDistortion();
				}
			} 
		},
		{ static_cast<int16_t>(0),			{ "reserved",			static_cast<void*>(&ThisState.reserved)							}, [this](auto Input) {} },

		{ 1.f,								{ "Dry/Wet",			static_cast<void*>(&ThisState.DryWet)							}, [this](auto Input) {} },
		{ volume_gain(1.f),					{ "Input volume",		static_cast<void*>(&ThisState.InputVolume)						}, [this](auto Input) {} },
		{ volume_gain(1.f),					{ "Output volume",		static_cast<void*>(&ThisState.OutputVolume)						}, [this](auto Input) {} },
		{ 0.5f,								{ "Tilt EQ",			static_cast<void*>(&ThisState.TiltEQ)							}, [this](auto Input) {} },
		{ 0.0f,								{ "Drive",				static_cast<void*>(&ThisState.Drive)							}, [this](auto Input) {} },
		{ 0.0f,								{ "Hardness",			static_cast<void*>(&ThisState.Hardness)							}, [this](auto Input) {} },
		{ 0.0f,								{ "Downshifter",		static_cast<void*>(&ThisState.Downshifter)						}, [this](auto Input) {} },
		{ 0.0f,								{ "Bitshifter",			static_cast<void*>(&ThisState.Bitshifter)						}, [this](auto Input) {} },


		{ CompressorStatus::Disabled,		{ "1 status",			static_cast<void*>(&ThisState.FirstCompressor.CompStatus)		}, [this](auto Input) {} },
		{ CompressorMode::BasicCompressor,	{ "1 mode",				static_cast<void*>(&ThisState.FirstCompressor.CompStatus)		}, [this](auto Input) {} },
		{ static_cast<int16_t>(0),			{ "1 reserved2",		static_cast<void*>(&ThisState.FirstCompressor.reserved2)		}, [this](auto Input) {} },
		{ static_cast<int16_t>(0),			{ "1 reserved3",		static_cast<void*>(&ThisState.FirstCompressor.reserved3)		}, [this](auto Input) {} },												
		{ 0.f,								{ "1 attack",			static_cast<void*>(&ThisState.FirstCompressor.Attack)			}, [this](auto Input) {} },
		{ 0.f,								{ "1 release",			static_cast<void*>(&ThisState.FirstCompressor.Release)			}, [this](auto Input) {} },
		{ log_gain(1.f),					{ "1 threshold",		static_cast<void*>(&ThisState.FirstCompressor.Threshold)		}, [this](auto Input) {} },
		{ 0.f,								{ "1 ratio",			static_cast<void*>(&ThisState.FirstCompressor.Ratio)			}, [this](auto Input) {} },
		{ 0.f,								{ "1 reserved3",		static_cast<void*>(&ThisState.FirstCompressor.Reserved)			}, [this](auto Input) {} },
		{ 1.f,								{ "1 parallel mixing",	static_cast<void*>(&ThisState.FirstCompressor.ParallelMix)		}, [this](auto Input) {} },
		{ volume_gain(1.f),					{ "1 pump gain",		static_cast<void*>(&ThisState.FirstCompressor.PumpGain)			}, [this](auto Input) {} },
		{ 0.f,								{ "1 analog submix",	static_cast<void*>(&ThisState.FirstCompressor.AnalogSubmix)		}, [this](auto Input) {} },

		{ CompressorStatus::Disabled,		{ "2 status",			static_cast<void*>(&ThisState.SecondCompressor.CompStatus)		}, [this](auto Input) {} },
		{ CompressorMode::BasicCompressor,	{ "2 mode",				static_cast<void*>(&ThisState.SecondCompressor.CompStatus)		}, [this](auto Input) {} },
		{ static_cast<int16_t>(0),			{ "2 reserved",			static_cast<void*>(&ThisState.SecondCompressor.reserved2)		}, [this](auto Input) {} },
		{ static_cast<int16_t>(0),			{ "2 reserved2",		static_cast<void*>(&ThisState.SecondCompressor.reserved3)		}, [this](auto Input) {} },												
		{ 0.f,								{ "2 attack",			static_cast<void*>(&ThisState.SecondCompressor.Attack)			}, [this](auto Input) {} },
		{ 0.f,								{ "2 release",			static_cast<void*>(&ThisState.SecondCompressor.Release)			}, [this](auto Input) {} },
		{ log_gain(1.f),					{ "2 threshold",		static_cast<void*>(&ThisState.SecondCompressor.Threshold)		}, [this](auto Input) {} },
		{ 0.f,								{ "2 ratio",			static_cast<void*>(&ThisState.SecondCompressor.Ratio)			}, [this](auto Input) {} },
		{ 0.f,								{ "2 reserved3",		static_cast<void*>(&ThisState.SecondCompressor.Reserved)		}, [this](auto Input) {} },
		{ 1.f,								{ "2 parallel mixing",	static_cast<void*>(&ThisState.SecondCompressor.ParallelMix)		}, [this](auto Input) {} },
		{ volume_gain(1.f),					{ "2 pump gain",		static_cast<void*>(&ThisState.SecondCompressor.PumpGain)		}, [this](auto Input) {} },
		{ 0.f,								{ "2 analog submix",	static_cast<void*>(&ThisState.SecondCompressor.AnalogSubmix)	}, [this](auto Input) {} },
	};

	Parameters = std::make_unique<DynationContainer>(List);

	Info.Name = "Dynation";
	Info.Product = "Dynation";
	Info.Vendor = "Anton Kovalev";
	Info.Version = 14;
}

DynationPlugin::~DynationPlugin()
{
	delete State;
}

void 
DynationPlugin::GetPluginInfo(PluginInfo*& Info)
{
	Info = &this->Info;
}

void 
DynationPlugin::Reset()
{

}

bool
DynationPlugin::Initialize(PluginNotifier* InNotifier)
{
	if (InNotifier == nullptr) {
		return false;
	}

	Notifier = InNotifier;
	return true;
}

void 
DynationPlugin::Destroy()
{
	delete Notifier;
}

void 
DynationPlugin::Suspend()
{
	Reset();
}

void
DynationPlugin::Resume()
{
	Reset();
}

void
DynationPlugin::SetChannelsCount(int32_t InputChannels, int32_t OutputChannels)
{
	this->InputChannels = InputChannels;
	this->OutputChannels = OutputChannels;
}

void 
DynationPlugin::SetSamplerate(int32_t Samplerate)
{
	this->Samplerate = Samplerate;
}

void
DynationPlugin::SetBlockSize(int32_t BlockSize)
{
	this->BlockSize = BlockSize;
}

std::string_view
DynationPlugin::GetParameterNameString(int32_t ParameterIndex)
{
	return Parameters->GetParameterName(ParameterIndex);
}

std::string_view
DynationPlugin::GetParameterLabelString(int32_t ParameterIndex)
{
	return Parameters->GetParameterSymbol(ParameterIndex);
}

std::string
DynationPlugin::GetParameterValueString(int32_t ParameterIndex)
{
	return Parameters->GetParameterValueString(ParameterIndex);
}

int32_t 
DynationPlugin::GetParametersCount()
{
	return Parameters->GetParamsCount();
}

float 
DynationPlugin::GetParameter(int32_t ParameterIdx)
{
	float OutValue = 0.f;

	State->Lock();
	Parameters->GetValueNormalized(ParameterIdx, OutValue);
	State->Unlock();

	return OutValue;
}

void 
DynationPlugin::SetParameter(int32_t ParameterIdx, float ParameterValue)
{
	State->Lock();
	Parameters->SetValueNormalized(ParameterIdx, ParameterValue); 
	State->Unlock();
}
