/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

class GainLevel
{
private:
    float Storage = 0.f;

public:
    GainLevel() = default;
    GainLevel(GainLevel&) = default;
    GainLevel(GainLevel&&) = default;
    static GainLevel CreateGainLevel(float InLevel)
    {
        GainLevel level;
        level.SetValue(InLevel);
        return level;
    }

    float GetValueNormalized() { return Storage; }
    float GetValue() { return lin2dB(Storage); }
    void SetValue(float Val) { Storage = dB2lin(Val); }
};

template<typename T>
struct TypeConverter;

template<>
struct TypeConverter<std::monostate>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(std::monostate Value)
    {
        return "";
    }

    static float normalize(std::monostate val)
    {
        return 0.f;
    }

    static std::monostate denormalize(float val)
    {
        return {};
    }
};

template<>
struct TypeConverter<bool>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(bool Value)
    {
        return Value ? "true" : "false";
    }

    static float normalize(bool val)
    {
        if (val) {
            return 1.f;
        }

        return 0.f;
    }

    static bool denormalize(float val)
    {
        if (val >= 0.5) {
            return true;
        }

        return false;
    }
};

template<>
struct TypeConverter<float>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(float Value)
    {
        return std::to_string(Value);
    }

    static float normalize(const float val, const float minv = 0.f, const float maxv = 1.0f)
    {
        return (1.f - val) * maxv + val * minv;
    }

    static float denormalize(float val, const float minv = 0.f, const float maxv = 1.0f)
    {
        return (val - minv) / (maxv - minv);
    }
};

template<>
struct TypeConverter<long>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(long Value)
    {
        return std::to_string(Value);
    }

    static float normalize(long val)
    {
        return 0.f;
    }

    static long denormalize(float val)
    {
        return 0;
    }
};

template<>
struct TypeConverter<GainLevel>
{
    static std::string_view GetSymbol()
    {
        return "dB";
    }

    static std::string GetValueString(GainLevel Value)
    {
        return std::to_string(Value.GetValue());
    }

    static float normalize(GainLevel val)
    {
        return val.GetValueNormalized();
    }

    static GainLevel denormalize(float val)
    {
        return GainLevel::CreateGainLevel(val);
    }
};