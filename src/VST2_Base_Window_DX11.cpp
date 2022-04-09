/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "VST2_Header.h"

StaticCS    VST2_ImGuiContextCS;
LONG        VST2_WindowPlugin_Counter     = 0;
LONG        VST2_Window_Counter     = 0;
HMODULE     VST2_Window_Instance    = nullptr;
string512   cclasname;

#include "VST2_D3D11_Imp.h"
#include "VST2_WindowImGui_Imp.h"

LRESULT WINAPI          WindowMNG_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    
    VST2_WINDOW *ed = reinterpret_cast<VST2_WINDOW *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (ed) {
        LRESULT Result = 0;
        if (ed->Window_WndProc(hwnd, message, wParam, lParam, Result)) return Result;
    } else {
        if (message == WM_PAINT) { 
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 1;  
        }
    }
    return DefWindowProcA(hwnd, message, wParam, lParam);
}

void                    CreateWindowManager( ) { 
    // Get handle DLL
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR) &VST2_Window_Instance, &VST2_Window_Instance);

    //cr class
    sprintf(cclasname, "IMPVHDX11_%lld", (u64)VST2_Window_Instance);


    WNDCLASSEX wc = {  
        sizeof(WNDCLASSEX),  
        CS_GLOBALCLASS | CS_DBLCLKS,
        WindowMNG_WndProc,  
        0L,  
        0L,  
        VST2_Window_Instance,  
        NULL,  
        LoadCursorA( 0, IDC_ARROW ),  
        (HBRUSH)GetStockObject(NULL_BRUSH), 
        NULL,  
        cclasname,   
        NULL 
    };
    RegisterClassExA(&wc);
}

void                    DestroyWindowManager( ) {
    // destr
    UnregisterClassA(cclasname,  VST2_Window_Instance);
}
//================================================================
ImGuiContext*           BaseContext;
//================================================================

