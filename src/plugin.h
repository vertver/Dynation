/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

struct SRect
{
	int32_t top;
	int32_t left;
	int32_t bottom;
	int32_t right;
};

template<typename T>
struct AudioProcessingBlock
{
	int32_t FramesToProcess = 0;
	int32_t FramesPosition = 0;

	T** Sidechain = nullptr;
	T** Inputs = nullptr;
	T** Outputs = nullptr;
};

class PluginNotifier
{
public:
	virtual void BeginEdit(int32_t ParameterIndex) = 0;
	virtual void PerformEdit(int32_t ParameterIndex, float NormalizedValue) = 0;
	virtual void EndEdit(int32_t ParameterIndex) = 0;
};

class BaseView
{
public:
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;

public:
	virtual bool WindowAttached(void* ParentWindow) = 0;
	virtual bool WindowRemoved() = 0;

public:
	virtual bool IsPluginResizable() = 0;
	virtual void GetWindowSize(SRect& rect) = 0;
	virtual bool CheckSizeConstraint(SRect& rect) = 0;

public:
	virtual bool InformResize(SRect& rect) = 0;
};

struct PluginInfo
{
	int32_t Reserved;
	int32_t Version;
	std::string_view Name;
	std::string_view Vendor;
	std::string_view Product;
};

class BasePlugin
{
public:
	virtual void GetPluginInfo(PluginInfo*& Info) = 0;
	virtual bool Initialize(PluginNotifier* InNotifier) = 0;
	virtual void Destroy() = 0;

public:
	virtual void Suspend() = 0;
	virtual void Resume() = 0;

public:
	virtual void SetChannelsCount(int32_t InputChannels, int32_t OutputChannels) = 0;
	virtual void SetSamplerate(int32_t Samplerate) = 0;
	virtual void SetBlockSize(int32_t BlockSize) = 0;

public:
	virtual std::string_view GetParameterNameString(int32_t ParameterIndex) = 0;
	virtual std::string_view GetParameterLabelString(int32_t ParameterIndex) = 0;
	virtual std::string GetParameterValueString(int32_t ParameterIndex) = 0;

public:
	virtual int32_t GetParametersCount() = 0;
	virtual float GetParameter(int32_t ParameterIdx) = 0;
	virtual void SetParameter(int32_t ParameterIdx, float ParameterValue) = 0;

public:
	virtual void Process(AudioProcessingBlock<float>& ProcessingBlock) = 0;
	virtual void ProcessDouble(AudioProcessingBlock<double>& ProcessingBlock) = 0;
};

extern bool CreatePluginInterface(BasePlugin*& OutPlugin, BaseView*& OutView);