#pragma once

#include  <vector>

#define COMPRESSORPUNCHDC FLT_EPSILON   

class CompressorPunchF32 {
public:
	CompressorPunchF32() {
        Buffers_Init();
        SetAttack(0.0f);
        SetRelease(0.0f);
        SetThreshold(1.0f);
        SetRatio(1.0f);
	}

	~CompressorPunchF32() {
        Buffers_Free();
	}

    void        Reset() {
        Buffers_Reset();
        CompReset();
    }

    f32         ProcessSample(f32* In, f32* OutScale) {
        RMSBufferSample(In);

        f32 ARGRMS = 0.0f;
        for (u32 ch = 0; ch < RMSBufferChannels; ch++) {
            f32 RmsIn1 = (RMSBufferRMS[ch][0] <= COMPRESSORPUNCHDC) ? 0.0f : (f32)(sqrt(RMSBufferRMS[ch][0] / (f64)RMSBufferSize1));
            f32 RmsIn2 = (RMSBufferRMS[ch][1] <= COMPRESSORPUNCHDC) ? 0.0f : (f32)(sqrt(RMSBufferRMS[ch][1] / (f64)RMSBufferSize2));

            RmsIn1 = (RmsIn1 + RmsIn2) * 0.5f;

            f32 Scale1 = CompSample(RmsIn1);
            f32 Scale2 = CompSample2(RmsIn2);

            OutScale[ch] = (Scale1 + Scale2) * 0.5f * Gate_;

            ARGRMS += RmsIn1;
        }

        return ARGRMS / (f32)RMSBufferChannels;
    }
public:
    void		SetAttack(f32 Attack) {
        Attack_sec  = Attack;
        Attack_     = (Attack_sec == 0.0f) ? 0.0 : (f32)exp(-1.0 / (SampleRate * Attack_sec));
    }

    f32			GetAttack() { 
        return Attack_sec;
    }

    void		SetRelease(f32 Release) { 
        Release_sec = Release;
        Release_    = (Release_sec == 0.0f) ? 0.0 : (f32)exp(-1.0 / (SampleRate * Release_sec));
    }

    f32			GetRelease() { 
        return Release_sec;
    }

    void		SetThreshold(f32 Threshold) {
        Threshold_      = Threshold;
        Threshold_db    = lin2dB(Threshold_);
    }

    f32			GetThreshold() { 
        return Threshold_;
    };

    void		SetRatio(f32 Ratio) {
        Ratio_orig  = Ratio;
        Ratio_      = (1.0f - 1.0f / (Ratio_orig));
    };

    f32			GetRatio() { 
        return Ratio_orig; 
    };

    void		SetLookahead(f32 fLookahead) {  
        // not used
    }

    f32			GetLookahead() { 
        return 0.0f;  // not used
    };

    void		SetSlope(f32 fSlope) {  
        // not used
    }

    f32			GetSlope() { 
        return 0.0f;   // not used
    };

    void		SetGain(f32 Gain) {
        Gate_ = Gain;
    }

    f32			GetGain() {
        return Gate_;
    };

    void        SetChannels(u32 Channels) {
        if (RMSBufferChannels == Channels) return;
        RMSBufferChannels = Channels;
        Buffers_UpdateChannels();
        Buffers_Update();
        Buffers_Reset();
    }
    u32			GetChannels() {
        return RMSBufferChannels;
    };

    void		SetSampleRate(f32 fSampleRate) {
        if (SampleRate == fSampleRate) return;
        SampleRate = fSampleRate;
        SetAttack(Attack_sec);
        SetRelease(Release_sec);
        Buffers_Update();
        Buffers_Reset();
    }

    f32			GetSampleRate() {
        return SampleRate;
    };

    void		SetCompressModeIsB(bool ModeIsB) {
        if (CompressModeB == ModeIsB) return;
        CompressModeB = ModeIsB;
        Buffers_Update();
        Buffers_Reset();
    }

    bool	    GetCompressModeIsB() {
        return CompressModeB;
    };
private:
    f32 Gate_           = 1.0f;
    f32 Threshold_      = 1.0f;
    f32 Ratio_orig      = 1.0f;
    f32 Attack_sec      = 0.0f;
    f32 Release_sec     = 0.0f;
    f32 SampleRate      = 0.0f;
    bool CompressModeB  = true; //  default B mode
private:
    f32 Envelope        = COMPRESSORPUNCHDC;
    f32 Envelope2       = COMPRESSORPUNCHDC;
    f32 Threshold_db    = 0.0f;
    f32 Ratio_          = 1.0f;
    f32 Attack_         = 0.0f;
    f32 Release_        = 0.0f;
private:
    void        CompReset() {
        Envelope = COMPRESSORPUNCHDC;
        Envelope2 = COMPRESSORPUNCHDC;
    }

