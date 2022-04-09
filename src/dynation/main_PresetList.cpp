/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "Header.h"

void            PluginDistortion::LoadDefaultPresets( ) { 
	{
		auto& data = PresetsDefault[0];
		vst_strncpy(data.Name, "Init", 64);
		data.Version = VERSION;
		data.Params[0] = ParametrToParams(0, 0); // TypeDistortion
		data.Params[1] = ParametrToParams(1, 0); // InVolume
		data.Params[2] = ParametrToParams(2, 0); // OutVolume
		data.Params[3] = ParametrToParams(3, 0.00); // DriveLevel
		data.Params[4] = ParametrToParams(4, 32); // BitLevel
		data.Params[5] = ParametrToParams(5, 1.00); // SampleRateLevel
		data.Params[6] = ParametrToParams(6, 0.00); // EQLevel
		data.Params[7] = ParametrToParams(7, 1.00); // DryWetLevel
		data.Params[8] = ParametrToParams(8, 0.00); // CurvenessLevel
		data.Params[9] = ParametrToParams(9, 0.001f * 0.1f); // Compressor_Attack
		data.Params[10] = ParametrToParams(10, 0.001f * 10.0f); // Compressor_Release
		data.Params[11] = ParametrToParams(11, 0.0); // Compressor_Trashold
		data.Params[12] = ParametrToParams(12, 1.0); // Compressor_Ratio
		data.Params[13] = ParametrToParams(13, 0.0); // Compressor_Gain
		data.Params[14] = ParametrToParams(14, 0); // Oversampling
		data.Params[15] = ParametrToParams(15, 0); // Compressor_Post
	}

	{
		auto& data = PresetsDefault[1];
		vst_strncpy(data.Name, "BASS Gentle FatSub", 64);
		data.Version = VERSION;
		data.Params[0] = 0.0f;
		data.Params[1] = 0.5f;
		data.Params[2] = 0.5f;
		data.Params[3] = 0.0f;
		data.Params[4] = 1.0f;
		data.Params[5] = 1.0f;
		data.Params[6] = 0.423333406f;
		data.Params[7] = 1.0f;
		data.Params[8] = 1.0f;
		data.Params[9] = 0.223333344;
		data.Params[10] = 0.07100000023;
		data.Params[11] = 0.0633335545;
		data.Params[12] = 0.0600000545;
		data.Params[13] = 0.506666660;
		data.Params[14] = 1.0f;
		data.Params[15] = 0.0f;
	}

	{
		auto& data = PresetsDefault[2];
		vst_strncpy(data.Name, "BASS Heavy FatSub", 64);
		data.Version = VERSION;
		data.Params[0] = 0.0f;
		data.Params[1] = 0.5f;
		data.Params[2] = 0.507000029f;
		data.Params[3] = 0.0f;
		data.Params[4] = 1.0f;
		data.Params[5] = 1.0f;
		data.Params[6] = 0.436666727f;
		data.Params[7] = 1.0f;
		data.Params[8] = 1.0f;
		data.Params[9] = 0.223333344;
		data.Params[10] = 0.07100000023;
		data.Params[11] = 0.0633335545;
		data.Params[12] = 0.0600000545;
		data.Params[13] = 0.519999981;
		data.Params[14] = 1.0f;
		data.Params[15] = 0.0f;
	}

	{
		auto& data = PresetsDefault[3];
		vst_strncpy(data.Name, "DRM Cymbal Fattener", 64);
		data.Version = VERSION;
		data.Params[0] = 0.0f;
		data.Params[1] = 0.510000229f;
		data.Params[2] = 0.5f;
		data.Params[3] = 1.0f;
		data.Params[4] = 1.0f;
		data.Params[5] = 1.0f;
		data.Params[6] = 0.5f;
		data.Params[7] = 1.0f;
		data.Params[8] = 0.0f;
		data.Params[9] = 0.169999942;
		data.Params[10] = 0.0900000110;
		data.Params[11] = 0.0733335018;
		data.Params[12] = 0.896666646;
		data.Params[13] = 0.533333302;
		data.Params[14] = 0.5f;
		data.Params[15] = 0.0f;
	}

	{
		auto& data = PresetsDefault[4];
		vst_strncpy(data.Name, "DRM Smack That Drum", 64);
		data.Version = VERSION;
		data.Params[0] = 0.0f;
		data.Params[1] = 0.5f;
		data.Params[2] = 0.506666660f;
		data.Params[3] = 0.5066666850f;
		data.Params[4] = 1.0f;
		data.Params[5] = 1.0f;
		data.Params[6] = 0.5f;
		data.Params[7] = 1.0f;
		data.Params[8] = 1.0f;
		data.Params[9] = 0.320000023;
		data.Params[10] = 0.0710000023;
		data.Params[11] = 0.0633335635;
		data.Params[12] = 0.253333330;
		data.Params[13] = 0.539999962;
		data.Params[14] = 1.0f;
		data.Params[15] = 0.0f;
	}

	{
		auto& data = PresetsDefault[5];
		vst_strncpy(data.Name, "VOX Push n Pull", 64);
		data.Version = VERSION;
		data.Params[0] = 0.0f;
		data.Params[1] = 0.5f;
		data.Params[2] = 0.506666660f;
		data.Params[3] = 0.0700001493;
		data.Params[4] = 1.0f;
		data.Params[5] = 1.0f;
		data.Params[6] = 0.5f;
		data.Params[7] = 1.0f;
		data.Params[8] = 1.0f;
		data.Params[9] = 0.170000061;
		data.Params[10] = 0.107666649;
		data.Params[11] = 0.0633335635;
		data.Params[12] = 0.776666462;
		data.Params[13] = 0.599999905;
		data.Params[14] = 1.0f;
		data.Params[15] = 0.0f;
	}

	// copy to used presets
	memcpy(&Presets[0], &PresetsDefault[0], DEFAULT_PRESET_COUNT * sizeof(PluginPreset));
}
