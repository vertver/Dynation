/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "Header.h"
#define sign(x)  (signbit(x) ?  -1 : 1)

// доделать
#ifndef WIN32
#define sprintf_s(x, y, z, ...) sprintf(x, z, ...)

#endif

const char* NameTypesDist[] = { 
    "Tube Distortion",
    "Sincrusher", 
    "Hard Distortion", 
    "F-Back Distortion", 
    "Soft Clip-p-per",  
    "Soft Clip-p-per (alt)" 
};


//------------------------------------------------------------------------
/** Must be implemented externally. */
extern AudioEffect* createEffectInstance(audioMasterCallback audioMaster);

extern "C" {

#if defined (__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define VST_EXPORT	__attribute__ ((visibility ("default")))
#else
#define VST_EXPORT __declspec(dllexport)
#endif

    //------------------------------------------------------------------------
    /** Prototype of the export function main */
    //------------------------------------------------------------------------
    VST_EXPORT AEffect* VSTPluginMain(audioMasterCallback audioMaster)
    {
        // Get VST Version of the Host
        if (!audioMaster(0, audioMasterVersion, 0, 0, 0, 0))
            return 0;  // old version

        // Create the AudioEffect
        AudioEffect* effect = createEffectInstance(audioMaster);
        if (!effect)
            return 0;

        // Return the VST AEffect structur
        return effect->getAeffect();
    }

    // support for old hosts not looking for VSTPluginMain
#if (TARGET_API_MAC_CARBON && __ppc__)
    VST_EXPORT AEffect* main_macho(audioMasterCallback audioMaster) { return VSTPluginMain(audioMaster); }
#elif WIN32
    VST_EXPORT AEffect* MAIN(audioMasterCallback audioMaster) { return VSTPluginMain(audioMaster); }
#elif BEOS
    VST_EXPORT AEffect* main_plugin(audioMasterCallback audioMaster) { return VSTPluginMain(audioMaster); }
#endif

} // extern "C"

#if WIN32
#include <windows.h>
void* hInstance;

extern void VST2_DLL_Load(void* hInst);
extern void VST2_DLL_Unload();

extern "C" {
    BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
    {
        hInstance = hInst;


        if (dwReason == DLL_PROCESS_ATTACH) {
            VST2_DLL_Load(hInstance);
        }

        if (dwReason == DLL_PROCESS_DETACH) {
            VST2_DLL_Unload();
        }

        return 1;
    }
} // extern "C"
#endif


VST2_PLUGIN* CreatePlugin( ) {
    VST2_PLUGIN* plugin = new PluginDistortion;
    if (plugin->Create()) return plugin;
    delete plugin;
    return nullptr;
}

PluginDistortion::PluginDistortion(){
}

PluginDistortion::~PluginDistortion() { 
}

bool            PluginDistortion::Create()  { 
    // load startup default presets
    LoadDefaultPresets( );

    // load def preset &  reset states to default
    SetPreset(0);

    return true; 
} 

void		PluginDistortion::PluginResetStates() {
    // проверяем какую тему используем
    SelectCurrentTheme();

    // сброс настроек для отображения пиков
    for ( u32 i = 0; i < 8; i++ ) {
        InputEnvelops[i].Reset(CurrentSampleRate, 1, 1000);
        OutputEnvelops[i].Reset(CurrentSampleRate, 1, 1000);

        UV_Out_Peak[i]          = false;
        UV_In_Peak[i]           = false;
        UV_In_Values[i]         = 0.0;
        UV_In_PeakValues[i]     = 0.0;
        UV_Out_Values[i]        = 0.0;
        UV_Out_PeakValues[i]    = 0.0;
    }

    // сброс настроек для компрессора
    CompressorPunch.SetAttack(Compressor_Attack);
    CompressorPunch.SetRelease(Compressor_Release);
    CompressorPunch.SetThreshold(Compressor_Trashold);
    CompressorPunch.SetRatio(Compressor_Ratio);
    CompressorPunch.SetSampleRate(CurrentSampleRate);
    CompressorPunch.SetCompressModeIsB(false);
    CompressorPunch.Reset();

    // сброс настроек для эффекта TiltEQ
    for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
        TiltEQ[i].SetSampleRate(CurrentSampleRate);
        TiltEQ[i].SetEQLevel(EQLevel);
    }

    // сброс настроек для эффекта Sample Rate Reducer
    for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
        RateReducer_Value[i]  = 0.0f;
        RateReducer_Value2[i] = 0;
    }

    // сброс настроек для отображающихся линий
    memset(&PoleLines_In[0],   0.0f, POLYLINEMETERCOUNT*sizeof(f32));
    memset(&PoleLines_Out[0],  0.0f, POLYLINEMETERCOUNT*sizeof(f32));
    memset(&PoleLines_Comp[0], 1.0f, POLYLINEMETERCOUNT*sizeof(f32));
    PoleLinesOffset = 0;
    PoleLinesSizePerOne = max(CurrentSampleRate /60.0f * 2.0f, 1.0); // кадры в сек !!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // сброс для плавных параметров
    InVolumeResult          = InVolume;
    OutVolumeResult         = OutVolume;
    DryWetLevelResult       = DryWetLevel;
    DriveLevelResult        = DriveLevel;
}

