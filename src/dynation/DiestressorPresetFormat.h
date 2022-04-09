/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once
#include "Header.h"

// все структуры с выравниваем в 1 байт!!!
#pragma pack(push, 1)
struct BASE_OPTIONS
{
	f32				InVolume;
	f32				OutVolume;
	f32				EQLevel;			// -1 - Full lowpass filter, +1 - Full highpass filter, 0 - without any filter
	f32				DryWetLevel;		// dry/wet для дисторшена
	f32				CompressorDryWet;	// dry/wet для компрессора
	f32				DitheringMix = 1.f;
	u32				DitheringType = 0;		// 0 - выключен, 1 - 16 бит, 2 - 20 бит, 3 - 24 бита
};

struct DISTORTION_OPTIONS
{
	f32				ClippingRatio;		// 0 - нету, 1 - заметный клиппинг, 100 - сильный клиппинг
	f32				DriveLevel;
	f32				BitLevel;
	f32				SampleRateLevel;
	f32				CurveLevel;
	i32             TypeDistortion;
};

struct COMPRESSOR_OPTIONS
{
	i32				CompressorRatio;
	f32				CompressorThreshold;
	f32				CompressorSlope;
	f32				CompressorLookAhead;	
	f32				CompressorWindowTime;
	f32				CompressorAttackTime;	
	f32				CompressorReleaseTime;	
	i32				CompressorType;
};

struct DIESTRESSOR_PRESET
{
	u32					PresetNumber;
	string128			UTF8PresetName;
	BASE_OPTIONS		BaseOptions;
	DISTORTION_OPTIONS	DistortionOptions;
	COMPRESSOR_OPTIONS	CompressorOptions;
};

struct DIESTRESSOR_PRESET_NODE;
struct DIESTRESSOR_PRESET_NODE
{
	DIESTRESSOR_PRESET*			pThis;
	DIESTRESSOR_PRESET_NODE*	pPrev;
	DIESTRESSOR_PRESET_NODE*	pNext;
};

struct DIESTRESSOR_PRESETGROUP
{
	u32							GroupNumber;
	u32							CountOfPresets;
	string128					UTF8GroupName;
	DIESTRESSOR_PRESET_NODE*	pPresetNode;
};

struct DIESTRESSOR_PRESETSLIST
{
	u32							MagicWord;			// 'DIES' - Dynation
	u32							PresetVersion;		// 1.0
	u32							CountOfGroups;
	void*						pData;
};
#pragma pack(pop)

class DiestressorPresetLoader
{
public:
	void LoadPreset(char* PathToPreset);
	void SetPreset(i32 PresetNumber, PluginDistortion* pDistortion);

private:
	void* BaseOptions;
};
