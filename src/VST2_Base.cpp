/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "VST2_Header.h"

#ifdef WIN32
void    VST2_DLL_Load( void* hInst ) {
    // current dll is load
    CreateWindowManager();
}

void    VST2_DLL_Unload( ) {
    // current dll is unload
    DestroyWindowManager();
}
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
    VST2_BASE* plugin_base = new VST2_BASE (audioMaster);
    if (plugin_base->Create()) return plugin_base;
    delete plugin_base;
    return nullptr;
}

VST2_BASE::VST2_BASE (audioMasterCallback audioMaster) : AudioEffectX (audioMaster, 1, 1) { 
    Plugin = nullptr;
    Plugin_Window = nullptr;
	IsOldReaper = true;
	IsReaper = false;
}

VST2_BASE::~VST2_BASE () { 
    Destroy( ); 
}


bool            VST2_BASE::Create( ) {
    Plugin = CreatePlugin( );
    if (!Plugin) return false;

    Plugin->Plugin_Base = this; // save base for client
    Plugin->Plugin_Window = nullptr;

    SetInitParams (Plugin->GetCountPresets(),  Plugin->GetCountParams());
    isSynth(Plugin->PluginIsSynth());
    setUniqueID(Plugin->GetUniqueID());

	// задаем кол-во поддерживаемых каналов, при этом НЕ МЕНЯЕМ кол-во обрабатываемых каналов!
    setNumInputs(2);	
    setNumOutputs(2);

    canProcessReplacing(); 
    canDoubleReplacing(Plugin->PluginIsDoubleSupport()); 
    programsAreChunks(true);

    if (Plugin->PluginIsUI())   Plugin_Window = new VST2_WINDOW(this);
    else                        Plugin_Window = nullptr;

    if (Plugin_Window && Plugin_Window->isCreate == false) {
        delete Plugin_Window; 
        Plugin_Window = nullptr; 
        return false;
    }

	string256 VendorString = {};
	getHostProductString(VendorString);
	if (!_strcmpi(VendorString, "REAPER")) IsReaper = true;
    if (!_strcmpi(VendorString, "Live")) {
        Plugin->SetIsAbleton(true);
    }

    setEditor(Plugin_Window); 
    setProgram(0);

    suspend();

    return true;
}

void            VST2_BASE::Destroy( ) {
    if (!Plugin) return;
    
    if (Plugin_Window) { // vst sdk сам удаляет окно но уже после релиза Plugin. поэтому сделаем это раньше его
        delete Plugin_Window;
        Plugin_Window = nullptr;
        setEditor(nullptr); 
    }
    
    delete Plugin;
    Plugin = nullptr;
}

bool            VST2_BASE::getEffectName(char* name) {
    vst_strncpy (name, Plugin->GetName(), kVstMaxEffectNameLen);
    return true;
}

bool            VST2_BASE::getVendorString(char* text) {
    vst_strncpy (text, Plugin->GetVendorName(), kVstMaxVendorStrLen);
    return true;
}

bool            VST2_BASE::getProductString(char* text) {
    PConstStr Productname = Plugin->GetProductName();
    vst_strncpy (text, Productname, kVstMaxProductStrLen);
    return true;
}

VstInt32        VST2_BASE::getVendorVersion() { 
    return Plugin->GetVersion(); 
}

void            VST2_BASE::suspend( ) {
    Plugin->Suspend();
}

void            VST2_BASE::resume( ) {
    Plugin->Resume();
}

VstInt32        VST2_BASE::getChunk (void** data, bool isPreset) {
    u32 ResultSize = Plugin->GetPresetSize();
    
    if (isPreset) {
        if (data) *data = Plugin->GetPresetDataCurrent();
    } else {
        ResultSize *= Plugin->GetCountPresets();

        if (data) *data = Plugin->GetPresetDataArr();
    }
    
    return ResultSize;
}

VstInt32        VST2_BASE::setChunk( void* data, VstInt32 byteSize, bool isPreset ) {
    u32 ResultSize = Plugin->GetPresetSize();
    
    if (isPreset) {
        if (ResultSize != byteSize) { assert(0); return 0; } //not valid preset
        if (!Plugin->SetPresetDataCurrent(data)) return 0;  //not valid preset  size
    } else {
        ResultSize *= Plugin->GetCountPresets();
        if (ResultSize != byteSize) { assert(0); return 0; } //not valid preset
        if (!Plugin->SetPresetDataArr(data)) return 0;  //not valid preset
    }
    
    return 1;
}