void            PluginDistortion::Suspend() { 
    PluginResetStates();
}

void            PluginDistortion::Resume() { 
    PluginResetStates();
}

void            PluginDistortion::SetOversampling(i32 Sampling) {
    //(1x 2x 4x 8x)
    //OversampleCount = Sampling;
    OversampleCount = 1;
}

void            PluginDistortion::SetSampleRate(f32 SampleRate) { 
    CurrentSampleRate       = SampleRate;
    
    // обновление частоты у компрессора
    CompressorPunch.SetSampleRate(CurrentSampleRate);

    // обновление частоты у TiltEQ
    for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
        TiltEQ[i].SetSampleRate(CurrentSampleRate);
    }

    // некоторые параметры зависят от семплрейта их следует обновить
    for (u32 i = 0; i < DEFAULT_PARAMS_COUNT; i++) SetParametr(i, Presets[CurrentPresetNumber].Params[i]);
}

void*           PluginDistortion::GetPresetDataCurrent() {
    return &Presets[CurrentPresetNumber];
}

bool            PluginDistortion::SetPresetDataCurrent(void* Buffer) {
    PluginPreset* LoadPresets = (PluginPreset*)Buffer;

    // check version
    if (LoadPresets[0].Version != VERSION) return false;

    // copy
    memcpy(&Presets[CurrentPresetNumber], LoadPresets, 1 * sizeof(PluginPreset));
    
    // update values
    for (u32 i = 0; i < DEFAULT_PARAMS_COUNT; i++) SetParametr(i, Presets[CurrentPresetNumber].Params[i]);

    // reset states to default
    PluginResetStates();
    
    return true;
}

void*           PluginDistortion::GetPresetDataArr() {
    return &Presets[0];
}

bool            PluginDistortion::SetPresetDataArr(void* Buffer) {
    PluginPreset* LoadPresets = (PluginPreset*)Buffer;

    // check version
    if (LoadPresets[0].Version != VERSION) return false;

    // copy
    memcpy(&Presets[0], LoadPresets, GetCountPresets() * sizeof(PluginPreset));

    // update values for cur prsete
    for (u32 i = 0; i < DEFAULT_PARAMS_COUNT; i++) SetParametr(i, Presets[CurrentPresetNumber].Params[i]);

    // reset states to default
    PluginResetStates();

    return true;
}

PStr            PluginDistortion::GetPresetNameBuffer(u32 Index) { 
    if (Index >= DEFAULT_PRESET_COUNT ) return &Presets[0].Name[0];
    return &Presets[Index].Name[0];
}

void            PluginDistortion::SetPreset(u32 Index) { 
    if (Index >= DEFAULT_PRESET_COUNT ) return ; 

    // copy from default
    CurrentPresetNumber = Index; 
    memcpy(&Presets[CurrentPresetNumber], &PresetsDefault[CurrentPresetNumber], 1 * sizeof(PluginPreset));
    CurrentPreset       = &Presets[CurrentPresetNumber];
    
    // set preset
    for ( u32 i = 0; i < DEFAULT_PARAMS_COUNT; i++ ) SetParametr(i, CurrentPreset->Params[i]); // обновить параметры ui
    
    // reset states to default
    PluginResetStates();
}

