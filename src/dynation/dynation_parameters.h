#pragma once

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

