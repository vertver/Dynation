/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "base.h"
#include "dynation_parameters.h"

class CStateStorage
{
private:
	StateType* CurrentState = nullptr;
	std::mutex DataLock;

public:
	CStateStorage(StateType* pInGainState) : CurrentState(pInGainState) {}
	~CStateStorage() { delete CurrentState; }

	void Lock() { DataLock.lock(); }
	void Unlock() { DataLock.unlock(); }

	void OnBeginEdit(int32_t OffsetOfEditing, int32_t SizeOfEditing) {}
	void OnEndEdit(int32_t OffsetOfEditing, int32_t SizeOfEditing) {}

	StateType* State() { return this->CurrentState; }
};

class DynationView : BaseView
{
private:
	BasePlugin* Parent = nullptr;

public:
	DynationView(BasePlugin* InParent) : Parent(InParent) {}
	~DynationView() {}

public:
	virtual bool Initialize(PluginNotifier* InNotifier) override;
	virtual void Destroy() override;

public:
	virtual bool WindowAttached(void* ParentWindow) override;
	virtual bool WindowRemoved() override;

public:
	virtual bool IsPluginResizable() override;
	virtual void GetWindowSize(SRect& rect) override;
	virtual bool CheckSizeConstraint(SRect& rect) override;

public:
	virtual bool InformResize(SRect& rect) override;

};

class DynationPlugin : BasePlugin
{
private:
	CStateStorage* State = nullptr;
	PluginNotifier* Notifier = nullptr;
	PluginInfo Info = {};

private:
	DynationState TempState = {};

private:
	int32_t Samplerate = 0;
	int32_t BlockSize = 0;
	int32_t InputChannels = 0;
	int32_t OutputChannels = 0;

	std::unique_ptr<DynationContainer> Parameters;

private:
	void Reset();

public:
	DynationPlugin(CStateStorage* InGainState);
	~DynationPlugin();

public:
	virtual void GetPluginInfo(PluginInfo*& Info) override;
	virtual bool Initialize(PluginNotifier* InNotifier) override;
	virtual void Destroy() override;

public:
	virtual void Suspend() override;
	virtual void Resume() override;

public:
	virtual void SetChannelsCount(int32_t InputChannels, int32_t OutputChannels) override;
	virtual void SetSamplerate(int32_t Samplerate) override;
	virtual void SetBlockSize(int32_t BlockSize) override;

public:
	virtual std::string_view GetParameterNameString(int32_t ParameterIndex) override;
	virtual std::string_view GetParameterLabelString(int32_t ParameterIndex) override;
	virtual std::string GetParameterValueString(int32_t ParameterIndex) override;

public:
	virtual int32_t GetParametersCount() override;
	virtual float GetParameter(int32_t ParameterIdx) override;
	virtual void SetParameter(int32_t ParameterIdx, float ParameterValue) override;

public:
	virtual void Process(AudioProcessingBlock<float>& ProcessingBlock) override;
	virtual void ProcessDouble(AudioProcessingBlock<double>& ProcessingBlock) override;
};