void            VST2_BASE::setSampleRate( float sampleRate ) {
	VstSpeakerArrangement* speakerArrang1 = nullptr;
	VstSpeakerArrangement* speakerArrang2 = nullptr;

    AudioEffectX::setSampleRate (sampleRate);
    Plugin->SetSampleRate(sampleRate);
}

void            VST2_BASE::setBlockSize( VstInt32 blockSize ) {
    AudioEffectX::setBlockSize (blockSize);
    Plugin->SetBlockSize(blockSize);
}

void            VST2_BASE::setProgram( VstInt32 program ) {
    if (program < 0 || program >= (i32)Plugin->GetCountPresets()) return;
    curProgram = program;
    Plugin->SetPreset(curProgram);
}

void            VST2_BASE::setProgramName( char* name ) {
    vst_strncpy(Plugin->GetPresetNameBuffer(curProgram), name, kVstMaxProgNameLen);
}

void            VST2_BASE::getProgramName( char* name ) {
    vst_strncpy(name, Plugin->GetPresetNameBuffer(curProgram), kVstMaxProgNameLen);
}

bool            VST2_BASE::getProgramNameIndexed( VstInt32 category, VstInt32 index, char* text ) {
    if (index < (i32)Plugin->GetCountPresets()) {
        vst_strncpy (text, Plugin->GetPresetNameBuffer(index), kVstMaxProgNameLen);
        return true;
    }
    return false;
}



void            VST2_BASE::setParameter( VstInt32 index, float value ) {
    Plugin->SetParametr(index, value);
}

float           VST2_BASE::getParameter( VstInt32 index ) {
    return Plugin->GetParametr(index);
}

void            VST2_BASE::getParameterLabel( VstInt32 index, char* label ) {
    Plugin->GetParameterLabel8(index, label);
    label[kVstMaxParamStrLen] = '\0';
}

void            VST2_BASE::getParameterDisplay( VstInt32 index, char* text ) {
    Plugin->GetParametrDisplay8(index, text);
    text[kVstMaxParamStrLen] = '\0';
}

void            VST2_BASE::getParameterName( VstInt32 index, char* text ) {
    Plugin->GetParametrName8(index, text);
    text[kVstMaxParamStrLen] = '\0';
}

