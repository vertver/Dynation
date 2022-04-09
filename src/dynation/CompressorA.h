#pragma once

#define COMPRESSORDC FLT_EPSILON

class CompressorA {
public:
    void        Update( f32 Samplerate, f32 At, f32 Rel, f32 Threshold_, f32 Ratio_) {
        KAttack     = At    == 0.0f ?   0.0 : (f32) exp(-1.0/(Samplerate*At));
        KRelease    = Rel   == 0.0f ?   0.0 : (f32) exp(-1.0/(Samplerate*Rel));
        Threshold   = Threshold_;
        ThresholdDB = lin2dB(Threshold) - 3; // fix th
        Ratio       = (1.0f - 1.0f/(Ratio_));
    }

    void        Reset( f32 Samplerate, f32 At, f32 Rel, f32 Threshold_, f32 Ratio_ ) {
        Update( Samplerate, At, Rel, Threshold_, Ratio_);
        Envelope    = COMPRESSORDC;
    }

    f64         NextSampleRMS( f64 InRms ) { // return output scale
        f64 InRmsDB = lin2dB(InRms + COMPRESSORDC);

        f64 OverDB  = InRmsDB - ThresholdDB;
        if (OverDB < 0.0) OverDB = 0.0;
        OverDB += COMPRESSORDC;
        
        f64 Theta = (OverDB > Envelope) ? KAttack : KRelease;
        Envelope = OverDB + Theta * (Envelope - OverDB);

        f32 Pvart = Envelope - COMPRESSORDC;
        if (Pvart > 0.0f) Pvart -= Envelope * Envelope * 0.001f; // opto pseudo curve
        f64 Gain = 0.0f - Pvart * Ratio;

        return dB2lin(Gain);

        /*f64 InRmsDB = log10(InRms);
        f64 Gain = 0.0;
        if ( InRmsDB >= ThresholdDB)  Gain = -(InRmsDB - ThresholdDB) * Ratio;


        f64 Theta = (Gain < Envelope) ? KAttack : KRelease;

        Envelope += Theta * (Gain - Envelope);

        Gain = pow(10, Envelope);

        return Gain;*/
    }

private:
    f64     Envelope;
    f32     Threshold;
    f64     ThresholdDB;
    f32     Ratio;
    f64     KAttack;
    f64     KRelease;
};
