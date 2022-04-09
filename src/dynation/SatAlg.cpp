/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "Header.h"

////////////////////////////////////////////////////////

f32 SatAlg_HardClip(f32 In, f32 Threshold) {
    if (In > Threshold) return Threshold;
    if (In < -Threshold) return -Threshold;
    return In;
}

f64 SatAlg_HardClipD(f64 In, f64 Threshold) {
    if (In > Threshold) return Threshold;
    if (In < -Threshold) return -Threshold;
    return In;
}

f32 SatAlg_HardClipDrive(f32 In, f32 Drive) {
    f32 Value = In * Drive;
    if (Value > 1.0f)  Value = 1.0f;
    if (Value < -1.0f) Value = -1.0f;
    Value *= (1.0f / Drive);
    return Value;
}

f64 SatAlg_HardClipDriveD(f64 In, f64 Drive) {
    f64 Value = In * Drive;
    if (Value > 1.0)  Value = 1.0;
    if (Value < -1.0) Value = -1.0;
    Value *= (1.0 / Drive);
    return Value;
}


f32 SatAlg_HardClipDriveCurve(f32 In, f32 Drive, f32 Curve) {
    f32 Value = In * Drive;
    if (Value > 1.0f) {
        Value = 1.0f - sin((Value - 1.0f) * Curve * 3.14159265358979323846f * 0.1f);
        if (Value < 0.0f) Value = 0.0f;
    }
    if (Value < -1.0f) {
        Value = -1.0f - sin((Value - -1.0f) * Curve * 3.14159265358979323846f * 0.1f);
        if (Value > 0.0f) Value = 0.0f;
    }
    Value *= (1.0f / Drive);
    return Value;
}

f64 SatAlg_HardClipDriveCurveD(f64 In, f64 Drive, f32 Curve) {
    f64 Value = In * Drive;
    if (Value > 1.0) {
        Value = 1.0 - sin((Value - 1.0) * Curve * 3.14159265358979323846f * 0.1f);
        if (Value < 0.0) Value = 0.0;
    }
    if (Value < -1.0) {
        Value = -1.0 - sin((Value - -1.0) * Curve * 3.14159265358979323846f * 0.1f);
        if (Value > 0.0) Value = 0.0;
    }
    Value *= (1.0 / Drive);
    return Value;
}

////////////////////////////////////////////////////////

f32 SatAlg_FoldbackDrive(f32 In, f32 Drive) {
    f32 Value = In * Drive;
    const f32 threshold = 1.0f;
    if (Value > threshold || Value < -threshold) {
        Value = fabsf(fabsf(fmodf(Value - threshold, threshold * 4.0f)) - threshold * 2.0f) - threshold;
    }
    Value *= (1.0 / Drive);
    return Value;
}

f64 SatAlg_FoldbackDriveD(f64 In, f64 Drive) {
    f64 Value = In * Drive;
    const f64 threshold = 1.0;
    if (Value > threshold || Value < -threshold) {
        Value = fabs(fabs(fmod(Value - threshold, threshold * 4.0)) - threshold * 2.0) - threshold;
    }
    Value *= (1.0 / Drive);
    return Value;
}

////////////////////////////////////////////////////////

f32 SatAlg_SoftClip2Drive(f32 In, f32 Drive, f32 A) {
    f32 Value       = In * Drive;
    f32 ValueAbs    = fabsf(Value);
    if (ValueAbs > 1.0f) {
        ValueAbs = (A + 1.0f) * 0.5f;
    } else if(ValueAbs > A) {
        ValueAbs = A + (ValueAbs - A) / (1.0f + pow((ValueAbs - A) / (1.0f - A), 2.0f));
    }
    Value = ValueAbs * sign(Value);
    Value *= (1.0f / Drive);
    return Value;
}

f64 SatAlg_SoftClip2DriveD(f64 In, f64 Drive, f64 A) {
    f64 Value = In * Drive;
    f64 ValueAbs    = fabs(Value);
    if (ValueAbs > 1.0) {
        ValueAbs = (A + 1.0) * 0.5;
    } else if(ValueAbs > A) {
        ValueAbs = A + (ValueAbs - A) / (1.0 + pow((ValueAbs - A) / (1.0 - A), 2.0));
    }
    Value = ValueAbs * sign(Value);
    Value *= (1.0 / Drive);
    return Value;
}

////////////////////////////////////////////////////////

f64 SatAlg_fasttanh2( f64 x) {
    f64 ax = fabs(x);
    f64 x2 = x * x;

    return(x * (2.45550750702956 + 2.45550750702956 * ax +
        (0.893229853513558 + 0.821226666969744 * ax) * x2) /
        (2.44506634652299 + (2.44506634652299 + x2) *
            fabs(x + 0.814642734961073 * x * ax)));
}

////////////////////////////////////////////////////////

f32 SatAlg_SoftClip(f32 In) {
    return (f32)SatAlg_fasttanh2((f64)In);
}

f64 SatAlg_SoftClipD(f64 In) {
    return SatAlg_fasttanh2(In);
}

f32 SatAlg_SoftClipDrive(f32 In, f32 Drive) {
    f32 Value = In * Drive;
    Value = (f32)SatAlg_fasttanh2((f64)Value);
    Value *= (1.0f / Drive);
    return Value;
}

f64 SatAlg_SoftClipDriveD(f64 In, f64 Drive) {
    f64 Value = In * Drive;
    Value = SatAlg_fasttanh2(Value);
    Value *= (1.0 / Drive);
    return Value;
}

