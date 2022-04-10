#*********************************************************************
# Copyright (C) Anton Kovalev, 2018-2022. All rights reserved.
# Dynation plugin
# MIT License
#****************************************************************/
cmake_minimum_required(VERSION 3.16)
project(Dynation C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

######################################
#
# Options setup here
#
#####################################
option(DYNATION_BUILD_VST2 "Build VST2 version of Dynation" ON)
option(DYNATION_BUILD_CLAP "Build CLAP version of Dynation" OFF)
option(DYNATION_BUILD_UI "Build Dynation with UI" OFF)

# TODO: other targets check
if (NOT DYNATION_BUILD_VST2 AND NOT DYNATION_BUILD_CLAP)
	message(FATAL_ERROR "No targets selected to build. Please, select required target.")
endif()

######################################
#
# Sources list setup
#
#####################################
if (DYNATION_BUILD_VST2)
	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK")
		message(FATAL_ERROR "Can't find VST2 SDK. Please, copy it to deps folder.")
	endif()

	file(GLOB DYNATION_VST2_SDK_SOURCE_CODE	
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/public.sdk/source/vst2.x/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/public.sdk/source/vst2.x/*.h"
	)
	
	file(GLOB DYNATION_VST2_HOST_SOURCE_CODE
		"${CMAKE_CURRENT_SOURCE_DIR}/src/hosts/vst2/vst2_host.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/hosts/vst2/vst2_host.h"
	)
	
	file(GLOB DYNATION_VST2_EXPORT_SOURCE_CODE
		"${CMAKE_CURRENT_SOURCE_DIR}/src/hosts/vst2/vst2_export.cpp"
	)
else()
	set(DYNATION_VST2_HOST_SOURCE_CODE "")
	set(DYNATION_VST2_EXPORT_SOURCE_CODE "")
endif()

if (DYNATION_BUILD_UI)
	file(GLOB DYNATION_IMGUI_SOURCE_CODE	
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui-master/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui-master/*.h"
	)
	
	file(GLOB DYNATION_IMGUI_WINDOW_SOURCE_CODE	
		"${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.h"
	)
else()
	set(DYNATION_IMGUI_WINDOW_SOURCE_CODE "")
endif()

file(GLOB DYNATION_BASE_SOURCE_CODE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

file(GLOB DYNATION_PLUGIN_SOURCE_CODE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/dynation/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/dynation/*.h"
)

######################################
#
# Dependencies
#
#####################################
if (DYNATION_BUILD_VST2)
	add_library(vst2 STATIC ${DYNATION_VST2_SDK_SOURCE_CODE})	
	target_include_directories(vst2 PUBLIC 
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/pluginterfaces/vst2.x"
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/public.sdk/source/vst2.x"
	)
endif()

if (DYNATION_BUILD_UI)
	add_library(imgui STATIC ${DYNATION_IMGUI_SOURCE_CODE})
	target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui-master")
endif()

######################################
#
# Base library setup
#
#####################################
set(LIBRARIES_TO_LINK)

if (DYNATION_BUILD_UI)
	list(APPEND LIBRARIES_TO_LINK imgui)
endif()

if (DYNATION_BUILD_VST2)
	list(APPEND LIBRARIES_TO_LINK vst2)
endif()

add_library(base STATIC
	${DYNATION_BASE_SOURCE_CODE}
	${DYNATION_VST2_HOST_SOURCE_CODE}
	${DYNATION_IMGUI_WINDOW_SOURCE_CODE}
)
target_link_libraries(base PUBLIC ${LIBRARIES_TO_LINK})

if (DYNATION_BUILD_UI)
	target_compile_definitions(base PUBLIC PLUGIN_UI_SUPPORT)	
endif()

if (DYNATION_BUILD_VST2)
	target_compile_definitions(base PUBLIC PLUGIN_VST2_SUPPORT)	
endif()

target_include_directories(base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_precompile_headers(base
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
)

######################################
#
# Plugin setup 
#
#####################################
add_library(dynation STATIC
	${DYNATION_PLUGIN_SOURCE_CODE}
)

target_link_libraries(dynation PUBLIC base)

if (DYNATION_BUILD_VST2)
	add_library(dynation_vst2 MODULE
		${DYNATION_VST2_EXPORT_SOURCE_CODE}
	)

	target_link_libraries(dynation_vst2 PRIVATE dynation)
	target_include_directories(dynation_vst2 PRIVATE 
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/pluginterfaces/vst2.x"
		"${CMAKE_CURRENT_SOURCE_DIR}/deps/VST2_SDK/public.sdk/source/vst2.x"
	)
	
	set_target_properties(dynation_vst2
		PROPERTIES
			LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
	)
endif()