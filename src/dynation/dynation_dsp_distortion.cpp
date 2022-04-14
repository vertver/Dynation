/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "dynation.h"
#include "dynation_dsp_distortions.h"

void
DynationPlugin::InitDistortion()
{
    ResetDistortion();
}

void
DynationPlugin::ResetDistortion()
{
    ProcessState.Distort = {};
    ProcessState.Distort.DeltaDriveLevel = State->State()->Drive;
}

void
DynationPlugin::ProcessDistortion(AudioProcessingBlock<float>& ProcessingBlock)
{
    const auto DeltaScale = 0.05 * (Samplerate / 44100.0); // 50ms coeff for delta drive value
    const auto DriveLevel = TempState.Drive;
    const auto HardnessLevel = TempState.Hardness;

    auto& GlobalDriveDeltaLevel = ProcessState.Distort.DeltaDriveLevel;
    auto DriveDeltaLevel = GlobalDriveDeltaLevel;

    const auto ProcessLoop = [&](auto&& Callable, auto... Parameters) {
        for (size_t i = 0; i < OutputChannels; i++) {
            auto* Data = ProcessingBlock.Outputs[i];
            DriveDeltaLevel = GlobalDriveDeltaLevel;
            for (size_t j = 0; j < ProcessingBlock.FramesToProcess; j++) {
                DriveDeltaLevel += ((DriveLevel - DriveDeltaLevel)) * DeltaScale;
                const auto DryValue = Data[j];
                const auto WetValue = Callable(DryValue, Parameters...);
                Data[j] = ((DryValue * (1.0 - DriveDeltaLevel)) + (WetValue * DriveDeltaLevel));
            }
        }

        GlobalDriveDeltaLevel = DriveDeltaLevel;
    };

    const auto TubeProcess = [HardnessLevel](const auto Sample, auto& Alpha, auto& Beta) -> auto {
        const auto FirstSignal = TubeTriode(Sample * 0.25);
        const auto SecondSignal = TubeTriode(TubeFailure(FirstSignal, Alpha, Beta) * 0.25);
        const auto OutMix = FirstSignal + ((SecondSignal - FirstSignal) * HardnessLevel);

        return OutMix;
    };

    const auto SincrusherProcess = [DriveDeltaLevel, HardnessLevel](const auto Sample) -> auto {
        const auto GainCompensatedDrive = DriveDeltaLevel * 2.0 + 1.0;
        const auto SincrusherDriveLevel = GainCompensatedDrive * Pi * 0.5;

        const auto FirstSignal = Sincrusher(Sample, SincrusherDriveLevel);
        const auto SecondSignal = Sincrusher(FirstSignal * Pi /* Crazy phase shifting here */, SincrusherDriveLevel);
        const auto OutMix = FirstSignal + ((SecondSignal - FirstSignal) * HardnessLevel);

        return (OutMix * (2. /* I really don't know remember why */));
    };

    const auto HardDistortionProcess = [DriveDeltaLevel, HardnessLevel](const auto Sample) -> auto {
        const auto GainCompensatedDrive = DriveDeltaLevel * 2.0 + 1.0;
        const auto SincrusherDriveLevel = GainCompensatedDrive * Pi * 0.5;

        const auto FirstSignal = HardClip(Sample, GainCompensatedDrive);
        const auto SecondSignal = Sincrusher(FirstSignal * Pi, SincrusherDriveLevel);
        const auto OutMix = FirstSignal + ((SecondSignal - FirstSignal) * HardnessLevel * 0.8);

        return (OutMix * (2. /* I really don't know remember why */));
    };

    const auto FoldBackDistortionProcess = [DriveDeltaLevel, HardnessLevel](const auto Sample) -> auto {
        const auto GainCompensatedDrive = DriveDeltaLevel * 1.0 + 1.0;

        const auto FirstSignal = FoldbackDistortion(Sample, GainCompensatedDrive);
        const auto SecondSignal = SoftClipSecond(FirstSignal, GainCompensatedDrive, 0.85 /* Experiment with that */);
        const auto OutMix = (FirstSignal + ((SecondSignal - FirstSignal) * HardnessLevel * 0.8));

        return OutMix * GainCompensatedDrive;
    };

    const auto SoftClipperProcess = [DriveDeltaLevel, HardnessLevel](const auto Sample) -> auto {
        const auto GainCompensatedDrive = DriveDeltaLevel * 2.0 + 1.0;
        const auto SincrusherDriveLevel = GainCompensatedDrive * Pi * 0.5;

        const auto FirstSignal = FoldbackDistortion(Sample, GainCompensatedDrive);
        const auto SecondSignal = Sincrusher(FirstSignal * Pi, SincrusherDriveLevel);

        const auto OutMix = (FirstSignal + ((SecondSignal - FirstSignal) * HardnessLevel * 0.8));
        return OutMix * 2.0;
    };

    switch (TempState.DistortType) {
    case DistortionType::NoneType:
        break;
    case DistortionType::TubeDistortion:
        TubeDistortionReset(0.5f /* UNUSED */, ProcessState.Distort.Alpha, ProcessState.Distort.Beta);
        ProcessLoop(TubeProcess, ProcessState.Distort.Alpha, ProcessState.Distort.Beta);
        break;
    case DistortionType::Sincrusher:
        ProcessLoop(SincrusherProcess);
        break;
    case DistortionType::HardClip:
        ProcessLoop(HardDistortionProcess);
        break;
    case DistortionType::FBack:
        ProcessLoop(FoldBackDistortionProcess);
        break;
    case DistortionType::SoftClip:
        ProcessLoop(SoftClipperProcess);
        break;
    default:
        break;
    }
}