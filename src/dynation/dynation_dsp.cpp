/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "dynation.h"

void
DynationPlugin::Process(AudioProcessingBlock<float>& ProcessingBlock)
{
    State->Lock();
    // #TODO: Is it safe? I really don't sure, because inside this structure
    // we've RAII classes (but I'm not sure. If it's safe to use memcpy - 
    // i'd use it).
    std::memcpy(&TempState, State->State(), SizeOfState);
    State->Unlock();

    for (size_t i = 0; i < OutputChannels; i++) {
        std::memcpy(ProcessingBlock.Outputs[i], ProcessingBlock.Inputs[i], ProcessingBlock.FramesToProcess * sizeof(float));
    }

    ProcessDistortion(ProcessingBlock);
}

void
DynationPlugin::ProcessDouble(AudioProcessingBlock<double>& ProcessingBlock)
{

}
