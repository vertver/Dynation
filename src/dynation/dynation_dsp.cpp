/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "dynation.h"
#include "dynation_dsp_distortions.h"
#include "dynation_dsp_bitcrush.h"

void
DynationPlugin::InitBitcrusher()
{
    ResetCompression();
}

void 
DynationPlugin::InitDistortion()
{
    ResetDistortion();
}

void 
DynationPlugin::InitCompression()
{
    auto& Compressor = ProcessState.Compressor;
    Compressor.RMSBufferChannels = 1;

    ResetCompression();
}

void
DynationPlugin::ResetBitcrusher()
{
    ProcessState.Shifters = {};
    BitcrusherReset(State->State()->Bitshifter, ProcessState.Shifters.BitcrusherPowerValue);
}

void
DynationPlugin::ResetDistortion()
{
    ProcessState.Distort = {};

    switch (State->State()->DistortType)
    {
        case DistortionType::NoneType:
            break;
        case DistortionType::TubeDistortion:
            TubeDistortionReset(State->State()->Drive, ProcessState.Distort.Alpha, ProcessState.Distort.Beta);
            break;
        case DistortionType::Sincrusher:
            break;
        case DistortionType::HardClip:
            break;
        case DistortionType::FBack:
            break;
        case DistortionType::SoftClip:
            break;
    default:
        break;
    }
}

void
DynationPlugin::ResetCompression()
{
    auto& Compressor = ProcessState.Compressor;

	// Clear buffers     
    int32_t CountABMode = Compressor.CompressModeB ? 256 : 16;
    Compressor.RMSBufferSize1 = std::max(2, 16 * Samplerate / 44100);
    Compressor.RMSBufferSize2 = std::max(2, CountABMode * Samplerate / 44100);
    Compressor.RMSBuffer1.Reset(Compressor.RMSBufferChannels, Compressor.RMSBufferSize1);
    Compressor.RMSBuffer2.Reset(Compressor.RMSBufferChannels, Compressor.RMSBufferSize1);
    Compressor.RMSBufferRMS.Reset(Compressor.RMSBufferChannels, 2);

	// DC Fix reset
	Compressor.Envelope = FLT_EPSILON;
	Compressor.Envelope2 = FLT_EPSILON;

    Compressor.RMSBufferOffset1 = 0;
    Compressor.RMSBufferOffset2 = 0;
}

void 
DynationPlugin::ProcessBitcrusher(AudioProcessingBlock<float>& ProcessingBlock)
{

}

void 
DynationPlugin::ProcessDistortion(AudioProcessingBlock<float>& ProcessingBlock)
{

}

void 
DynationPlugin::ProcessCompression(AudioProcessingBlock<float>& ProcessingBlock)
{
	auto& Compressor = ProcessState.Compressor;

    auto CompressSample = [&Compressor](auto Input, bool isSecondType) -> auto
    {
        double InRmsDB = lin2dB(Input + FLT_EPSILON);

        double OverDB = InRmsDB - Compressor.Threshold_db;
        if (OverDB < 0.0) OverDB = 0.0;
        OverDB += FLT_EPSILON;

        auto& Envelope = isSecondType ? Compressor.Envelope2 : Compressor.Envelope;
        double Theta = (OverDB > Envelope) ? Compressor.Attack_ : Compressor.Release_;
        Envelope = OverDB + Theta * (Envelope - OverDB);

        double Pvart = Envelope - FLT_EPSILON;
        if (Pvart > 0.0f) {
            Pvart -= Envelope * Envelope * 0.001f; // opto pseudo curve
        }

        double Gain = 0.0f - Pvart * Compressor.Ratio_;
        return dB2lin(Gain);
    };

	auto BufferSample = [&Compressor](auto* Input)
	{
        for (int32_t ch = 0; ch < Compressor.RMSBufferChannels; ch++) {
            auto In_pow2 = Input[ch];
            In_pow2 = In_pow2 * In_pow2;

            {
                auto& RMSSumm = Compressor.RMSBufferRMS[ch][0];
                auto RMS1 = &Compressor.RMSBuffer1[ch][0];

                if (Compressor.RMSBufferOffset1 >= Compressor.RMSBufferSize1)
                    Compressor.RMSBufferOffset1 = 0;

                auto PrevSample = RMS1[Compressor.RMSBufferOffset1];
                RMS1[Compressor.RMSBufferOffset1] = In_pow2;
                Compressor.RMSBufferOffset1++;

                RMSSumm += In_pow2;
                RMSSumm -= PrevSample;
            }

            {
                auto& RMSSumm = Compressor.RMSBufferRMS[ch][1];
                auto  RMS2 = &Compressor.RMSBuffer2[ch][0];

                if (Compressor.RMSBufferOffset2 >= Compressor.RMSBufferSize2)
                    Compressor.RMSBufferOffset2 = 0;

                auto PrevSample = RMS2[Compressor.RMSBufferOffset2];
                RMS2[Compressor.RMSBufferOffset2] = In_pow2;
                Compressor.RMSBufferOffset2++;

                RMSSumm += In_pow2;
                RMSSumm -= PrevSample;
            }
        }
	};
}

void
DynationPlugin::Process(AudioProcessingBlock<float>& ProcessingBlock)
{


}

void
DynationPlugin::ProcessDouble(AudioProcessingBlock<double>& ProcessingBlock)
{

}