VST2_WINDOW::VST2_WINDOW ( AudioEffect* effect ) : AEffEditor(effect) { 
    isCreate                    = false;

    VST2_BASE*    Plugin_Base   = GetPluginBase();
    VST2_PLUGIN*  Plugin        = GetPlugin( Plugin_Base );

    Window_ERect.left           = 0;
    Window_ERect.top            = 0;
    Window_ERect.right          = Plugin->GetWidthDefault();
    Window_ERect.bottom         = Plugin->GetHeightDefault();

    if ( !g_pd3dDevice && VST2_WindowPlugin_Counter == 0) {
        // create dx11
        ImGui_ImplDX11_Create( );

        // create base context
        BaseContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(BaseContext);
        ImGui::StyleColorsDark();
        
        ImGuiStyle* style = &ImGui::GetStyle();
        style->WindowRounding = 0.0f;
        style->ChildRounding = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->FrameBorderSize = 1.0f;

        style->WindowPadding = ImVec2(0.0f, 0.0f);
        /*
        ImVec4* colors = style->Colors;
        colors[ImGuiCol_Text] =                     ImVec4Color(255, 255, 255) ;
        colors[ImGuiCol_TextDisabled] =             ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] =                 ImVec4Color(29, 29, 29);
        colors[ImGuiCol_ChildBg] =                  ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] =                  ImVec4ColorA(62, 68, 82, 250) ;
        colors[ImGuiCol_Border] =                   ImVec4ColorA(121, 121, 121, 125);
        colors[ImGuiCol_BorderShadow] =             ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] =                  ImVec4Color(43, 43, 43);
        colors[ImGuiCol_FrameBgHovered] =           ImVec4Color(66, 66, 66); //ImVec4Color(66, 150, 250);
        colors[ImGuiCol_FrameBgActive] =            ImVec4ColorA(62, 68, 82, 250) ;
        colors[ImGuiCol_TitleBg]    =               ImVec4ColorA(26, 26, 26, 255);
        colors[ImGuiCol_TitleBgActive] =            ImVec4Color(75, 75, 75);
        colors[ImGuiCol_TitleBgCollapsed]   =      ImVec4ColorA(26, 26, 26, 255);
        colors[ImGuiCol_MenuBarBg] =                ImVec4Color(84, 84, 84);
        colors[ImGuiCol_ScrollbarBg] =              ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] =            ImVec4ColorA(55, 55, 55, 204) ;
        colors[ImGuiCol_ScrollbarGrabHovered] =     ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] =      ImVec4Color(113, 0, 0);
        colors[ImGuiCol_CheckMark] =                ImVec4Color(255, 255, 255) ;
        colors[ImGuiCol_SliderGrab] =               ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        colors[ImGuiCol_SliderGrabActive] =         ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
        colors[ImGuiCol_Button] =                   ImVec4ColorA(131, 131, 131, 102);
        colors[ImGuiCol_ButtonHovered] =            ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive] =             ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        colors[ImGuiCol_Header] =                   ImVec4ColorA(255, 255, 255, 79);
        colors[ImGuiCol_HeaderHovered] =            ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] =             ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] =                ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] =         ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive] =          ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip] =               ImVec4(0.80f, 0.80f, 0.80f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered] =        ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] =         ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_PlotLines] =                ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] =         ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] =            ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] =     ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] =           ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] =           ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_NavWindowingHighlight] =    ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] =        ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] =         ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
        */


        BaseContext->IO.Fonts->AddFontDefault();

        // Load resources
        Plugin->CreateUI();

        BaseContext->IO.Fonts->Build();
        
        // special initiation
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    VST2_WindowPlugin_Counter++;

    isCreate                    = true; // current not used  create flag
}

VST2_WINDOW::~VST2_WINDOW ( ) { 
    VST2_WindowPlugin_Counter--;

    if ( g_pd3dDevice && VST2_WindowPlugin_Counter == 0) {
        VST2_PLUGIN*  Plugin        = GetPlugin(  );

        if (Plugin) Plugin->DestroyUI();
        
        // destroy dx11
        ImGui_ImplDX11_Destroy( );
        
        // destroy imgui
        ImGui::SetCurrentContext(BaseContext);
        ImGui::DestroyContext(BaseContext);//11
    }

}

void                    VST2_WINDOW::Window_Open( HWND Parent ) {
    VST2_Window_Counter++;

    Window_HWND = CreateWindowExA( 0, cclasname, nullptr,  WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
                                   0, 0, Window_ERect.right - Window_ERect.left, Window_ERect.bottom - Window_ERect.top, 
                                   Parent, NULL,  VST2_Window_Instance, NULL
    );
    if (!Window_HWND) ErrorMessage("CreateWindowExA failed!");

    SetWindowLongPtrW(Window_HWND, GWLP_USERDATA, (LONG_PTR)this);

    // create swap chain
    IDXGIDevice * dxgiDevice = 0;
    HRESULT hr = g_pd3dDevice->QueryInterface( __uuidof( IDXGIDevice ),( void ** ) & dxgiDevice );
    if ( SUCCEEDED( hr ) ) {
        IDXGIAdapter * dxgiAdapter = 0;
        hr = dxgiDevice->GetParent( __uuidof( IDXGIAdapter ),( void ** ) & dxgiAdapter );
        if ( SUCCEEDED( hr ) ) {
            IDXGIFactory * dxgiFactory = 0;
            hr = dxgiAdapter->GetParent( __uuidof( IDXGIFactory ),( void ** ) & dxgiFactory );
            if ( SUCCEEDED( hr ) ) {
                //==============
                DXGI_SWAP_CHAIN_DESC sd;
                ZeroMemory(&sd, sizeof(sd));
                sd.BufferCount = 2;
                sd.BufferDesc.Width = 0;
                sd.BufferDesc.Height = 0;
                sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UNORM;  //  
                sd.Flags = 0 ; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE(DXGI_FORMAT_B8G8R8A8_UNORM)
                sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                sd.OutputWindow = Window_HWND;
                sd.SampleDesc.Count = 1;
                sd.SampleDesc.Quality = 0;
                sd.Windowed = TRUE;
                sd.SwapEffect =  DXGI_SWAP_EFFECT_DISCARD; 

                if (dxgiFactory->CreateSwapChain(dxgiDevice, &sd, &g_pSwapChain) != S_OK) {
                    ErrorMessage("CreateSwapChain failed!");
                    return ;
                }

                ID3D11Texture2D* pBackBuffer;
                g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
                g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
                pBackBuffer->Release();


                D3D11_TEXTURE2D_DESC textureDesc;
                pBackBuffer->GetDesc(&textureDesc);

                W = textureDesc.Width;
                H =  textureDesc.Height;

                textureDesc.Width  *= 2;
                textureDesc.Height *= 2;
                textureDesc.SampleDesc.Count    = 1;
                textureDesc.SampleDesc.Quality  = 0;
                textureDesc.Usage       = D3D11_USAGE_DEFAULT;
                textureDesc.BindFlags   = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
                textureDesc.MiscFlags   = 0;
                textureDesc.CPUAccessFlags  = 0;

                HRESULT hr = g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &SSAA_RT);
                if (hr != S_OK) {
                    ErrorMessage("CreateTexture2D RT failed!");
                    return ;
                }

                hr = g_pd3dDevice->CreateRenderTargetView(SSAA_RT, nullptr, &SSAA_RT_View);
                if (hr != S_OK) {
                    ErrorMessage("CreateRenderTargetView RTfailed!");
                    return ;
                }

                hr = g_pd3dDevice->CreateShaderResourceView(SSAA_RT, nullptr, &SSAA_RT_Resource);
                if (hr != S_OK) {
                    ErrorMessage("CreateShaderResourceView RT failed!");
                    return ;
                }
                //==============
                dxgiFactory->Release();
            } else {
                ErrorMessage(" failed!");
                return ;
            }
            dxgiAdapter->Release();
        } else {
            ErrorMessage(" failed!");
            return ;
        }
        dxgiDevice->Release();
    } else {
        ErrorMessage(" failed!");
        return ;
    }

    // begin draw 
    SetTimer( Window_HWND, 10, USER_TIMER_MINIMUM, NULL );

    
    WinContext          = ImGui::CreateContext();
    ImGui::SetCurrentContext(WinContext);

    SaveFont            = WinContext->Font;
    SaveFonts           = WinContext->IO.Fonts;
    WinContext->Font    = BaseContext->Font;
    WinContext->IO.Fonts    = BaseContext->IO.Fonts;
    WinContext->Style = BaseContext->Style;
    //WinContext          = BaseContext;

    ImGui_ImplWin32_Init(Window_HWND, WinContext);

    VST2_PLUGIN* Plugin        = GetPlugin();
    Plugin->Plugin_Window = this; // save window for client
}

