/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "dynation.h"
#include "dynation_dsp_bitcrush.h"

void
DynationPlugin::InitBitcrusher()
{
    ResetCompression();
}

void
DynationPlugin::ResetBitcrusher()
{
    ProcessState.Shifters = {};
    BitcrusherReset(State->State()->Bitshifter, ProcessState.Shifters.BitcrusherPowerValue);
}

void
DynationPlugin::ProcessBitcrusher(AudioProcessingBlock<float>& ProcessingBlock)
{

}