void            PluginDistortion::GetParameterLabel8(i32 index, utf8* Buffer8) { // дополнительная подпись параматра например db %
    vst_strncpy(Buffer8, "", kVstMaxParamStrLen); 
    switch ( index ) {
        case 0: vst_strncpy(Buffer8, "", kVstMaxParamStrLen); break;
        case 1: vst_strncpy(Buffer8, "db", kVstMaxParamStrLen); break;
        case 2: vst_strncpy(Buffer8, "db", kVstMaxParamStrLen); break;
        case 3: vst_strncpy(Buffer8, "%", kVstMaxParamStrLen); break;
        case 4: vst_strncpy(Buffer8, "", kVstMaxParamStrLen); break;
        case 5: vst_strncpy(Buffer8, "", kVstMaxParamStrLen);  break;
        case 6: vst_strncpy(Buffer8, "%", kVstMaxParamStrLen); break;
        case 7: vst_strncpy(Buffer8, "%", kVstMaxParamStrLen); break;

        case 8: vst_strncpy(Buffer8, "", kVstMaxParamStrLen); break;

        case 9: vst_strncpy(Buffer8, "ms", kVstMaxParamStrLen); break;
        case 10: vst_strncpy(Buffer8, "ms", kVstMaxParamStrLen); break;
        case 11: vst_strncpy(Buffer8, "db", kVstMaxParamStrLen); break;
        case 12: vst_strncpy(Buffer8, "", kVstMaxParamStrLen); break;
        case 13: vst_strncpy(Buffer8, "db", kVstMaxParamStrLen); break;
        case 14: vst_strncpy(Buffer8, "x", kVstMaxParamStrLen); break;

        case 15: vst_strncpy(Buffer8, "", kVstMaxParamStrLen); break;
        default: break;
    }
}

void            PluginDistortion::GetParametrDisplay8(i32 index, utf8* Buffer8) {  // цифровое отображение параметра
    switch ( index ) {
        case 0: vst_strncpy(Buffer8, NameTypesDist[TypeDistortion], kVstMaxParamStrLen);  
            break;
        case 1: {
            if (InVolume <= 0)  vst_strncpy (Buffer8, "-oo", kVstMaxParamStrLen);
            else                sprintf_s( Buffer8, kVstMaxParamStrLen, "%.1f", (f32)lin2dB(InVolume));
            break; 
        }
        case 2: {
            if (OutVolume <= 0) vst_strncpy (Buffer8, "-oo", kVstMaxParamStrLen);
            else                sprintf_s( Buffer8, kVstMaxParamStrLen, "%.1f", (f32)lin2dB(OutVolume));
            break;
        }
        case 3: AudioEffect::int2string((i32)round(DriveLevel*100.0), Buffer8, kVstMaxParamStrLen ); 
            break; 
        case 4: {
            if (BitLevel < 32)  AudioEffect::int2string((i32)round(BitLevel), Buffer8, kVstMaxParamStrLen);
            else                vst_strncpy(Buffer8, ">32", kVstMaxParamStrLen);
            break;
        }
        case 5: AudioEffect::int2string((i32)round(CurrentPreset->Params[5] * 100.0), Buffer8, kVstMaxParamStrLen );
            break;
        case 6: AudioEffect::int2string((i32)round(EQLevel*100.0), Buffer8, kVstMaxParamStrLen ); 
            break;
        case 7: AudioEffect::int2string((i32)round(DryWetLevel*100.0), Buffer8, kVstMaxParamStrLen ); 
            break;
        case 8: AudioEffect::int2string((i32)round(CurvenessLevel*100.0), Buffer8, kVstMaxParamStrLen );
            break;
        case 9: { 
			sprintf_s(Buffer8, kVstMaxParamStrLen, "%.1f", Compressor_Attack * 1000.0);
			break;
        }
        case 10: {
			sprintf_s(Buffer8, kVstMaxParamStrLen, "%.1f", Compressor_Release * 1000.0);
			break;
        }
        case 11: {
            if (InVolume <= 0)  vst_strncpy(Buffer8, "-oo", kVstMaxParamStrLen);
            else                sprintf_s(Buffer8, kVstMaxParamStrLen, "%.1f", (f32)lin2dB(Compressor_Trashold));
            break; 
        }
        case 12: {
            sprintf_s( Buffer8, kVstMaxParamStrLen, "%2.1f:1", (f32)Compressor_Ratio); 
            break;
        }
        case 13: {
            if (Compressor_Gain <= 0) vst_strncpy (Buffer8, "-oo", kVstMaxParamStrLen);
            else                sprintf_s( Buffer8, kVstMaxParamStrLen, "%.1f", (f32)lin2dB(Compressor_Gain));
            break;
        }
        case 14: {
            sprintf_s(Buffer8, kVstMaxParamStrLen, "%dx", OversampleCount);
            break;
        }
        case 15: {
            if (CurrentPreset->Params[15] == 0.0f)
                vst_strncpy(Buffer8, "pre", kVstMaxParamStrLen);
            else 
                vst_strncpy(Buffer8, "post", kVstMaxParamStrLen);
            break;
        }
        default: vst_strncpy(Buffer8, "", kVstMaxParamStrLen);    break;
    } 
}

