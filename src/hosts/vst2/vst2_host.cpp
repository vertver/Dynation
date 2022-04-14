/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "pch.h"

void
VST2Notifier::BeginEdit(int32_t ParameterIndex)
{
	ParentHost->endEdit(ParameterIndex);
}

void
VST2Notifier::PerformEdit(int32_t ParameterIndex, float NormalizedValue)
{
	ParentHost->setParameter(ParameterIndex, NormalizedValue);
	ParentHost->setParameterAutomated(ParameterIndex, NormalizedValue);
}

void
VST2Notifier::EndEdit(int32_t ParameterIndex)
{
	ParentHost->endEdit(ParameterIndex);
}

VST2Host::VST2Host(
	audioMasterCallback audioMaster,
	BasePlugin* NewPlugin,
	BaseView* NewView
) : 
	AudioEffectX(audioMaster, 1, 1),
	Plugin(NewPlugin),
	View(NewView)
{
	if (audioMaster == nullptr) {
		throw new std::exception("audioMaster is nullptr");
	}

	if (Plugin == nullptr) {
		throw new std::exception("Plugin is nullptr");
	}

	cEffect.numPrograms = 1;
	cEffect.numParams = Plugin->GetParametersCount();
	numPrograms = cEffect.numPrograms;
	numParams = cEffect.numParams;

	isSynth(false);
	setUniqueID(020202002);

	setNumInputs(2);
	setNumOutputs(2);
	Plugin->SetChannelsCount(2, 2);

	canProcessReplacing(true);
	canDoubleReplacing(false);
	programsAreChunks(true);

	VST2Notifier* TempNotifier = new VST2Notifier(this);
	if (!Plugin->Initialize(TempNotifier)) {
		delete TempNotifier;
		throw new std::exception("Can't initialize plugin. aborting...");
	}

	if (View != nullptr) {
		if (View->Initialize(TempNotifier)) {
			throw new std::exception("View can't be initialized. Something wrong inside.");
		}

		EditorHost = std::make_unique<VST2EditorHost>(this, NewView);
	}

	setEditor(EditorHost.get());
	setProgram(0);
	suspend();
}

void 
VST2Host::destroy()
{
	if (View != nullptr) {
		delete View;
		View = nullptr;
	}

	if (Plugin != nullptr) {
		delete Plugin;
		Plugin = nullptr;
	}
}

void
VST2Host::suspend()
{
	Plugin->Suspend();
}

void
VST2Host::resume()
{
	Plugin->Resume();
}

void 
VST2Host::processInternal(void** inputs, void** outputs, VstInt32 sampleFrames, bool isDouble)
{;
	if (isDouble) {
		AudioProcessingBlock<double> ProcessBlock;
		ProcessBlock.FramesPosition = 0;
		ProcessBlock.FramesToProcess = sampleFrames;
		ProcessBlock.Inputs = reinterpret_cast<double**>(inputs);
		ProcessBlock.Outputs = reinterpret_cast<double**>(outputs);
		ProcessBlock.Sidechain = nullptr;
		Plugin->ProcessDouble(ProcessBlock);
	}
	else {
		AudioProcessingBlock<float> ProcessBlock;
		ProcessBlock.FramesPosition = 0;
		ProcessBlock.FramesToProcess = sampleFrames;
		ProcessBlock.Inputs = reinterpret_cast<float**>(inputs);
		ProcessBlock.Outputs = reinterpret_cast<float**>(outputs);
		ProcessBlock.Sidechain = nullptr;
		Plugin->Process(ProcessBlock);
	}
}

void 
VST2Host::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	processInternal(
		reinterpret_cast<void**>(inputs), 
		reinterpret_cast<void**>(outputs),
		sampleFrames,
		false
	);
}

void 
VST2Host::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
	processInternal(
		reinterpret_cast<void**>(inputs),
		reinterpret_cast<void**>(outputs),
		sampleFrames,
		true
	);
}

bool
VST2Host::getEffectName(char* name)
{
	PluginInfo* Info = nullptr;
	Plugin->GetPluginInfo(Info);
	std::memset(name, 0, Info->Vendor.size() + 1);	// REAPER DEVELOPERS PLEASE FIX IT
	Info->Name.copy(name, Info->Name.size());
	name[Info->Name.size()] = 0;

	return true;
}

