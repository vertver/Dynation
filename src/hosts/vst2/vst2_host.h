/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once
#include "aeffeditor.h"
#include "audioeffectx.h"

class VST2EditorHost : public AEffEditor
{
private:
	AudioEffect* pEffect;
	BaseView* PluginView;

public:
	VST2EditorHost(AudioEffect* pParentEffect, BaseView* pParentPluginView)
	{
		PluginView = pParentPluginView;
	}

	~VST2EditorHost()
	{

	}

	bool getRect(ERect** rect) override
	{
		SRect baseRect = {};
		ERect Winrect = {};
		PluginView->GetWindowSize(baseRect);
		Winrect.bottom = baseRect.bottom;
		Winrect.top = baseRect.top;
		Winrect.left = baseRect.left;
		Winrect.right = baseRect.right;
		*rect = &Winrect;
		return true;
	}

	bool open(void* ptr) override
	{
		AEffEditor::open(ptr);
		return PluginView->WindowAttached(ptr);
	}

	void close() override
	{
		PluginView->WindowRemoved();
		AEffEditor::close();
	}

	void idle() override
	{

	}
};

class VST2Notifier;
class VST2Host : public AudioEffectX
{
private:
	std::unique_ptr<VST2EditorHost> EditorHost;
	BasePlugin* Plugin = nullptr;
	BaseView* View = nullptr;

private:
	void processInternal(void** inputs, void** outputs, VstInt32 sampleFrames, bool isDouble);

private:
	void destroy();

public:
	VST2Host(
		audioMasterCallback audioMaster,
		BasePlugin* NewPlugin,
		BaseView* NewView
	);

	~VST2Host()
	{
		destroy();
	}

public:
	void suspend() override;
	void resume() override;

public:
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) override;
	void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames) override;

public:
	bool getEffectName(char* name) override;
	bool getVendorString(char* text) override;
	bool getProductString(char* text) override;
	VstInt32 getVendorVersion() override;

public:
	VstInt32 getChunk(void** data, bool isPreset) override;
	VstInt32 setChunk(void* data, VstInt32 byteSize, bool isPreset) override;

public:
	void setSampleRate(float sampleRate) override;
	void setBlockSize(VstInt32 blockSize) override;

public:
	void setProgram(VstInt32 program) override;
	void setProgramName(char* name) override;
	void getProgramName(char* name) override;
	bool getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text) override;

public:
	void setParameter(VstInt32 index, float value) override;
	float getParameter(VstInt32 index) override;
	void getParameterLabel(VstInt32 index, char* label) override;
	void getParameterDisplay(VstInt32 index, char* text) override;
	void getParameterName(VstInt32 index, char* text) override;
	bool canParameterBeAutomated(VstInt32 index) override;

public:
	bool getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput) override;
	bool setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput) override;

public:
	VstInt32 canDo(char* text)  override;
	VstInt32 processEvents(VstEvents* ev) override;
	VstPlugCategory getPlugCategory() override;
	VstInt32 beginLoadBank(VstPatchChunkInfo* ptr) override;
	VstInt32 beginLoadProgram(VstPatchChunkInfo* ptr) override;
	VstIntPtr vendorSpecific(VstInt32 lArg, VstIntPtr lArg2, void* ptrArg, float floatArg) override;
};

class VST2Notifier : public PluginNotifier
{
private:
	long RefCount = 0;
	VST2Host* ParentHost = nullptr;

public:
	VST2Notifier(VST2Host* InHost) : ParentHost(InHost) {}

public:
	void BeginEdit(int32_t ParameterIndex) override;
	void PerformEdit(int32_t ParameterIndex, float NormalizedValue) override;
	void EndEdit(int32_t ParameterIndex) override;
};