////////////////////////////////////////////////////////
void SatAlg_BitCrushingInit(u32 DepthBits, u32& ValuePow) {
	ValuePow = pow(2, DepthBits) - 1;
}

f32 SatAlg_BitCrushing_ROUND(f32 f) { 
	return ((f32)((f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5))); 
}

f64 SatAlg_BitCrushing_ROUNDD(f64 f) {
	return ((f64)((f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5)));
}

f32 SatAlg_BitCrushing(f32 In, u32& ValuePow) {
	f32 Value = In;

	Value = SatAlg_BitCrushing_ROUND((Value + 1.0f) * ValuePow) / ValuePow - 1.0f;

	return Value;
}

f64 SatAlg_BitCrushingD(f64 In, u32& ValuePow) {
	f64 Value = In;

	Value = SatAlg_BitCrushing_ROUNDD((Value + 1.0) * ValuePow) / ValuePow - 1.0;

	return Value;
}
////////////////////////////////////////////////////////

f32 SatAlg_RateReducer(f32 In, f32& SaveLoadValue, u32& SaveLoadValue2, u32 SampleRateDivNeadRate) {
    if (SaveLoadValue2 == 0) SaveLoadValue = In;
    SaveLoadValue2++;
    if (SaveLoadValue2 >= SampleRateDivNeadRate)  SaveLoadValue2 = 0;
    return SaveLoadValue;
}

f64 SatAlg_RateReducerD(f64 In, f64& SaveLoadValue, u32& SaveLoadValue2, u32 SampleRateDivNeadRate) {
    if (SaveLoadValue2 == 0) SaveLoadValue = In;
    SaveLoadValue2++;
    if (SaveLoadValue2 >= SampleRateDivNeadRate)  SaveLoadValue2 = 0;
    return SaveLoadValue;
}

////////////////////////////////////////////////////////

f32 SatAlg_ADCFailure(f32 In, f32& SaveLoadValue, f32 Error) {
    if (Error == 0.0f)  return In;
    f32 RandPercent = (f32)(rand() % (100 + 1)) / 100.0f;
    if (RandPercent < Error) return SaveLoadValue;
    SaveLoadValue = In;
    return In;
}

f64 SatAlg_ADCFailureD(f64 In, f64& SaveLoadValue, f64 Error) {
    if (Error == 0.0) return In;
    f64 RandPercent = (f64)(rand() % (100 + 1)) / 100.0f;
    if (RandPercent < Error) return SaveLoadValue;
    SaveLoadValue = In;
    return In;
}

////////////////////////////////////////////////////////

void SatAlg_TubeTriodeInit(f32 Drive, f32& SaveLoada, f32& SaveLoadk) {
    SaveLoada = sin(((Drive * 30.0f) / 101.0f) * (3.14159265358979323846f / 2.0f));
    SaveLoadk = 2.0f * SaveLoada / (1.0f - SaveLoada);
}

f32 SatAlg_FailureTubeTriode(f32 In, f32& SaveLoada, f32& SaveLoadk) {
	return (1.0f + SaveLoadk) * (In) / (1.0f + SaveLoadk * fabsf(In)) * 0.9f;
}

f64 SatAlg_FailureTubeTriodeD(f64 In, f32& SaveLoada, f32& SaveLoadk) {
	return (1.0 + (f64)SaveLoadk) * (In) / (1.0 + (f64)SaveLoadk * fabs(In)) * 0.9f;
}

f32 SatAlg_TubeTriode(f32 In) {	
	f64 x = (f64)In;
	
	// lamp1
	x = sign(x) * ((2.0 - fabs(x)) * fabs(x));

	// lamp2
	if (x < -1.0f) {
		x = -1.0f;
	} else if (x < -0.08905) {
		f64 Temp6		= (fabs(x) - 0.032847);
		f64 Temp6_pow12 = pow((1.0 - Temp6), 12.0);
		f64 Temp7		= 1.0 - Temp6_pow12 + Temp6 * (1.0 / 3.0);
		x = -(0.01 + (3.0 / 4.0) * Temp7);
	} else if (x < 0.320018) {
		x = -6.152* x * x + x * 3.9375;
	} else {
		x = 0.630035;
	}
	
	return x;
}

f64 SatAlg_TubeTriodeD(f64 In) {
	f64 x = In;
	
	// lamp1
	x = sign(x) * ((2.0 - fabs(x)) * fabs(x));

	// lamp2
	if (x < -1.0f) {
		x = -1.0f;
	} else if (x < -0.08905) {
		f64 Temp6		= (fabs(x) - 0.032847);
		f64 Temp6_pow12 = pow((1.0 - Temp6), 12.0);
		f64 Temp7		= 1.0 - Temp6_pow12 + Temp6 * (1.0 / 3.0);
		x = -(0.01 + (3.0 / 4.0) * Temp7);
	} else if (x < 0.320018) {
		x = -6.152* x * x + x * 3.9375;
	} else {
		x = 0.630035;
	}
	
	return x;
}

////////////////////////////////////////////////////////

f32 SatAlg_Sincrusher(f32 In, f32 Drive) {
	f32 Value = In;
	Value *= Drive;
	Value = sin(Value);
	Value /= Drive;
	return Value;
}

f64 SatAlg_SincrusherD(f64 In, f64 Drive) {
	f64 Value = In;
	Value *= Drive;
	Value = sin(Value);
	Value /= Drive;
	return Value;
}

////////////////////////////////////////////////////////