bool
VST2Host::getVendorString(char* text)
{
	PluginInfo* Info = nullptr;
	Plugin->GetPluginInfo(Info);
	std::memset(text, 0, Info->Vendor.size() + 1);	// REAPER DEVELOPERS PLEASE FIX IT
	Info->Vendor.copy(text, Info->Vendor.size());
	text[Info->Name.size()] = 0;

	return true;
}

bool
VST2Host::getProductString(char* text)
{
	PluginInfo* Info = nullptr;
	Plugin->GetPluginInfo(Info);
	std::memset(text, 0, Info->Vendor.size() + 1);	// REAPER DEVELOPERS PLEASE FIX IT
	Info->Product.copy(text, Info->Product.size());
	text[Info->Name.size()] = 0;

	return true;
}

VstInt32 
VST2Host::getVendorVersion()
{
	PluginInfo* Info = nullptr;
	Plugin->GetPluginInfo(Info);
	return Info->Version;
}

VstInt32
VST2Host::getChunk(void** data, bool isPreset)
{
	return 0;
}

VstInt32
VST2Host::setChunk(void* data, VstInt32 byteSize, bool isPreset)
{
	return 0;
}

void
VST2Host::setSampleRate(float sampleRate)
{
	Plugin->SetSamplerate(static_cast<int32_t>(sampleRate));
}

void
VST2Host::setBlockSize(VstInt32 blockSize)
{
	Plugin->SetBlockSize(blockSize);
}

void
VST2Host::setProgram(VstInt32 program)
{
	// do nothing
}

void
VST2Host::setProgramName(char* name)
{
	// do nothing
}

void 
VST2Host::getProgramName(char* name)
{
	memcpy(name, "Default", 8);
}

bool 
VST2Host::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
	return false;
}

void 
VST2Host::setParameter(VstInt32 index, float value)
{
	Plugin->SetParameter(index, value);
}

float
VST2Host::getParameter(VstInt32 index)
{
	return Plugin->GetParameter(index);
}

void 
VST2Host::getParameterLabel(VstInt32 index, char* label)
{
	auto LabelString = Plugin->GetParameterLabelString(index);
	std::memset(label, 0, LabelString.size() + 1);	// REAPER DEVELOPERS PLEASE FIX IT
	LabelString.copy(label, LabelString.size());
}

void
VST2Host::getParameterDisplay(VstInt32 index, char* text)
{
	auto ValueString = Plugin->GetParameterValueString(index);
	std::memset(text, 0, ValueString.size() + 1);	// REAPER DEVELOPERS PLEASE FIX IT
	ValueString.copy(text, ValueString.size());
}

void 
VST2Host::getParameterName(VstInt32 index, char* text)
{
	auto NameString = Plugin->GetParameterNameString(index);
	std::memset(text, 0, NameString.size() + 1);	// REAPER DEVELOPERS PLEASE FIX IT
	NameString.copy(text, NameString.size());
}

bool
VST2Host::getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput)
{
	return false;
}

bool 
VST2Host::setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput)
{
	return false;
}

bool 
VST2Host::canParameterBeAutomated(VstInt32 index)
{
	return false;
}

VstInt32 
VST2Host::canDo(char* text)
{
	VstInt32 ret = -1;
	if (!strcmp(text, "hasCockosExtensions")) {
		ret = 0xbeef0000;
	}

	if (!strcmp(text, "hasCockosViewAsConfig")) {
		ret = 0xbeef0000;
	}

	if (!strcmp(text, "hasCockosNoScrollUI")) {
		ret = 0xbeef0000;
	}

	if (!strcmp(text, "hasCockosEmbeddedUI")) {
		ret = 0xbeef0000;
	}

	if (!strcmp(text, "wantsChannelCountNotifications")) {
		ret = 1;
	}

	return ret;
}

VstInt32 
VST2Host::processEvents(VstEvents* ev)
{
	if (ev->numEvents == 0) {
		return 1;
	}

	return 0;
}

VstPlugCategory
VST2Host::getPlugCategory()
{
	return VstPlugCategory::kPlugCategEffect;
}

VstInt32
VST2Host::beginLoadBank(VstPatchChunkInfo* ptr)
{
	return 0;
}

VstInt32 
VST2Host::beginLoadProgram(VstPatchChunkInfo* ptr)
{
	return 0;
}

VstIntPtr 
VST2Host::vendorSpecific(VstInt32 lArg, VstIntPtr lArg2, void* ptrArg, float floatArg)
{
	return 0;
}
