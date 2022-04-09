/*
bool                    Window_Create(HWND Parent = nullptr);
void                    Window_Destroy();
LRESULT WINAPI   Window_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
HANDLE                  Window_PaintThread_Handle;
DWORD                   Window_PaintThread_ID;
bool                    Window_PaintThread_isRun;

bool                    Window_PaintThreadisCreate;
void                    Window_BeginPaintThread();
void                    Window_EndPaintThread();
DWORD WINAPI     Window_PaintThreadStatic(CONST LPVOID lpParam);
void                    Window_PaintThread();

public:
HGLRC                   OpenGL_Context;
bool                    OpenGL_ResetContext_isSet;

bool                    OpenGL_Create(bool NewCreate = false);
void                    OpenGL_Destroy();
public:
bool                    ImGui_Create();
void                    ImGui_Destroy();
void                    ImGui_Draw();

void                    ImGui_SendWH(ImVec2 Size);
public: // imgui
GLuint                  g_FontTexture = 0;

ImFont*                 fontBig = nullptr;
ImFont*                 fontNormal = nullptr;

ImGuiContext*           ImGuiContext = nullptr;
void                    ImGui_ImplOpenGL2_Create(bool ResetWindow = false);
void                    ImGui_ImplOpenGL2_Destroy();
void                    ImGui_ImplOpenGL2_RenderDrawData(ImDrawData* draw_data);
bool                    ImGui_ImplOpenGL2_CreateFontsTexture();
void                    ImGui_ImplOpenGL2_DestroyFontsTexture();
*/


HANDLE                  WindowMNG_Thread_Handle;
DWORD                   WindowMNG_Thread_ID;
HANDLE                  WindowMNG_Thread_ExitEvent;
HANDLE                  WindowMNG_Thread_ExitEvent2;
HWND                    WindowMNG_Thread_WindowMessages;

void                    WindowMNG_Loop();

LRESULT WINAPI          WindowMNG_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

static DWORD WINAPI     WindowMNG_hreadStatic(CONST LPVOID lpParam) {
    WNDCLASSEX wc = { 
        sizeof(WNDCLASSEX), 
        CS_OWNDC,
        WindowMNG_WndProc, 
        0L, 
        0L, 
        VST2_Window_Instance, 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        "IMPVHDX11", 
        NULL 
    };
    RegisterClassEx(&wc);

    WindowMNG_Thread_WindowMessages = CreateWindowExA(0, "IMPVHDX11", nullptr, WS_POPUPWINDOW, 0, 0, 1, 1, NULL, NULL, wc.hInstance, NULL);

    while( true ) {
        if (WaitForSingleObject(WindowMNG_Thread_ExitEvent, 10) == WAIT_OBJECT_0) break;
        WindowMNG_Loop();
    }

    DestroyWindow(WindowMNG_Thread_WindowMessages);
    UnregisterClassA("IMPVHDX11",  VST2_Window_Instance);


    SetEvent(WindowMNG_Thread_ExitEvent2);
    return 0;
}

void                    CreateWindowManager( ) {
    /*WindowMNG_Thread_ExitEvent  = CreateEventA(NULL,FALSE,FALSE,NULL);
    WindowMNG_Thread_ExitEvent2 = CreateEventA(NULL,FALSE,FALSE,NULL);
    WindowMNG_Thread_ID         = 0;
    WindowMNG_Thread_Handle     = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(&WindowMNG_hreadStatic), nullptr, 0, &WindowMNG_Thread_ID);
    */
}

void                    DestroyWindowManager( ) {
    /*SetEvent(WindowMNG_Thread_ExitEvent);

    if (WaitForSingleObject(WindowMNG_Thread_ExitEvent2, 2000) == WAIT_TIMEOUT) TerminateThread(WindowMNG_Thread_Handle, 0 );

    CloseHandle(WindowMNG_Thread_Handle);
    CloseHandle(WindowMNG_Thread_ExitEvent);
    */

}

HWND                    WindowMNG_CreateWindow( HWND Parent ) {
    // wait init parent
    while ( WindowMNG_Thread_WindowMessages == (HWND)0 ) Sleep(1);

    HWND res = (HWND)-1;
    SendMessageA(WindowMNG_Thread_WindowMessages, (WM_USER + 21),(WPARAM)&res, 0);

    // waitt create
    while ( res == (HWND)-1 ) Sleep(1);

    if (res == 0) ErrorMessage("not create window!");

    SetTimer( res, 10, USER_TIMER_MINIMUM, NULL );

    return res;
}


void                    WindowMNG_Loop( ) {
    MSG msg;
    while (PeekMessageA(&msg, WindowMNG_Thread_WindowMessages, 0U, 0U, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

LRESULT WINAPI          WindowMNG_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    // create window extern thread
    //if (message == (WM_USER + 21))  { 
    //    HWND* Winout = (HWND*)wParam;  // PTR TO HWND
    //    *Winout = CreateWindowExA(0, "IMPVHDX11", nullptr,  
    //        WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE, 
    //        0, 0, 1, 1, WindowMNG_Thread_WindowMessages, NULL, VST2_Window_Instance, NULL);
    //    return 0;
    //}

    VST2_WINDOW *ed = reinterpret_cast<VST2_WINDOW *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (ed) {
        LRESULT Result = 0;
        if (ed->Window_WndProc(hwnd, message, wParam, lParam, Result)) return Result;
    }

    return DefWindowProcA(hwnd, message, wParam, lParam);
}