bool			VST2_BASE::getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput)
{
	if (!IsOldReaper)
	{
		try
		{
			*pluginInput = InArrang;
			*pluginOutput = OutArrang;
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	else return false;
}

bool			VST2_BASE::setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput)
{
	if (IsOldReaper && IsReaper)
	{
		return false;
	}

	InArrang = pluginInput;
	OutArrang = pluginOutput;

	InVstChannels = InArrang->numChannels;
	OutVstChannels = OutArrang->numChannels;

	if (InVstChannels > 8) { InVstChannels = 8; };
	if (OutVstChannels > 8) { OutVstChannels = 8; };

	setNumInputs(InVstChannels);
	setNumOutputs(OutVstChannels);
	return true;
}

bool            VST2_BASE::canParameterBeAutomated( VstInt32 index ) {
    return Plugin->GetParametrIsAutomated(index);
}

VstInt32        VST2_BASE::canDo(char* text) {
    if (Plugin->PluginIsEventSupport()){ 
        if (!strcmp (text, "receiveVstEvents"))     return 1;
        if (!strcmp (text, "receiveVstMidiEvent"))  return 1;
    }

	if (!strcmp(text, "hasCockosNoScrollUI"))	return 0xbeef0000;
	if (!strcmp(text, "wantsChannelCountNotifications"))
	{
		IsOldReaper = false;
		return 1;
	}

    return -1;
}

void            VST2_BASE::processReplacing( float** inputs, float** outputs, VstInt32 sampleFrames ) {
	u32  InCount = InVstChannels ? InVstChannels : Plugin->GetInputCount();
	u32  OutCount = OutVstChannels ? OutVstChannels : Plugin->GetOutputCount();

    i32 SampleFramesLeft = sampleFrames;

    VstEvent* LeftEvent = nullptr;

    i32 CurrentSample = 0;
    i32 SampleRendering = 0;


    while ( SampleFramesLeft > 0 ) {

        if ( InputEventPos < InputEventCount ) {
            LeftEvent       = &InputEventBuffer[InputEventPos];
            SampleRendering = LeftEvent->deltaFrames - CurrentSample;
            if (SampleRendering < 0) SampleRendering = 0;
            if (SampleRendering > SampleFramesLeft) SampleRendering = SampleFramesLeft;
        } else {
            LeftEvent       = nullptr;
            SampleRendering = SampleFramesLeft;
        }

        if ( SampleRendering > 0 ) {
            PROCESS_DATA ProcessData;
            for ( u32 i = 0; i < InCount; i++ ) ProcessData.In[i] = &inputs[i][CurrentSample];
            for ( u32 i = 0; i < OutCount; i++ ) ProcessData.Out[i] = &outputs[i][CurrentSample];
            ProcessData.SamplesCount     = SampleRendering;
			ProcessData.InChannelsCount  = InCount;
            ProcessData.OutChannelsCount = OutCount;

			if (ProcessData.SamplesCount)
            Plugin->Process(&ProcessData);

            SampleFramesLeft    -= SampleRendering;
            CurrentSample       += SampleRendering;
        }

        if (LeftEvent) {
            InputEventParse( LeftEvent );
            InputEventPos++;
        }
    }
}

void            VST2_BASE::processDoubleReplacing( double** inputs, double** outputs, VstInt32 sampleFrames) {
	u32  InCount = InVstChannels ? InVstChannels : Plugin->GetInputCount();
	u32  OutCount = OutVstChannels ? OutVstChannels : Plugin->GetOutputCount();

    i32 SampleFramesLeft = sampleFrames;

    VstEvent* LeftEvent = nullptr;

    i32 CurrentSample = 0;
    i32 SampleRendering = 0;

    while ( SampleFramesLeft > 0 ) {

        if ( InputEventPos < InputEventCount ) {
            LeftEvent       = &InputEventBuffer[InputEventPos];
            SampleRendering = LeftEvent->deltaFrames - CurrentSample;
            if (SampleRendering < 0) SampleRendering = 0;
            if (SampleRendering > SampleFramesLeft) SampleRendering = SampleFramesLeft;
        } else {
            LeftEvent       = nullptr;
            SampleRendering = SampleFramesLeft;
        }

        if ( SampleRendering > 0 ) {
            PROCESS_DATA_DOUBLE ProcessData;
            for ( u32 i = 0; i < InCount; i++ ) ProcessData.In[i] = &inputs[i][CurrentSample];
            for ( u32 i = 0; i < OutCount; i++ ) ProcessData.Out[i] = &outputs[i][CurrentSample];
            ProcessData.SamplesCount     = SampleRendering;
            ProcessData.InChannelsCount  = InCount;
            ProcessData.OutChannelsCount = OutCount;

            Plugin->ProcessDouble(&ProcessData);

            SampleFramesLeft    -= SampleRendering;
            CurrentSample       += SampleRendering;
        }

        if (LeftEvent) {
            InputEventParse( LeftEvent );
            InputEventPos++;
        }
    }
}

void            VST2_BASE::InputEventParse( VstEvent* Event ) {
    VstMidiEvent* LeftEvent = (VstMidiEvent*)Event;

    switch (LeftEvent->midiData[0] & 0xf0) {
        case 0x80: // note off
            Plugin->NoteOff((i32)LeftEvent->midiData[1]);
            break;
        case 0x90: // note on
            Plugin->NoteOn((i32)LeftEvent->midiData[1], (f32)LeftEvent->midiData[2]/127.0f);
            break;
        case 0xE0: // pitch
            Plugin->PitchBend((f32)((i32)LeftEvent->midiData[2] * 128 + (i32)LeftEvent->midiData[1] - 8192)/ 8192.0f);
            break;
        default: 
            if((i32)LeftEvent->midiData[1] > 0x7A)  Plugin->NoteOffAll();
        break;
    }
}

VstInt32        VST2_BASE::processEvents(VstEvents* ev) {
    if (ev->numEvents == 0) return 1;

    InputEventCount = 0;
    InputEventPos = 0;

    //сохраняем все сообщения в динамический буфер
    for (i32 i = 0; i < ev->numEvents; i++) {
        if((ev->events[i])->type != kVstMidiType) continue;
        if (InputEventCount >= MIDIEVENT_COUNT) break;//ни одна нормальная daw нешлёт столько миди сообщений

        if ( (((VstMidiEvent*)ev->events[i])->midiData[0] & 0xf0) != 0xf0) {
            InputEventBuffer[InputEventCount++] = *(ev->events[i]);
        }
    }

    ////CurrentSample = 0; надо будет решить надо делать так или нет

    return 1;
}

VstPlugCategory VST2_BASE::getPlugCategory()
{
	return kPlugCategEffect;
}

/*
while (PeekMessage(&msg, (HWND)ms32handle.window, 0, 0, PM_REMOVE) || PeekMessage(&msg, 0, WM_TIMER, WM_TIMER, PM_REMOVE)) {
// needed to forward WM_CHAR
TranslateMessage(&msg);
DispatchMessage(&msg);
if (msg.message == WM_QUIT) {
ret = (int)msg.wParam;
set_active(false);
}
}
*/