void            PluginDistortion::GetParametrName8(i32 index, utf8* Buffer8) {  
    switch ( index ) {
        case 0: vst_strncpy(Buffer8, "Type", kVstMaxParamStrLen); break;
        case 1: vst_strncpy(Buffer8, "InVolume", kVstMaxParamStrLen); break;
        case 2: vst_strncpy(Buffer8, "OutVolume", kVstMaxParamStrLen); break;
        case 3: vst_strncpy(Buffer8, "Drive", kVstMaxParamStrLen); break;
        case 4: vst_strncpy(Buffer8, "BitLevel", kVstMaxParamStrLen); break;
        case 5: vst_strncpy(Buffer8, "Samples", kVstMaxParamStrLen);  break;
        case 6: vst_strncpy(Buffer8, "EQLevel", kVstMaxParamStrLen); break;
        case 7: vst_strncpy(Buffer8, "DryWet", kVstMaxParamStrLen); break;
        
        case 8: vst_strncpy(Buffer8, "Hardness", kVstMaxParamStrLen); break;
        //
        case 9: vst_strncpy(Buffer8, "Attack", kVstMaxParamStrLen); break;
        case 10: vst_strncpy(Buffer8, "Release", kVstMaxParamStrLen); break;
        case 11: vst_strncpy(Buffer8, "Trashold", kVstMaxParamStrLen);  break;  
        case 12: vst_strncpy(Buffer8, "Ratio", kVstMaxParamStrLen); break;
        case 13: vst_strncpy(Buffer8, "Gain", kVstMaxParamStrLen); break;
        case 14: vst_strncpy(Buffer8, "Oversmp", kVstMaxParamStrLen); break;
        case 15: vst_strncpy(Buffer8, "PrePost", kVstMaxParamStrLen); break;
        default: break;
    }
}

bool            PluginDistortion::GetParametrIsAutomated(i32 index) { 
    switch ( index ) {
        case 0: return false; // TypeDistortion !!!!!
        case 1: return true; // InVolume
        case 2: return true; // OutVolume
        case 3: return true; // DriveLevel
        case 4: return true; // BitLevel
        case 5: return true; // SampleRateLevel
        case 6: return true; // EQLevel
        case 7: return true; // DryWetLevel
        case 8: return true; // CurvenessLevel
        case 9: return false; // Compressor_Attack !!!!!
        case 10: return false; // Compressor_Release !!!!!
        case 11: return true; // Compressor_Trashold
        case 12: return false; // Compressor_Ratio !!!!!
        case 13: return true; // Compressor_Gain
        case 14: return false; // Oversampling !!!!!
        case 15: return false;// Compressor_Post !!!!!
        default: break;
    }
    return false; 
}

f32             PluginDistortion::GetParametr(i32 index) { 
    if (index >= DEFAULT_PARAMS_COUNT ) return 0.0; 
    return CurrentPreset->Params[index]; 
}

