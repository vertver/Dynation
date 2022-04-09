#pragma once
#ifdef WIN32
#pragma warning (disable : 4244)
#pragma warning (disable : 4305)
#endif

#include "audioeffectx.h"
#include "aeffeditor.h"

#ifdef WIN32
#include <windows.h>
#else

#endif
#include <math.h>
#include <stdio.h>
#include "VST2_Types.h"

inline bool ErrorMessage( PConstStr Str ) {
	const char* HeaderString = "Critical error";

#ifdef WIN32
	DebugBreak();
    return (MessageBoxA(NULL, Str, HeaderString, MB_OKCANCEL | MB_ICONHAND) == IDCANCEL);
#else
	CFOptionFlags result; 

	// Create string reference
	CFStringRef HeaderRef = CFStringCreateWithCString(NULL, HeaderString, strlen(HeaderString));
	CFStringRef MessageRef = CFStringCreateWithCString(NULL, Str, strlen(Str));

	// Message box
	CFUserNotificationDisplayAlert(0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, HeaderRef, MessageRef, NULL, CFSTR("Cancel"), NULL, &result);

	if (HeaderRef) CFRelease(HeaderRef);
	if (MessageRef) CFRelease(MessageRef);

	return (result != kCFUserNotificationDefaultResponse);
#endif
}

#if 0
#include <GL/gl.h>
#include "glext.h"
#pragma comment(lib, "opengl32.lib")
typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int(*PFNWGLEXTGETSWAPINTERVALPROC) (void);
typedef void (APIENTRY *__MYGLEXTFP_GLGENERATEMIPMAPS)(GLenum);
#endif

#ifdef WIN32
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#endif

#include "imgui.h"
#define   IMGUI_DEFINE_MATH_OPERATORS
#define	  USE_INTRINSICS
#include  "imgui_internal.h"

#define ImVec4ColorA(r, g, b, a) ImVec4((f32)r/256.0f, (f32)g/256.0f, (f32)b/256.0f, (f32)a/256.0f);
#define ImVec4Color(r, g, b) ImVec4ColorA(r, g, b, 255) 

#include "ImGui_Tools1.h"

#include "VST2_Base.h"
#ifdef USE_INTRINSICS
#include <intrin.h>
#endif

class StaticCS {
public:
#ifdef WIN32
    CRITICAL_SECTION CS;

    StaticCS( ) {
        InitializeCriticalSection (&CS);
    }
    ~StaticCS(){
        DeleteCriticalSection( &CS);
    }

    bool                    Try( ) { 
        if ( TryEnterCriticalSection( &CS ) == 0 ) return false;
        return true;
    };

    void                    Lock( ) { 
        //if ( TryEnterCriticalSection( &CS ) == 0 ) 
        EnterCriticalSection( &CS );
    };

    void                    Unlock( ) { 
        LeaveCriticalSection( &CS );
    };
#else
	pthread_mutex_t Mutex;

	StaticCS() {
		pthread_mutex_init(&Mutex, NULL);
	}
	~StaticCS() {
		pthread_mutex_destroy(&Mutex);
	}

	bool                    Try() {
		pthread_mutex_trylock(&Mutex);
	};

	void                    Lock() {
		pthread_mutex_lock(&Mutex);
	};

	void                    Unlock() {
		pthread_mutex_unlock(&Mutex);
	};
#endif
};

#ifndef DBL_EPSILON
#define DBL_EPSILON 2.2204460492503131e-16
#endif
#ifndef FLT_EPSILON
#define FLT_EPSILON 1.19209290e-7f
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.4142135623730950488016887
#endif

#ifndef M_2PI
#define M_2PI 6.283185307179586476925286766559005
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.7071067811865475244008443621048490
#endif

#ifndef M_LOG_2PI
#define M_LOG_2PI 1.8378770664093454835606594728112
#endif

#ifndef M_LN2
#define M_LN2 0.693147180559945309417232121458
#endif

#ifndef M_LN10
#define M_LN10 2.302585092994045684017991454684
#endif