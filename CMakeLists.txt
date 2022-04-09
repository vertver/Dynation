#*********************************************************************
# Copyright (C) Anton Kovalev, 2018-2022. All rights reserved.
# Dynation plugin
# MIT License
#****************************************************************/
cmake_minimum_required(VERSION 3.16)
project(Dynation C CXX)

file(GLOB DYNATION_BASE_SOURCE_CODE
	"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/public.sdk/source/vst2.x/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/public.sdk/source/vst2.x/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui-master/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui-master/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

file(GLOB DYNATION_PLUGIN_SOURCE_CODE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/dynation/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/dynation/*.h"
)

add_library(dyn_base STATIC ${DYNATION_BASE_SOURCE_CODE})

target_include_directories(dyn_base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps")
target_include_directories(dyn_base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(dyn_base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/pluginterfaces/vst2.x")
target_include_directories(dyn_base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/public.sdk/source/vst2.x")
target_include_directories(dyn_base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui-master")

add_library(dynation MODULE ${DYNATION_PLUGIN_SOURCE_CODE})
target_link_libraries(dynation PUBLIC dyn_base)

set_target_properties(dynation
    PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)