void                    VST2_WINDOW::Window_Close( ) {
    ImGui::SetCurrentContext(WinContext);
    WinContext->Font = SaveFont;
    WinContext->IO.Fonts = SaveFonts;
    ImGui::DestroyContext(WinContext);
    
    ImGui_ImplWin32_Shutdown();
    

    //==============
    if (SSAA_RT_Resource) { SSAA_RT_Resource->Release(); SSAA_RT_Resource = NULL; }
    if (SSAA_RT_View) { SSAA_RT_View->Release(); SSAA_RT_View = NULL; }
    if (SSAA_RT) { SSAA_RT->Release(); SSAA_RT = NULL; }
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }

    DestroyWindow(Window_HWND); // studio auto release window

    VST2_Window_Counter--;
}


bool          VST2_WINDOW::Window_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT & Result ) {
    if ( message == WM_ERASEBKGND) {
        return 1;
    }
    
    if (message == WM_SIZE) { 
        VST2_ImGuiContextCS.Lock();
        
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }

            W = (UINT)LOWORD(lParam);
            H = (UINT)HIWORD(lParam);

            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);

            ID3D11Texture2D* pBackBuffer;
            g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
            pBackBuffer->Release();

            //
            if (SSAA_RT_Resource) { SSAA_RT_Resource->Release(); SSAA_RT_Resource = NULL; }
            if (SSAA_RT_View) { SSAA_RT_View->Release(); SSAA_RT_View = NULL; }
            if (SSAA_RT) { SSAA_RT->Release(); SSAA_RT = NULL; }

            D3D11_TEXTURE2D_DESC textureDesc;
            pBackBuffer->GetDesc(&textureDesc);
            textureDesc.Width  *= 2;
            textureDesc.Height *= 2;
            textureDesc.SampleDesc.Count    = 1;
            textureDesc.SampleDesc.Quality  = 0;
            textureDesc.Usage       = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags   = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            textureDesc.MiscFlags   = 0;
            textureDesc.CPUAccessFlags  = 0;

            HRESULT hr = g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &SSAA_RT);
            if (hr != S_OK) {
                ErrorMessage("CreateTexture2D RT failed!");
                VST2_ImGuiContextCS.Unlock(); return false;
            }

            hr = g_pd3dDevice->CreateRenderTargetView(SSAA_RT, nullptr, &SSAA_RT_View);
            if (hr != S_OK) {
                ErrorMessage("CreateRenderTargetView RTfailed!");
                VST2_ImGuiContextCS.Unlock(); return false;
            }

            hr = g_pd3dDevice->CreateShaderResourceView(SSAA_RT, nullptr, &SSAA_RT_Resource);
            if (hr != S_OK) {
                ErrorMessage("CreateShaderResourceView RT failed!");
                VST2_ImGuiContextCS.Unlock(); return false;
            }
        }

        VST2_ImGuiContextCS.Unlock();

        Result = 0; 
        return true;  
    }

    if ( message == WM_TIMER ) {
        Draw1( );
        //RECT rc;
        //GetClientRect(hwnd, &rc);
        //InvalidateRect(hwnd, &rc, TRUE);
        //RedrawWindow( hwnd, &rc, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_INTERNALPAINT ); // RDW_INVALIDATE | RDW_INTERNALPAINT
        //RedrawWindow( (HWND)systemWindow, NULL, NULL, RDW_FRAME | RDW_INVALIDATE );
        //InvalidateRect(GetParent((HWND)systemWindow), NULL, TRUE);
        return true;  
    }

	/**********************************************
	* WM_POPOUTPROCMSG - 0xF5XX
	*
	* HIBYTE - message id
	* LOBYTE - value
	*
	* if pop out value is 0 - proc completed
	**********************************************/
	if (HIBYTE(message) == 0xF5) {
		PopOutValue = min(LOBYTE(message) + 1, 255);
	}   
    
    if (message == WM_SETFOCUS) {
		return 0;
	}

    if (message == WM_PAINT) { 
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        Result = 1; 
        return true;  
    }

    //
    if (WinContext) 
        Result = ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam, WinContext);


    //
    if (message == WM_USER + 10 && GetPluginBase())  { // ui editorbegin
        GetPluginBase()->beginEdit (wParam);
        return 1;
    }

    if (message == WM_USER + 11 && GetPluginBase())  { // ui editorend
        GetPluginBase()->endEdit (wParam);
        return 1;
    }

    if (message == WM_USER + 12 && GetPluginBase())  { // ui setParameterAutomated
        GetPluginBase()->setParameterAutomated (wParam, *((float*)lParam));
        return 1;
    }