f32             PluginDistortion::ParametrToParams(i32 index, f32 ValueReal) {
    switch (index) {
    case 0: {  // TypeDistortion
        f32 Count1 = ARRAYSIZE(NameTypesDist) - 1;
        return LIN2PARAM(ValueReal, 0, Count1);
    } 
    case 1:     return ((ValueReal < -24.0f) ? (-24.0f) : ((ValueReal > 24.0f) ? (24.0f) : ValueReal)) / 24.0f * 0.5f + 0.5f; // InVolume
    case 2:     return ((ValueReal < -24.0f) ? (-24.0f) : ((ValueReal > 24.0f) ? (24.0f) : ValueReal)) / 24.0f * 0.5f + 0.5f; // OutVolume
    case 3:     return ValueReal; // DriveLevel
    case 4:     return LIN2PARAM(ValueReal, 1.0f, 32.0f); // 
    case 5:     return sqrt(sqrt(ValueReal)); // SampleRateLevel
    case 6:     return (ValueReal + 1.0f) * 0.5f;   // EQLevel
    case 7:     return ValueReal;                   // DryWetLevel
    case 8:     return LIN2PARAM(ValueReal, 0.0f, 1.0f);                        // CurvenessLevel
    case 9:     return LOG2PARAM(ValueReal, 0.001f * 0.1f, 0.001f * 500.0f);    // Compressor_Attack
    case 10:    return LOG2PARAM(ValueReal, 0.001f * 10.0f, 0.001f * 1000.0f);  // Compressor_Release
    case 11:    return LIN2PARAM(ValueReal, -60.0, 0.0);        // Compressor_Trashold
    case 12:    return LIN2PARAM(ValueReal, 1.0f, 10.0f);                       // Compressor_Ratio
    case 13:    return ((ValueReal < -24.0f) ? (-24.0f) : ((ValueReal > 24.0f) ? (24.0f) : ValueReal)) / 24.0f * 0.5f + 0.5f;                 // Compressor_Gain
    case 14:    return ValueReal;   // Oversampling
    case 15:    return (ValueReal >= 0.5f) ? 1.0f : 0.0f; // OldState_Pre OldState_Post
    default: break;
    }
    return 0.0f;
}

void            PluginDistortion::SetParametr(i32 index, f32 Value) { 
    if (index >= DEFAULT_PARAMS_COUNT ) return ;

    CurrentPreset->Params[index] = Value;
    
    switch ( index ) {
        case 0: {
            f32 Count1 = ARRAYSIZE(NameTypesDist) - 1;
            TypeDistortion  = (i32)PARAM2LIN(Value, 0, Count1);
            break;
        }
        case 1: { 
            InVolume = dB2lin((Value - 0.5f) * 2.0f * 24.0f);
            break; 
        }
        case 2:   {
            OutVolume = dB2lin((Value - 0.5f) * 2.0f * 24.0f);
            break; 
        }
        case 3: DriveLevel = Value;
            break; 
        case 4: BitLevel = PARAM2LIN(Value, 1.0f, 32.0f);
            break;
        case 5: {
            SampleRateLevel = pow(Value, 4.0);
            SampleRateLevel = max(SampleRateLevel * CurrentSampleRate, 1.0f);
            break;
        }
        case 6: {
            EQLevel = Value * 2.0f - 1.0f;
            for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
                TiltEQ[i].SetEQLevel(EQLevel);
            }
            break;
        }
        case 7: DryWetLevel    = Value;  
            break;
        case 8: CurvenessLevel = Value;
            break;
        case 9: 
            Compressor_Attack = PARAM2LOG(Value, 0.001f * 0.1f, 0.001f * 500.0f);
            CompressorPunch.SetAttack(Compressor_Attack);
            break; 
        case 10: 
            Compressor_Release = PARAM2LOG(Value, 0.001f * 10.0f, 0.001f * 1000.0f);
            CompressorPunch.SetRelease(Compressor_Release);
            break;
        case 11: 
            Compressor_Trashold = PARAM2LIN(Value, dB2lin(-60.0), dB2lin(0.0));
            CompressorPunch.SetThreshold(Compressor_Trashold);
            break;
        case 12: 
            Compressor_Ratio = PARAM2LIN(Value, 1.0f, 10.0f);
            CompressorPunch.SetRatio(Compressor_Ratio);
            break;
        case 13: 
            Compressor_Gain = dB2lin((Value - 0.5f) * 2.0f * 24.0f);
            CompressorPunch.SetGain(Compressor_Gain);
            break;
        case 14: {
            i32 valueToApply = pow(2, round(Value * 3.0f));  // (1x 2x 4x 8x)
            SetOversampling(valueToApply);
        }
        case 15: {
            if (Value == 0.0f) {
                OldState_Pre = true;
                OldState_Post = false;
                Compressor_Post = false;
            } else {
                OldState_Pre = false;
                OldState_Post = true;
                Compressor_Post = true;
            }
        }
        default: break;
    }
}
