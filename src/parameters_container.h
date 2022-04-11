/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

struct ParameterStruct
{
    std::string_view ParameterName;
    void* ParameterRawPointer = nullptr;
};

template<std::int64_t N, typename ParamType>
class ParametersContainer
{
private:
    using OnParameterChangeCallback = fu2::function<void(ParamType)>;

    using InfosContainer = std::array<ParameterStruct, N>;
    using ParamsContainer = std::array<ParamType, N>;
    using FuncsContainer = std::array<OnParameterChangeCallback, N>;

    InfosContainer ParametersInfo;
    ParamsContainer Parameters;
    FuncsContainer Functions;

public:
    // Allow only "MoveConstructible" semantic for this class
    ParametersContainer(ParametersContainer&) = delete;
    ParametersContainer(ParametersContainer&&) = default;

    ParametersContainer(std::initializer_list<std::tuple<ParamType, ParameterStruct, OnParameterChangeCallback>> InitList)
    {
        if (InitList.size() != N) {
            throw std::out_of_range("initializer_list size doesn't equal container's size!");
        }    

        int32_t Counter = 0;
        for (auto const& [Type, Info, Callback] : InitList) {
            SetRawValue(Counter, Type);
            ParametersInfo[Counter] = std::move(Info);
            Functions[Counter] = Callback;
            Counter++;
        }
    }

    constexpr std::int64_t GetParamsCount()
    {
        return N;
    }

    std::string_view GetParameterName(const int32_t Index)
    {
        return ParametersInfo[Index].ParameterName;
    }

    std::string_view GetParameterSymbol(const int32_t Index)
    {
        std::string_view ReturnString = "";
        std::visit([&](auto&& arg) {
            using U = std::decay_t<decltype(arg)>;
            ReturnString = TypeConverter<U>::GetSymbol();
        }, Parameters[Index]);

        return ReturnString;
    }

    std::string GetParameterValueString(const int32_t Index)
    {
        std::string ReturnString = "";
        std::visit([&](auto&& arg) {
            using U = std::decay_t<decltype(arg)>;
            ReturnString = TypeConverter<U>::GetValueString(std::get<U>(Parameters[Index]));
        }, Parameters[Index]);

        return ReturnString;
    }

    void SetParameterRawPtr(const int32_t Index, void* NewLocation)
    {
        ParametersInfo[Index].ParameterRawPointer = NewLocation;
    }

    template<typename T>
    bool HoldsNeedyType(const int32_t Index)
    {
        if (Index >= Parameters.size()) {
            throw std::out_of_range("Index doesn't equal container's size!");
        }

        return std::holds_alternative<T>(Parameters[Index]);
    }

    template<typename T>
    bool SetValue(const int32_t Index, T ValueToSet)
    {
        if (Index >= Parameters.size()) {
            return false;
        }

        // If current parameter contains value and type of value doesn't
        // equal type of input value - don't try to read this value
        if (!std::holds_alternative<std::monostate>(Parameters[Index]) &&
            !std::holds_alternative<T>(Parameters[Index])) {
            return false;
        }

        Parameters[Index] = ValueToSet;
        return true;
    }

    bool SetRawValue(const int32_t Index, ParamType OutValue)
    {
        std::visit([&](auto&& arg) {
            using U = std::decay_t<decltype(arg)>;
            Parameters[Index] = std::get<U>(OutValue);
            U* RawPtr = reinterpret_cast<U*>(ParametersInfo[Index].ParameterRawPointer);
            if (RawPtr != nullptr) {
                *RawPtr = std::get<U>(Parameters[Index]);
            }
        }, OutValue);

        return true;
    }

    bool GetRawValue(const int32_t Index, ParamType& OutValue) noexcept
    {
        std::visit([&](auto&& arg) {
            using U = std::decay_t<decltype(arg)>;
            OutValue = std::get<U>(Parameters[Index]);
        }, Parameters[Index]);

        return true;
    }

    template<typename T>
    bool GetValue(const int32_t Index, T& OutValue) noexcept
    {
        try {
            if (Index >= Parameters.size()) {
                return false;
            }

            bool isVisited = true;
            std::visit([&](auto&& arg) {
                using U = std::decay_t<decltype(arg)>;
                if constexpr (!std::is_same_v<U, T>) {
                    isVisited = false;
                    return;
                }
            }, Parameters[Index]);

            if (!isVisited) {
                return false;
            }

            OutValue = std::get<T>(Parameters[Index]);
        }
        catch (const std::exception& exc) {
            // Don't care what exactly exception currently happen, just print to log something
            PLUGINS_LOG(exc.what());
            return false;
        }

        return true;
    }

    template <typename ... Args>
    bool GetValueNormalized(const int32_t Index, float& OutValue, Args const& ... args) noexcept
    {
        try {
            if (Index >= Parameters.size()) {
                return false;
            }

            bool isVisited = true;
            std::visit([&](auto&& arg) {
                using U = std::decay_t<decltype(arg)>;
                OutValue = TypeConverter<U>::normalize(std::get<U>(Parameters[Index]), args...);
            }, Parameters[Index]);
        }
        catch (const std::exception& exc) {
            return false;
        }

        return true;
    }

    template <typename ... Args>
    bool SetValueNormalized(const int32_t Index, float& OutValue, Args const& ... args) noexcept
    {
        try {
            if (Index >= Parameters.size()) {
                return false;
            }

            bool isVisited = true;
            std::visit([&](auto&& arg) {
                using U = std::decay_t<decltype(arg)>;
                Parameters[Index] = TypeConverter<U>::denormalize(OutValue, args...);
                Functions[Index](Parameters[Index]);

                U* RawPtr = reinterpret_cast<U*>(ParametersInfo[Index].ParameterRawPointer);
                if (RawPtr != nullptr) {
                    U TempValue = std::get<U>(Parameters[Index]);
                    *RawPtr = TempValue;
                }
            }, Parameters[Index]);
        }
        catch (const std::exception& exc) {
            return false;
        }

        return true;
    }
};

