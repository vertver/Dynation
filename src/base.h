/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once
#include <string>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <variant>
#include <array>

#include "strong_type.h"
#include "dsp_math.h"

#include "plugin.h"
#include "parameters_converter.h"
#include "parameters_container.h"

#if defined (__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define PLUGIN_EXPORT	__attribute__ ((visibility ("default")))
#else
#define PLUGIN_EXPORT __declspec(dllexport)
#endif