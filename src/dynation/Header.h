#pragma once

#include "VST2_Header.h"
#include <cmath>

// linear -> dB conversion
inline double lin2dB(double lin)
{
	return log(lin) * 8.6858896380650365530225783783321;		// 20 / ln( 10 )
}

// dB -> linear conversion
inline double dB2lin(double dB)
{
	return exp(dB * 0.11512925464970228420089957273422);		// ln( 10 ) / 20
}

#define PARAM2LOG(x, mn, mx) (exp(log(mn) + (x) * (log(mx) - log(mn))))
#define LOG2PARAM(x, mn, mx) ((log(x) - log(mn)) / (log(mx) - log(mn)))

#define PARAM2LIN(x, mn, mx) ((mn) + (x) * ((mx) - (mn)))
#define LIN2PARAM(x, mn, mx) (((x) - (mn)) / ((mx) - (mn)))

#define ARRAYSIZE(ar) (sizeof(ar)/sizeof(ar[0]))

//inline double fastatan(double x)
//{
//	return (x / (1.0 + 0.28 * (x * x)));
//}

#define sign(x)  (signbit(x) ?  -1 : 1)

#include "CompAlg.h"
#include "SatAlg.h"
#include "TempBuffers.h"
#include "TiltEQAlg.h"
#include "EnvelopeDetectorAlg.h"

#include "CompressorA.h"

#define CREATEVERSION(HI_, LO_) (LO_ + HI_*1000) 
#define VERSION                 CREATEVERSION(1, 3) // 1.3

#define DEFAULT_PRESET_COUNT 6 // default preset
#define DEFAULT_PARAMS_COUNT 16 // tilteq, drywet, inlevel, outlevel, character, type, samplerate, bitrate, curv, AT, REL, TRASH, RAT, LOOKA, oversample, prepost

extern const char* NameTypesDist[6];

struct PluginPreset {
    string64            Name;
    u32                 Version;
    f32                 Params[DEFAULT_PARAMS_COUNT]; // cприведеные параметры к 0.0f-1.0f
    u32                 UITheme; // 0-light 1-black
};

class PluginDistortion : public VST2_PLUGIN {
public:
	PluginDistortion();
	~PluginDistortion();

	bool            Create() override;

	bool            PluginIsUI()            override { return true; }
	bool            PluginIsSynth()         override { return false; }
	bool            PluginIsEventSupport()  override { return true; }
	bool            PluginIsDoubleSupport() override { return false; }

	u32             GetCountPresets()       override { return DEFAULT_PRESET_COUNT; }
	u32             GetCountParams()        override { return DEFAULT_PARAMS_COUNT; }
	i32             GetUniqueID()           override { return 'Sfla'; }
	u32             GetInputCount()         override
	{
		// Stereo input
		return (Plugin_Base->InVstChannels ? Plugin_Base->InVstChannels : 2);
	}
	u32             GetOutputCount()        override
	{
		// Stereo output
		return (Plugin_Base->OutVstChannels ? Plugin_Base->OutVstChannels : 2);
	}

	PConstStr       GetName()               override { return "Dynation"; }
	PConstStr       GetVendorName()         override { return "Anton Kovalev"; }
	PConstStr       GetProductName()        override { return "Dynation"; }
	i32             GetVersion()            override { return VERSION; }

	void            Suspend()               override;
	void            Resume()                override;

	void            SetSampleRate(f32 SampleRate)                                   override;
	void            SetOversampling(i32 Sampling)                                   override;

	u32             GetPresetSize()                                                 override { return sizeof(PluginPreset); }
	void*			GetPresetDataCurrent()											override;
	bool            SetPresetDataCurrent(void* Buffer)								override;
	void*			GetPresetDataArr()												override;
	bool            SetPresetDataArr(void* Buffer)									override;
	void            SetPreset(u32 Index)                                            override;
	PStr            GetPresetNameBuffer(u32 Index)                                  override;

	void            GetParameterLabel8(i32 index, utf8* Buffer8)                    override;
	void            GetParametrDisplay8(i32 index, utf8* Buffer8)                   override;
	void            GetParametrName8(i32 index, utf8* Buffer8)                      override;
	bool            GetParametrIsAutomated(i32 index)                               override;
	void            SetParametr(i32 index, f32 Value)                               override;
	f32             GetParametr(i32 index)                                          override;

	void            Process(PROCESS_DATA* data)                                     override;
	void            ProcessDouble(PROCESS_DATA_DOUBLE* data)                        override;

	i32				GetPluginDelay();
	void			SetPluginDelay(i32 DelayTime);

	// ui
	i32             GetWidthDefault()       override;
	i32             GetHeightDefault()      override;
	i32             GetMaxWidthDefault()    override;
	i32             GetMaxHeightDefault()   override;
	PConstStr       GetWindowName()         override;
	bool            CreateUI()              override;
	void            DestroyUI()             override;
	void            DrawUI()                override;
public:
	//
	PluginPreset		PresetsDefault[DEFAULT_PRESET_COUNT];
	PluginPreset		Presets[DEFAULT_PRESET_COUNT];
	PluginPreset*		CurrentPreset;
	u32					CurrentPresetNumber				= 0;
	
	//
	f32					CurrentSampleRate				= 44100.0f;
	i32					CurrentDelay					= 0;
	i32					OversampleCount					= 1;

	// transformated preset data
	bool				ByPass = false;

	i32					TypeDistortion;
	f32					InVolume;
	f32					OutVolume;
	f32					DriveLevel;
	f32					BitLevel;
	f32					SampleRateLevel;
	f32					EQLevel; // -1 - Full lowpass filter, +1 - Full highpass filter, 0 - without any filter
	f32					DryWetLevel; // dry/wet функции process
	f32					CurvenessLevel;		//
	f32					Compressor_Attack;
	f32					Compressor_Release;
	f32					Compressor_Trashold;
	f32					Compressor_Ratio;
	f32					Compressor_Gain;
	bool				Compressor_Post;

