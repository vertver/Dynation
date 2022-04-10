/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "dynation.h"

void
DynationPlugin::Process(AudioProcessingBlock<float>& ProcessingBlock)
{
	for (size_t i = 0; i < OutputChannels; i++) {
		memcpy(ProcessingBlock.Outputs[i], ProcessingBlock.Inputs[i], ProcessingBlock.FramesToProcess * sizeof(float));
	}
}

void
DynationPlugin::ProcessDouble(AudioProcessingBlock<double>& ProcessingBlock)
{

}