#ifdef PUB_RELEASE
#ifdef USE_INTRINSICS
	// Нужно для разгрузки процессора при часто юзаемом лупе. 
	// Аналог NtDelayExecution() с нулем, только в SSE2
	YieldProcessor();
#else
	Sleep(0);
#endif
#endif

    return false;
}

void                    VST2_WINDOW::ImGui_SendWH( ImVec2 Size ) {
    if (Size.x < GetPlugin()->GetWidthDefault()) return ;
    if (Size.y < GetPlugin()->GetHeightDefault()) return ;

    VST2_BASE* Base =  GetPluginBase();
    if (!Base) return ;

#ifdef PUB_RELEASE
	__nop();
#endif

    i32 X = Window_ERect.left;
    i32 Y = Window_ERect.top;
    i32 W = Window_ERect.right - Window_ERect.left;
    i32 H = Window_ERect.bottom - Window_ERect.top;
    Window_ERect.right  = X + Size.x;
    Window_ERect.bottom = Y + Size.y;

    ////SendMessageA(Window_HWND, WM_SETREDRAW, FALSE, 0);
    
    MoveWindow(Window_HWND, 0, 0, Size.x, Size.y, TRUE);

    Base->sizeWindow(Size.x, Size.y);

    ////SendMessageA(Window_HWND, WM_SETREDRAW, TRUE, 0);
}