    f32         CompSample(f32 InRms) { // return output scale
        f32 InRmsDB = lin2dB(InRms + COMPRESSORPUNCHDC);

        f32 OverDB = InRmsDB - Threshold_db;
        if (OverDB < 0.0) OverDB = 0.0;
        OverDB += COMPRESSORPUNCHDC;

        f32 Theta = (OverDB > Envelope) ? Attack_ : Release_;
        Envelope = OverDB + Theta * (Envelope - OverDB);

        f32 Pvart = Envelope - COMPRESSORPUNCHDC;
        if (Pvart > 0.0f) Pvart -= Envelope * Envelope * 0.001f; // opto pseudo curve
        f32 Gain = 0.0f - Pvart * Ratio_;

        return dB2lin(Gain);
    }

    f32         CompSample2(f32 InRms) { // return output scale
        f32 InRmsDB = lin2dB(InRms + COMPRESSORPUNCHDC);

        f32 OverDB = InRmsDB - Threshold_db;
        if (OverDB < 0.0) OverDB = 0.0;
        OverDB += COMPRESSORPUNCHDC;

        f32 Theta = (OverDB > Envelope2) ? Attack_ : Release_;
        Envelope2 = OverDB + Theta * (Envelope2 - OverDB);

        f32 Pvart = Envelope2 - COMPRESSORPUNCHDC;
        if (Pvart > 0.0f) Pvart -= Envelope2 * Envelope2 * 0.001f; // opto pseudo curve
        f32 Gain = 0.0f - Pvart * Ratio_;

        return dB2lin(Gain);
    }
private:
    std::vector<std::vector<f64>>   RMSBuffer1;
    std::vector<std::vector<f64>>   RMSBuffer2;
    std::vector<std::vector<f64>>   RMSBufferRMS;
    u32                             RMSBufferChannels   = 0;
    u32                             RMSBufferSize1      = 0;
    u32                             RMSBufferSize2      = 0;
    u32                             RMSBufferOffset1    = 0;
    u32                             RMSBufferOffset2    = 0;
private:
    void        RMSBufferSample(f32* In) {
        for (u32 ch = 0; ch < RMSBufferChannels; ch++) {
            
            f64 In_pow2     = In[ch];
            In_pow2         = In_pow2 * In_pow2;

            {
                auto& RMSSumm   = RMSBufferRMS[ch][0];
                auto RMS1       = &RMSBuffer1[ch][0];

                if (RMSBufferOffset1 >= RMSBufferSize1) 
                    RMSBufferOffset1 = 0;
                
                f32 PrevSample = RMS1[RMSBufferOffset1];
                RMS1[RMSBufferOffset1] = In_pow2;
                RMSBufferOffset1++;

                RMSSumm     += In_pow2;
                RMSSumm     -= PrevSample;
            }
            {
                auto& RMSSumm   = RMSBufferRMS[ch][1];
                auto  RMS2      = &RMSBuffer2[ch][0];

                if (RMSBufferOffset2 >= RMSBufferSize2) 
                    RMSBufferOffset2 = 0;

                f64 PrevSample = RMS2[RMSBufferOffset2];
                RMS2[RMSBufferOffset2] = In_pow2;
                RMSBufferOffset2++;

                RMSSumm     += In_pow2;
                RMSSumm     -= PrevSample;
            }
        }
    }
private:
    void        Buffers_Init() {
        // reserve
        RMSBufferRMS.reserve(8);
        RMSBuffer1.reserve(8);
        RMSBuffer2.reserve(8);

        // resize for default mono 44.1khz
        RMSBufferChannels   = 1;
        SampleRate          = 44100.0f;

        Buffers_UpdateChannels();
        Buffers_Update();
        Buffers_Reset();
    }

    void        Buffers_Free() {
        Buffers_Reset();

        for (u32 i = 0; i < RMSBufferChannels; i++) {
            RMSBufferRMS[i].clear();
            RMSBuffer1[i].clear();
            RMSBuffer2[i].clear();
        }

        RMSBuffer1.clear();
        RMSBuffer2.clear();
        RMSBufferRMS.clear();

        RMSBufferChannels = 0;
    }

    void        Buffers_Reset() {
        for (u32 i = 0; i < RMSBufferChannels; i++) {
            memset(&RMSBufferRMS[i][0], 0, sizeof(f64) * RMSBufferRMS[i].size());
            memset(&RMSBuffer1[i][0], 0, sizeof(f64) * RMSBuffer1[i].size());
            memset(&RMSBuffer2[i][0], 0, sizeof(f64) * RMSBuffer2[i].size());
        }
        RMSBufferOffset1 = 0;
        RMSBufferOffset2 = 0;
    }

    void        Buffers_UpdateChannels() {
        RMSBuffer1.resize(RMSBufferChannels);
        RMSBuffer2.resize(RMSBufferChannels);
        RMSBufferRMS.resize(RMSBufferChannels);
    }

    void        Buffers_Update() {
        for (u32 i = 0; i < RMSBufferChannels; i++) {
            u32 CountABMode = CompressModeB ? 256 : 16;

            RMSBufferSize1 = max(2, 16          * (u32)SampleRate / 44100);
            RMSBufferSize2 = max(2, CountABMode * (u32)SampleRate / 44100);

            RMSBuffer1[i].resize(RMSBufferSize1);
            RMSBuffer2[i].resize(RMSBufferSize2);

            RMSBufferRMS[i].resize(2);
        }
    }
};
