#pragma once

f32 SatAlg_HardClip(f32 In, f32 Threshold);
f64 SatAlg_HardClipD(f64 In, f64 Threshold);

f32 SatAlg_HardClipDrive(f32 In, f32 Drive); // Drive = 1..N
f64 SatAlg_HardClipDriveD(f64 In, f64 Drive); // Drive = 1..N

f32 SatAlg_HardClipDriveCurve(f32 In, f32 Drive, f32 Curve); // Drive = 1..N Curve = 0..1
f64 SatAlg_HardClipDriveCurveD(f64 In, f64 Drive, f32 Curve); // Drive = 1..N Curve = 0..1

f32 SatAlg_FoldbackDrive(f32 In, f32 Drive); // Drive = 1..N
f64 SatAlg_FoldbackDriveD(f64 In, f64 Drive); // Drive = 1..N

f32 SatAlg_SoftClip2Drive(f32 In, f32 Drive, f32 A); // Drive = 1..N  A = 0..1
f64 SatAlg_SoftClip2DriveD(f64 In, f64 Drive, f64 A); // Drive = 1..N

f64 SatAlg_fasttanh2(const f64 x);

f32 SatAlg_SoftClip(f32 In);
f64 SatAlg_SoftClipD(f64 In);

f32 SatAlg_SoftClipDrive(f32 In, f32 Drive); // Drive = 1..N
f64 SatAlg_SoftClipDriveD(f64 In, f64 Drive); // Drive = 1..N

void SatAlg_BitCrushingInit(u32 DepthBits, u32& ValuePow);
f32 SatAlg_BitCrushing(f32 In, u32& ValuePow);
f64 SatAlg_BitCrushingD(f64 In, u32& ValuePow);

f32 SatAlg_RateReducer(f32 In, f32& SaveLoadValue, u32& SaveLoadValue2, u32 SampleRateDivNeadRate);
f64 SatAlg_RateReducerD(f64 In, f64& SaveLoadValue, u32& SaveLoadValue2, u32 SampleRateDivNeadRate);

// Эффект ошибки ацп типа белый шум но прикольней (заедание семпла)
f32 SatAlg_ADCFailure(f32 In, f32& SaveLoadValue, f32 Error); // Error = 0..1
f64 SatAlg_ADCFailureD(f64 In, f64& SaveLoadValue, f64 Error); // Error = 0..1

// tube double-triode and failure triode
void SatAlg_TubeTriodeInit(f32 Drive, f32& SaveLoada, f32& SaveLoadk); // Drive = 0..N
f32 SatAlg_FailureTubeTriode(f32 In, f32& SaveLoada, f32& SaveLoadk);
f64 SatAlg_FailureTubeTriodeD(f64 In, f32& SaveLoada, f32& SaveLoadk);
f32 SatAlg_TubeTriode(f32 In);
f64 SatAlg_TubeTriodeD(f64 In);

f32 SatAlg_Sincrusher(f32 In, f32 Drive); // Drive = 1..N
f64 SatAlg_SincrusherD(f64 In, f64 Drive); // Drive = 1..N
