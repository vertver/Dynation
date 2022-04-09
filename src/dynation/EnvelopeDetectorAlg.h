#pragma once

class EnvelopeDetector {
public:
    void        Reset( f32 Samplerate , f32 SpecAt = 0.0f, f32 SpecRel = 0.0f) {
        KAttack  = SpecAt  == 0.0f ? 0.0f : (float)exp(-1/(Samplerate* SpecAt  * 0.001));
        KRelease = SpecRel == 0.0f ? 0.0f : (float)exp(-1/(Samplerate* SpecRel * 0.001));
        Envelope = 0.0;
        EnvelopeMax = 0.0;
    }

    void        NextSample( f64 In ) {
        In = fabs(In);

        if( EnvelopeMax < In )  EnvelopeMax = In;
        else                    EnvelopeMax = In + KRelease * (EnvelopeMax - In);

        if( Envelope < In ) Envelope = In + KAttack * (Envelope - In);
        else                Envelope = In + KRelease * (Envelope - In);
    }

	f64         GetEnvelop() {
        return Envelope;
    }

    f64         GetEnvelopMax( ) {
        return EnvelopeMax;
    }
private:
    f64     Envelope;
    f64     EnvelopeMax;
    f64     KAttack;
    f64     KRelease;
};