	// smool data for automated
	f32					InVolumeResult = 1.0f;
	f32					OutVolumeResult = 1.0f;
	f32					DryWetLevelResult = 1.0f;
	f32					DriveLevelResult = 0.0f;
	
	//
	PROCESS_DATA_BUFFER	BufferA;
	PROCESS_DATA_BUFFER	BufferC;
	
	//
	TiltEQAlg_Simple	TiltEQ[DATAMAXCHANNELS];
	
	//
	f32					RateReducer_Value[DATAMAXCHANNELS]  = { 0.0f };
	u32					RateReducer_Value2[DATAMAXCHANNELS] = { 0 };
	
	// main compressor
	CompressorPunchF32	CompressorPunch;
	
	// visible envilops
    EnvelopeDetector    InputEnvelops[8];
    EnvelopeDetector    OutputEnvelops[8];
public:
	void				PluginResetStates();
	f32					ParametrToParams(i32 index, f32 ValueReal);
    void				LoadDefaultPresets();

private: // UI
    f32             NewUIScale = 1.5/4.0; // 1.5/3.0;
    
    ImVec2          ResizeStartPos;
    
    ImColor         Color_BG;// фон всего
    ImColor         Color_SettingBG; // фон нижней панельки
    ImColor         Color_BodyBG;// фон основной
    ImColor         Color_BodyZoneBG; // фон под выделеными учасками

    ImColor         Color_ButtonResize; // цвета кнопки ресайза
    ImColor         Color_ButtonResize_Hovered;
    ImColor         Color_ButtonResize_Dawn;

    ImColor         Color_Crutilka;
    ImColor         Color_CrutilkaBG;
    ImColor         Color_CrutilkaPoint;

    ImColor         Color_Text1 ;// def color in imgui
    ImColor         Color_Text2 ;// цветная надпись
    ImColor         Color_Text3 ;// у крутилок
    ImColor         Color_Text4 ;// hovered

    ImColor         Color_Monitor_Line1 ;// 
    ImColor         Color_Monitor_Line2 ;// 
    ImColor         Color_Monitor_Line3 ;// 
    ImColor         Color_Monitor_BGLine1 ;// 
    ImColor         Color_Monitor_BGLine2 ;// 

    ImColor         Color_UV_Color1 ;// 
    ImColor         Color_UV_Color2 ;// 
    ImColor         Color_UV_Color3 ;// 
    ImColor         Color_UV_Color4 ;// 
    ImColor         Color_UV_Color5 ;// 
    ImColor         Color_UV_Color6 ;// 
    //
    #define POLYLINEMETERCOUNT  (60)

    f32             PoleLines_In[POLYLINEMETERCOUNT];
    f32             PoleLines_Out[POLYLINEMETERCOUNT];
    f32             PoleLines_Comp[POLYLINEMETERCOUNT];
    ImVec2          PoleLines_In_Vec[POLYLINEMETERCOUNT];
    ImVec2          PoleLines_Out_Vec[POLYLINEMETERCOUNT];
    ImVec2          PoleLines_Comp_Vec[POLYLINEMETERCOUNT];
    u32             PoleLinesOffset;
    u32             PoleLinesSizePerOne;

    void            Metter_Update(f32 LinearIn, f32 LinearOut, f32 LinearZip);
    void            Metter_Draw(f32 X, f32 Y, f32 W, f32 H);
    //
    u32             UV_In_Count = 2;
    f32             UV_In_Values[8];
    f32             UV_In_PeakValues[8];
    bool            UV_In_Peak[8];
    //
    u32             UV_Out_Count = 2;
    f32             UV_Out_Values[8];
    f32             UV_Out_PeakValues[8];
    bool            UV_Out_Peak[8];
    //
    bool            OldState_Drive = false;
    bool            OldState_In = false;
    bool            OldState_Out = false;
    bool            OldState_DryWet = false;
    bool            OldState_TiltEq = false;
    bool            OldState_Attack = false;
    bool            OldState_Release = false;
    bool            OldState_Trashold = false;
    bool            OldState_Ratio = false;
    bool            OldState_Lookahead = false;
    bool            OldState_SampleRate = false;
	bool            OldState_BitRate = false;
	bool            OldState_Curv = false;
	//
	bool            OldState_Pre = true;
	bool            OldState_Post = false;
	//
	bool			tiltChanged = false;
	bool			inChanged = false;
	bool			outChanged = false;
	bool			drywetChanged = false;
	bool			driveChanged = false;
	bool			attackChanged = false;
	bool			releaseChanged = false;
	bool			thresholdChanged = false;
	bool			ratioChanged = false;
	bool			gainChanged = false;

	bool			samplerateChanged = false;
	bool			samplebitChanged = false;
	bool			hardnessChanged = false;
    //
    bool            DrawSettings = false;
	//
	bool			isDark = true;
    //
    void            SelectTheme_Light();
    void            SelectTheme_Black();

	void			SelectCurrentTheme() {
		if (isDark) {
			SelectTheme_Black();
		} else {
			SelectTheme_Light();
		}
	}

	// status bar string
	void SetNewStatus(const char* valueName, const char* valueText) {
		memset(PrintString, 0, sizeof(PrintString));
		strcat_s(PrintString, valueName);
		strcat_s(PrintString, ": ");
		strcat_s(PrintString, valueText);
		TimeoutIdle = GetDeltaTime();
	}

	size_t TimeoutPeriod = 3000;
	bool isTimoutActive = false;
	string128 PrintString = {};
};
