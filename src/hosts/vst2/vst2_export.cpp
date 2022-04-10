/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "base.h"
#include "vst2_host.h"

extern "C" 
{

PLUGIN_EXPORT 
AEffect* 
VSTPluginMain(audioMasterCallback audioMaster)
{
	if (!audioMaster(0, audioMasterVersion, 0, 0, 0, 0)) {
		return 0;
	}

	BasePlugin* Plugin = nullptr;
	BaseView* View = nullptr;
	if (!CreatePluginInterface(Plugin, View)) {
		return 0;
	}

	AudioEffect* effect = new VST2Host(audioMaster, Plugin, View);
	if (effect == nullptr) {
		return 0;
	}

	return effect->getAeffect();
}

PLUGIN_EXPORT
AEffect*
MAIN(audioMasterCallback audioMaster)
{
	return VSTPluginMain(audioMaster);
}
	
}