void                    VST2_WINDOW::Draw1( ) {
    if (WinContext) {  // imgui draw
        VST2_ImGuiContextCS.Lock();

        ImGui::SetCurrentContext(WinContext);

        ImGui_ImplWin32_NewFrame(WinContext);
        ImGui::NewFrame();

        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize); 
            ImGui::Begin("Plugin", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); // | ImGuiWindowFlags_NoResize
            
            /*ImVec2 NewSize = ImGui::GetWindowSize();
            if (NewSize.x != ImGui::GetIO().DisplaySize.x || NewSize.y != ImGui::GetIO().DisplaySize.y) {
                if (NewSize.x < GetPlugin()->GetWidthDefault()) NewSize.x = GetPlugin()->GetWidthDefault();
                if (NewSize.y < GetPlugin()->GetHeightDefault()) NewSize.y = GetPlugin()->GetHeightDefault();
                ImGui::SetWindowSize(NewSize);
                ImGui_SendWH(NewSize);
            }*/

            //ImGui::Text("WinContext 0x%08x", WinContext);
            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / WinContext->IO.Framerate, WinContext->IO.Framerate);
            
            GetPlugin()->DrawUI();

            ImGui::End();
        }
        //..
        ImGui::Render();
        DrawData = ImGui::GetDrawData();

        ImGui::SetCurrentContext(BaseContext);

        VST2_ImGuiContextCS.Unlock();
    }

    if (g_pSwapChain && DrawData && g_pd3dDeviceContext && WinContext) {  // я не знаю как правельно
        const float clear_color[4] = {  1.0, 1.0, 1.0, 1.0 };

        VST2_ImGuiContextCS.Lock();

        ImGui::SetCurrentContext(WinContext);

        if (!g_pFontSampler) ImGui_ImplDX11_CreateDeviceObjects();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
        ImGui_ImplDX11_RenderDrawData(DrawData, this);

        //g_pSwapChain->Present(0, 0); 
        if (g_pSwapChain) g_pSwapChain->Present(1, 0);

        ImGui::SetCurrentContext(BaseContext);

        VST2_ImGuiContextCS.Unlock();
    }

}



VST2_BASE*              VST2_WINDOW::GetPluginBase() { 
    return (VST2_BASE*)getEffect();
}

VST2_PLUGIN*            VST2_WINDOW::GetPlugin( VST2_BASE* Base ) { 
    if (Base) return Base->Plugin; 
    return GetPluginBase()->Plugin; 
}

void                    VST2_WINDOW::idle( ) { 
    VST2_BASE*  Plugin_Base = GetPluginBase();
    if (!Plugin_Base) return ;

    VST2_PLUGIN*  Plugin = GetPlugin( Plugin_Base );
    if (!Plugin) return ;

    Plugin->EditorIdle();
}














