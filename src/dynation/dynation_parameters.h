/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

#define PARAMETERS_COUNT 34

enum class DistortionType : int16_t
{
    NoneType,
    TubeDistortion,
    Sincrusher,
    HardClip,
    FBack,
    SoftClip,

    // For TypeConverter
    MaxEnum
};

enum class CompressorStatus : int16_t
{
    Disabled,
    PreComp,
    PostComp,

    // For TypeConverter
    MaxEnum
};

enum class CompressorMode : int16_t
{
    BasicCompressor,
    EnvelopeCompressor,
    RMSCompressor,
    AnalogCompressor,

    // For TypeConverter
    MaxEnum
};

struct CompressorState
{
    CompressorStatus CompStatus = CompressorStatus::Disabled;
    CompressorMode CompressorNode = CompressorMode::BasicCompressor;
    int16_t reserved2 = 0;
    int16_t reserved3 = 0;

    // Time block
    float Attack = 0.f;							// Attack 
    float Release = 0.f;						// Release
    volume_gain Threshold = volume_gain(0.f);	// Linear threshold 
    float Ratio = 1.f;							// 1:x ration value

    // Mixing block
    float Reserved = 0.f;
    float ParallelMix = 1.f;					// Parallel mixing
    volume_gain PumpGain = volume_gain(0.f);	// Amplifier value in dB
    float AnalogSubmix = 0.f;					// Mix between two parallel compressor with unique sound
};

struct DynationState
{
    // States and modes block
    DistortionType DistortType = DistortionType::NoneType;
    int16_t reserved = 0;

    // Base block
    float DryWet = 1.f;								// Global dry/wet level
    volume_gain InputVolume = volume_gain(0.f);		// input volume in dB
    volume_gain OutputVolume = volume_gain(0.f);	// output volume in dB
    float TiltEQ = 0.5f;							// Tilt EQ level

    // Distortion block
    float Drive = 0.f;				// Distortion drive! (dry/wet of distortion + power of distortion)
    float Hardness = 0.f;			// Distortion hardness.

    // Bitcrusher
    float Downshifter = 0.f;		// Downshifter level
    float Bitshifter = 0.f;			// Bitshifter level

    // Compressor
    CompressorState FirstCompressor;
    CompressorState SecondCompressor;
};

constexpr size_t SizeOfState = sizeof(DynationState);

template<>
struct TypeConverter<DistortionType>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(DistortionType Value)
    {
        switch (Value) {
        case DistortionType::NoneType:
            return "None";
            break;
        case DistortionType::TubeDistortion:
            return "Tube distortion";
            break;
        case DistortionType::Sincrusher:
            return "Sincrusher";
            break;
        case DistortionType::HardClip:
            return "Hard clipper";
            break;
        case DistortionType::FBack:
            return "Fold-back distortion";
            break;
        case DistortionType::SoftClip:
            return "Soft clipper";
            break;
        case DistortionType::MaxEnum:
            return "Max value";
            break;
        default:
            break;
        }

        return "Invalid value";
    }

    static float normalize(DistortionType val)
    {
        float MaxValue = static_cast<float>(static_cast<int16_t>(DistortionType::MaxEnum) - 1);
        float CurrentValue = static_cast<float>(static_cast<int16_t>(val));
        return CurrentValue / MaxValue;
    }

    static DistortionType denormalize(float val)
    {
        float MaxValue = static_cast<float>(static_cast<int16_t>(DistortionType::MaxEnum) - 1);
        float CurrentValue = val * MaxValue;
        return static_cast<DistortionType>(static_cast<int16_t>(CurrentValue));
    }
};

template<>
struct TypeConverter<CompressorStatus>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(CompressorStatus Value)
    {
        switch (Value) {
        case CompressorStatus::Disabled:
            return "Disabled";
            break;
        case CompressorStatus::PreComp:
            return "Pre compressor";
            break;
        case CompressorStatus::PostComp:
            return "Post compressor";
            break;
        case CompressorStatus::MaxEnum:
            return "Max value";
            break;
        default:
            break;
        }

        return "Invalid value";
    }

    static float normalize(CompressorStatus val)
    {
        float MaxValue = static_cast<float>(static_cast<int16_t>(CompressorStatus::MaxEnum) - 1);
        float CurrentValue = static_cast<float>(static_cast<int16_t>(val));
        return CurrentValue / MaxValue;
    }

    static CompressorStatus denormalize(float val)
    {
        float MaxValue = static_cast<float>(static_cast<int16_t>(CompressorStatus::MaxEnum) - 1);
        float CurrentValue = val * MaxValue;
        return static_cast<CompressorStatus>(static_cast<int16_t>(CurrentValue));
    }
};

template<>
struct TypeConverter<CompressorMode>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(CompressorMode Value)
    {
        switch (Value) {
        case CompressorMode::BasicCompressor:
            return "Basic compressor";
            break;
        case CompressorMode::EnvelopeCompressor:
            return "Envelope compressor";
            break;
        case CompressorMode::RMSCompressor:
            return "RMS compressor";
            break;
        case CompressorMode::AnalogCompressor:
            return "Analog compressor";
            break;
        case CompressorMode::MaxEnum:
            return "Max value";
            break;
        default:
            break;
        }
      
        return "Invalid value";
    }

    static float normalize(CompressorMode val)
    {
        float MaxValue = static_cast<float>(static_cast<int16_t>(CompressorMode::MaxEnum) - 1);
        float CurrentValue = static_cast<float>(static_cast<int16_t>(val));
        return CurrentValue / MaxValue;
    }

    static CompressorMode denormalize(float val)
    {
        float MaxValue = static_cast<float>(static_cast<int16_t>(CompressorMode::MaxEnum) - 1);
        float CurrentValue = val * MaxValue;
        return static_cast<CompressorMode>(static_cast<int16_t>(CurrentValue));
    }
};

typedef std::variant<
	std::monostate,
	bool,
	float,
	int16_t,

    log_gain,
    volume_gain,

	DistortionType,
	CompressorStatus,
	CompressorMode
	//GainLevel
	//QRatio,
	//CompressorRatio,
	//LinearHertz,
	//Log10Hertz,
	//LogTime,
	//FilterSlopeValue
> DynationParamType;
using DynationContainer = ParametersContainer<PARAMETERS_COUNT, DynationParamType>;

using StateType = DynationState;

