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

	// This is fucking magic. Isn't it?
	using ParameterPair = std::pair<DynationParamType, ParameterStruct>;
	std::initializer_list<ParameterPair> List =
	{
		{ DistortionType::NoneType,			{ "Distortion type",	static_cast<void*>(&ThisState.DistortType) } },
		{ static_cast<int16_t>(0),			{ "Reserved",			static_cast<void*>(&ThisState.DistortType) } },

		{ 1.f,								{ "Dry/Wet",			static_cast<void*>(&ThisState.DryWet) } },
		{ volume_gain(1.f),					{ "Input volume",		static_cast<void*>(&ThisState.InputVolume) } },
		{ volume_gain(1.f),					{ "Output volume",		static_cast<void*>(&ThisState.OutputVolume) } },
		{ 0.5f,								{ "Tilt EQ",			static_cast<void*>(&ThisState.TiltEQ) } },
		{ 0.0f,								{ "Drive",				static_cast<void*>(&ThisState.Drive) } },
		{ 0.0f,								{ "Hardness",			static_cast<void*>(&ThisState.Hardness) } },
		{ 0.0f,								{ "Downshifter",		static_cast<void*>(&ThisState.Downshifter) } },
		{ 0.0f,								{ "Bitshifter",			static_cast<void*>(&ThisState.Bitshifter) } },

		// First compressor
		{ CompressorStatus::Disabled,		{ "1 status",			static_cast<void*>(&ThisState.FirstCompressor.CompStatus) } },
		{ CompressorMode::BasicCompressor,	{ "1 mode",				static_cast<void*>(&ThisState.FirstCompressor.CompStatus) } },
		{ static_cast<int16_t>(0),			{ "1 reserved",			static_cast<void*>(&ThisState.FirstCompressor.reserved2) } },
		{ static_cast<int16_t>(0),			{ "1 reserved2",		static_cast<void*>(&ThisState.FirstCompressor.reserved3) } },																					
		{ 0.f,								{ "1 attack",			static_cast<void*>(&ThisState.FirstCompressor.Attack) } },
		{ 0.f,								{ "1 release",			static_cast<void*>(&ThisState.FirstCompressor.Release) } },
		{ log_gain(1.f),					{ "1 threshold",		static_cast<void*>(&ThisState.FirstCompressor.Threshold) } },
		{ 0.f,								{ "1 ratio",			static_cast<void*>(&ThisState.FirstCompressor.Ratio) } },	
		{ 0.f,								{ "1 reserved3",		static_cast<void*>(&ThisState.FirstCompressor.Reserved) } },
		{ 1.f,								{ "1 parallel mixing",	static_cast<void*>(&ThisState.FirstCompressor.ParallelMix) } },
		{ volume_gain(1.f),					{ "1 pump gain",		static_cast<void*>(&ThisState.FirstCompressor.PumpGain) } },
		{ 0.f,								{ "1 analog submix",	static_cast<void*>(&ThisState.FirstCompressor.AnalogSubmix) } },

		// Second compressor
		{ CompressorStatus::Disabled,		{ "2 status",			static_cast<void*>(&ThisState.SecondCompressor.CompStatus) } },
		{ CompressorMode::BasicCompressor,	{ "2 mode",				static_cast<void*>(&ThisState.SecondCompressor.CompStatus) } },
		{ static_cast<int16_t>(0),			{ "2 reserved",			static_cast<void*>(&ThisState.SecondCompressor.reserved2) } },
		{ static_cast<int16_t>(0),			{ "2 reserved2",		static_cast<void*>(&ThisState.SecondCompressor.reserved3) } },																					
		{ 0.f,								{ "2 attack",			static_cast<void*>(&ThisState.SecondCompressor.Attack) } },
		{ 0.f,								{ "2 release",			static_cast<void*>(&ThisState.SecondCompressor.Release) } },
		{ log_gain(1.f),					{ "2 threshold",		static_cast<void*>(&ThisState.SecondCompressor.Threshold) } },
		{ 0.f,								{ "2 ratio",			static_cast<void*>(&ThisState.SecondCompressor.Ratio) } },	
		{ 0.f,								{ "2 reserved3",		static_cast<void*>(&ThisState.SecondCompressor.Reserved) } },
		{ 1.f,								{ "2 parallel mixing",	static_cast<void*>(&ThisState.SecondCompressor.ParallelMix) } },
		{ volume_gain(1.f),					{ "2 pump gain",		static_cast<void*>(&ThisState.SecondCompressor.PumpGain) } },
		{ 0.f,								{ "2 analog submix",	static_cast<void*>(&ThisState.SecondCompressor.AnalogSubmix) } },
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
