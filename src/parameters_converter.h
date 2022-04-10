/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

using volume_gain = strong::type<float, struct volume_gain_>;

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
struct TypeConverter<int16_t>
{
    static std::string_view GetSymbol()
    {
        return "";
    }

    static std::string GetValueString(int16_t Value)
    {
        return std::to_string(Value);
    }

    static float normalize(int16_t val)
    {
        return 0.f;
    }

    static int16_t denormalize(float val)
    {
        return 0;
    }
};

template<>
struct TypeConverter<volume_gain>
{
    static std::string_view GetSymbol()
    {
        return "dB";
    }

    static std::string GetValueString(volume_gain val)
    {
        return std::to_string(val.value_of());
    }

    static float normalize(volume_gain val)
    {
        return val.value_of();
    }

    static volume_gain denormalize(float val)
    {
        return volume_gain(lin2dB(val));
    }
};