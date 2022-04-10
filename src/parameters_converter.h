/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

using log_gain = strong::type<float, struct log_gain_>;
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

#define ParamToLog(x, mn, mx) (exp(log(mn) + (x) * (log(mx) - log(mn))))
#define LogToParam(x, mn, mx) ((log(x) - log(mn)) / (log(mx) - log(mn)))

#define ParamToLin(x, mn, mx) ((mn) + (x) * ((mx) - (mn)))
#define LinToParam(x, mn, mx) (((x) - (mn)) / ((mx) - (mn)))

template<>
struct TypeConverter<log_gain>
{
    static std::string_view GetSymbol()
    {
        return "dB";
    }

    static std::string GetValueString(log_gain val)
    {
        return std::to_string(lin2dB(val.value_of()));
    }

    static float normalize(log_gain val, const float minv = -24.f, const float maxv = 24.f)
    {
        return LinToParam(val.value_of(), minv, maxv);
    }

    static log_gain denormalize(float val, const float minv = -24.f, const float maxv = 24.f)
    {
        return log_gain(ParamToLin(val, dB2lin(minv), dB2lin(maxv)));
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
        return std::to_string(lin2dB(val.value_of()));
    }

    static float normalize(volume_gain val, const float minv = -24.f, const float maxv = 24.f)
    {
        return  ((val.value_of() < minv)  ? (minv) :
                ((val.value_of() > maxv)   ? (maxv) :
                  val.value_of())) / maxv * 0.5f + 0.5f;
    }

    static volume_gain denormalize(float val, const float minv = -24.f, const float maxv = 24.f)
    {
        return volume_gain(dB2lin((val - 0.5f) * 2.0f * maxv));
    }
};