#pragma once

struct TiltEQAlg_Simple { // Tilt eq 1000gz +- 18db
    void    SetSampleRate(f32 SampleRate) {
		f32	sr3 = 3.0f * SampleRate;
		f32 Omega = 2.0f * 3.14159265358979323846f * (f32)1000.0;
		f32 n = 1 / (sr3 + Omega);
		a0 = 2 * Omega * n;
		b1 = (sr3 - Omega) * n;
    }

	void	SetEQLevel(f32 EQLevel) {
		const f32 G_factor = 5.0f;
		
		if (EQLevel > 0) {
			g1 = -G_factor * EQLevel * 6.0;
			g2 = EQLevel * 6.0;
		} else {
			g1 = -EQLevel * 6.0;
			g2 = G_factor * EQLevel * 6.0;
		}

		f32 Amp = 6.0 / log(2.0);
		lgain	= exp(g1 / Amp) - 1;
		hgain	= exp(g2 / Amp) - 1;
	}

    f32     NextSample(f32 In) {
		prev_out = a0 * In + b1 * prev_out;
		return In + lgain * prev_out + hgain * (In - prev_out);
    }
private:
    f32		prev_out = 0.0f;
	f32		g1 = 0.0f;
	f32		g2 = 0.0f;
	f32		lgain = 0.0f;
	f32		hgain = 0.0f;
	f32		a0 = 0.0f;
	f32		b1 = 0.0f;
};
