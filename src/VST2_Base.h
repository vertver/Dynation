#pragma once

#define     MIDIEVENT_COUNT 128

#define     REDRAWTIMERID  1
#define     REDRAWTIMER    16  // 60hz refrash

#define     DEFWINSTYLE ( WS_CHILD | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define DATAMAXCHANNELS 16

class VST2_PLUGIN;
class VST2_BASE;

#ifdef WIN32
extern HMODULE     VST2_Window_Instance;

extern ID3D11Device*               g_pd3dDevice ;
extern ID3D11DeviceContext*        g_pd3dDeviceContext ;
#endif

void        CreateWindowManager();
void        DestroyWindowManager();

class VST2_WINDOW: public AEffEditor {
public:
    VST2_WINDOW ( AudioEffect* effect );
    ~VST2_WINDOW ( );

    bool                    getRect (ERect** rect)  override { *rect = &Window_ERect;  return true;  }
    bool                    open (void* ptr)        override {  AEffEditor::open(ptr);  Window_Open((HWND)ptr); return true;  }
    void                    close ()                override { Window_Close(); AEffEditor::close();  }
    void                    idle( )                 override;

    VST2_BASE*              GetPluginBase();
    VST2_PLUGIN*            GetPlugin( VST2_BASE* Base  = nullptr);
public:
    bool                    isCreate;
    ERect                   Window_ERect;
    HWND                    Window_Parent_HWND;  
    
    HWND                    Window_HWND;  
    HWND                    Window_HWND2;  
    void                    Window_Open(HWND Parent);
    void                    Window_Close();

	void					DrawPopOutWindow();

    bool                    Window_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT & Result );

    void                    ImGui_SendWH(ImVec2 Size);
    ImFont*                 fontBig = nullptr;

public:
    // main
	i32						PopOutValue;
    i32                     W, H;

#ifdef WIN32
    IDXGISwapChain*         g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    
    ID3D11Texture2D*        SSAA_RT;
    ID3D11RenderTargetView* SSAA_RT_View;
    ID3D11ShaderResourceView* SSAA_RT_Resource;
    bool                    SSAA_Enable = false;
#endif

    ImGuiContext*           WinContext = nullptr;
    ImFont*                 SaveFont = nullptr;  
    ImFontAtlas*            SaveFonts = nullptr;  
    ImDrawData*             DrawData = nullptr;
   
#ifdef WIN32
    HWND                    g_hWnd = 0;
    INT64                   g_Time = 0;
    INT64                   g_TicksPerSecond = 0;
#else
	
#endif

    ImGuiMouseCursor        g_LastMouseCursor = ImGuiMouseCursor_COUNT;
    bool                    g_HasGamepad = false;
    bool                    g_WantUpdateHasGamepad = true;

    void                    Draw1();

    bool                    ImGui_ImplWin32_Init(void* hwnd, ImGuiContext* Context);
    void                    ImGui_ImplWin32_Shutdown();
    bool                    ImGui_ImplWin32_UpdateMouseCursor(ImGuiContext* Context);
    void                    ImGui_ImplWin32_UpdateMousePos(ImGuiContext* Context);
    void                    ImGui_ImplWin32_NewFrame(ImGuiContext* Context);
    LRESULT                 ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, ImGuiContext* Context);
};

class VST2_BASE : public AudioEffectX {
public:
    VST2_BASE (audioMasterCallback audioMaster);
    ~VST2_BASE (); 
    VST2_PLUGIN*    Plugin;
    VST2_WINDOW*    Plugin_Window;
    bool            Create();
    void            Destroy();
    void            suspend( )  override;
    void            resume( )  override;
    void            processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames) override;
    void            processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames) override;
    bool            getEffectName(char* name) override;
    bool            getVendorString(char* text) override;
    bool            getProductString(char* text) override;
    VstInt32        getVendorVersion() override;
    VstInt32        getChunk (void** data, bool isPreset) override;
    VstInt32        setChunk (void* data, VstInt32 byteSize, bool isPreset) override;
    void            setSampleRate( float sampleRate ) override;
    void            setBlockSize (VstInt32 blockSize) override;
    void            setProgram( VstInt32 program ) override; 
    void            setProgramName( char* name ) override;
    void            getProgramName( char* name ) override;
    bool            getProgramNameIndexed( VstInt32 category, VstInt32 index, char* text ) override;
    void            setParameter( VstInt32 index, float value ) override;
    float           getParameter( VstInt32 index ) override;
    void            getParameterLabel( VstInt32 index, char* label ) override;
    void            getParameterDisplay( VstInt32 index, char* text ) override;
    void            getParameterName( VstInt32 index, char* text )  override;
	bool			getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput) override;
	bool			setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput) override;
	bool            canParameterBeAutomated(VstInt32 index)   override;
	VstInt32        canDo(char* text)  override;
    VstInt32        processEvents(VstEvents* ev) override;
	VstPlugCategory getPlugCategory() override;

	VstSpeakerArrangement* InArrang;
	VstSpeakerArrangement* OutArrang;
	bool			IsOldReaper;
	bool			IsReaper;
    VstEvent        InputEventBuffer[MIDIEVENT_COUNT];
    u32             InputEventCount = 0;
    u32             InputEventPos = 0;
	u32				InVstChannels;
	u32				OutVstChannels;

    void            InputEventParse(VstEvent* Event);
};

struct PROCESS_MONO
{
	f32*	pIn;
	f32*	pOut;
	u32     SamplesCount;
	u32		CurrentChannel;
}; 

struct PROCESS_MONO_DOUBLE
{
	f64*	pIn;
	f64*	pOut;
	u32     SamplesCount;
	u32		CurrentChannel;
};

struct PROCESS_DATA {
    f32*    In[DATAMAXCHANNELS];
    f32*    Out[DATAMAXCHANNELS];
	u32		InChannelsCount;
    u32		OutChannelsCount;
    u32     SamplesCount;
};

struct PROCESS_DATA_DOUBLE {
    f64*    In[DATAMAXCHANNELS];
    f64*    Out[DATAMAXCHANNELS];
	u32		InChannelsCount;
    u32		OutChannelsCount;
    u32     SamplesCount;
};

struct GLImage {
   unsigned int Image;
    u32         Width;
    u32         Height;
};


struct UI_DIVIDE {
    ImVec2 Offset;
    ImVec2 Size;
    ImVec2 PosMin; // = Offset
    ImVec2 PosMax; // = Offset + Size
    f32    WPercent;
    f32    HPercent;
};


class VST2_PLUGIN_UI {
public: // new ui metods
	size_t GetDeltaTime()
	{
#ifdef _WIN32
		return GetTickCount64();
#else
		return 0;
#endif
	}

    ImVec2              UI_Save_MainSize;
    ImVec2              UI_Save_CursorPos;
    f32                 UI_Scale = 1.0f/2.25f;
	size_t              TimeoutIdle = 0;

    virtual void        UI_Begin( ) = 0;
    virtual void        UI_End( ) = 0;
    virtual f32         UI_BlockWAllign( UI_DIVIDE* Block, f32 Pixels) = 0;
    virtual void        UI_Divide_Width( UI_DIVIDE* Parent, u32 Elements, f32* PersentList, UI_DIVIDE* Outs ) = 0;
    virtual void        UI_Divide_Height( UI_DIVIDE* Parent, u32 Elements, f32* PersentList, UI_DIVIDE* Outs ) = 0;
    virtual void        UI_Pading( UI_DIVIDE* Parent, UI_DIVIDE* Out, f32 PadingPercent) = 0;
    virtual void        UI_DrawHeightUV_1(ImFont* Font, UI_DIVIDE* Out, u32 CountValues,  f32* Values, f32* ValuesPeak, bool* ValuesPeakTrue, ImU32 Color1,  ImU32 Color2, ImU32 Color3,  ImU32 Color4, ImU32 Color5,  ImU32 Color6 ) = 0;
    virtual void        UI_BlockWRou( UI_DIVIDE* Block, f32 Perc ) = 0;
    virtual ImFont*     LoadGlobalFontFromResources( u32 ResNum, f32 Fonst_Size = 19.0f) = 0;
    virtual void        UI_TextCentered( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen = 0 ) = 0;
    virtual void        UI_TextLeft( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen = 0 ) = 0;
    virtual void        UI_TextRight( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen = 0 ) = 0;
    virtual void        UI_CrutlkaDraw( UI_DIVIDE& Block, f32 Scale, f32 AngleMin, f32 AngleMax,  ImColor Color = ImColor(255, 255, 255), ImColor ColorBG = ImColor(0, 0, 0), ImColor ColorPoint = ImColor(0, 0, 0) ) = 0;
    virtual bool        UI_TextButton( ImFont* Font, PConstStr TextAndID, u32 TextAllign, UI_DIVIDE& Block, ImColor Color1, ImColor Color2, ImColor Color3, bool PrevState) = 0;
    virtual bool        UI_InvisibleButton(PConstStr ID, UI_DIVIDE& Block, bool* Phovered = nullptr) = 0;
    virtual bool        UI_Crutlka( PConstStr ID, i32 Index, UI_DIVIDE& Block, f32* Value, f32 DefValue, f32 Steps, bool* OldState, ImColor Color = ImColor(255, 255, 255), ImColor ColorBG = ImColor(0, 0, 0), ImColor ColorPoint = ImColor(0, 0, 0), f32 AngleMin = -120.0f, f32 AngleMax = 120.0f, char* name = nullptr, char* out = nullptr) = 0;
    virtual bool        UI_InvisibleCrutilka( PConstStr ID, i32 Index, UI_DIVIDE& Block, f32* Value, f32 DefValue,  f32 Steps, bool* OldState) = 0;




};

class VST2_PLUGIN: public VST2_PLUGIN_UI { // для будушего портирования плагинов делаем их через врапер
public:
    VST2_PLUGIN(){ }
    virtual ~VST2_PLUGIN() { }

    virtual void            SetIsAbleton(bool state) { isAbleton = state; }
    virtual bool            Create() { return true; } // для базовой подрузки конфигов пресетов и пр. без opengl и ui!!!

    virtual bool            PluginIsUI() { return true; }
    virtual bool            PluginIsSynth() { return true; }
    virtual bool            PluginIsEventSupport() { return true; } 
    virtual bool            PluginIsDoubleSupport() { return false; }

    virtual u32             GetCountPresets() { return 1; }
    virtual u32             GetCountParams() { return 1; }
    virtual i32             GetUniqueID() { return 'VSTs'; }
    virtual u32             GetInputCount() { return Plugin_Base->InVstChannels; }
    virtual u32             GetOutputCount() { return Plugin_Base->OutVstChannels;; }

    virtual PConstStr       GetName() { return "Name"; }
    virtual PConstStr       GetVendorName() { return "VendorName"; }
    virtual PConstStr       GetProductName() { return "ProductName"; }
    virtual i32             GetVersion() { return 1; }

    virtual void            Suspend() { }
    virtual void            Resume() { }

    virtual void            SetSampleRate(f32 SampleRate) { }
    virtual void            SetBlockSize (i32 BlockSize) { }
    virtual void            SetOversampling(i32 Sampling) { }
    
    virtual u32             GetPresetSize() { return sizeof(string64); }
    virtual void*           GetPresetDataCurrent() { return &presetnull[0]; }
    virtual bool            SetPresetDataCurrent(void* Buffer) { return true; }
    virtual void*           GetPresetDataArr() { return &presetnull[0]; }
    virtual bool            SetPresetDataArr(void* Buffer) { return true; }
    virtual void            SetPreset(u32 Index) { }
    virtual PStr            GetPresetNameBuffer(u32 Index) { return &presetname[Index]; }

    virtual void            GetParameterLabel8(i32 index, utf8* Buffer8) { vst_strncpy(Buffer8, "unk", kVstMaxParamStrLen); }
    virtual void            GetParametrDisplay8(i32 index, utf8* Buffer8) {  vst_strncpy(Buffer8, "unk", kVstMaxParamStrLen);   }
    virtual void            GetParametrName8(i32 index, utf8* Buffer8) {  vst_strncpy(Buffer8, "unk", kVstMaxParamStrLen);   }
    virtual bool            GetParametrIsAutomated(i32 index) { return true; }
    virtual void            SetParametr(i32 index, f32 Value) { }
    virtual f32             GetParametr(i32 index) { return 0.0f; }


	virtual void			ProcessChannel(PROCESS_MONO* data) { }
	virtual void			ProcessChannelDouble(PROCESS_MONO_DOUBLE* data) { }
    virtual void            Process(PROCESS_DATA* data) { }
    virtual void            ProcessDouble(PROCESS_DATA_DOUBLE* data) { }
    
    virtual void            NoteOn(i32 Note, f32 Vel) { }
    virtual void            NoteOff(i32 Note) { }
    virtual void            NoteOffAll() { }
    virtual void            PitchBend(f32 Pitch) { }

    // ui
    virtual i32             GetWidthDefault() { return 800; };
    virtual i32             GetHeightDefault() { return 660; };
    virtual i32             GetMaxWidthDefault() { return 800; };
    virtual i32             GetMaxHeightDefault() { return 660; };
    virtual PConstStr       GetWindowName() { return "noplugin"; };

    virtual bool            CreateUI() { return true; } // Init Global resources
    virtual void            DestroyUI() { }             // Init Global resources
    virtual void            DrawUI() { ImGui::Text("Not set plugin"); } 

    virtual void            EditorIdle() { }

    bool                    isAbleton = false;
    string64                presetname = {0}; 
    string64                presetnull = {0}; 
    VST2_BASE*              Plugin_Base; // сылка на базовый клас
    VST2_WINDOW*            Plugin_Window; // сылка на базовый клас


public:
    virtual void        UI_Begin( );
    virtual void        UI_End( );
    virtual f32         UI_BlockWAllign( UI_DIVIDE* Block, f32 Pixels);
    virtual void        UI_Divide_Width( UI_DIVIDE* Parent, u32 Elements, f32* PersentList, UI_DIVIDE* Outs );
    virtual void        UI_Divide_Height( UI_DIVIDE* Parent, u32 Elements, f32* PersentList, UI_DIVIDE* Outs );
    virtual void        UI_Pading( UI_DIVIDE* Parent, UI_DIVIDE* Out, f32 PadingPercent);
    virtual void        UI_DrawHeightUV_1(ImFont* Font, UI_DIVIDE* Out, u32 CountValues,  f32* Values, f32* ValuesPeak, bool* ValuesPeakTrue, ImU32 Color1,  ImU32 Color2, ImU32 Color3,  ImU32 Color4, ImU32 Color5,  ImU32 Color6 );
    virtual void        UI_BlockWRou( UI_DIVIDE* Block, f32 Perc );
    virtual ImFont*     LoadGlobalFontFromResources( u32 ResNum, f32 Fonst_Size = 19.0f);
    virtual void        UI_TextCentered( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen = 0 );
    virtual void        UI_TextCenteredLogo(ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen = 0);
    virtual void        UI_TextLeft( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen = 0 );
    virtual void        UI_TextRight( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen = 0 );
    virtual void        UI_CrutlkaDraw( UI_DIVIDE& Block, f32 Scale, f32 AngleMin, f32 AngleMax,  ImColor Color = ImColor(255, 255, 255), ImColor ColorBG = ImColor(0, 0, 0), ImColor ColorPoint = ImColor(0, 0, 0) );
    virtual bool        UI_TextButton( ImFont* Font, PConstStr TextAndID, u32 TextAllign, UI_DIVIDE& Block, ImColor Color1, ImColor Color2, ImColor Color3, bool PrevState);
    virtual bool        UI_InvisibleButton(PConstStr ID, UI_DIVIDE& Block, bool* Phovered = nullptr);
    virtual bool        UI_Crutlka( PConstStr ID, i32 Index, UI_DIVIDE& Block, f32* Value, f32 DefValue, f32 Steps, bool* OldState, ImColor Color = ImColor(255, 255, 255), ImColor ColorBG = ImColor(0, 0, 0), ImColor ColorPoint = ImColor(0, 0, 0), f32 AngleMin = -120.0f, f32 AngleMax = 120.0f, char* name = nullptr, char* out = nullptr);
    virtual bool        UI_InvisibleCrutilka( PConstStr ID, i32 Index, UI_DIVIDE& Block, f32* Value, f32 DefValue,  f32 Steps, bool* OldState);


public:
    void        UpdateForceOneParameter(u32 Index, f32* Value);

};


extern VST2_PLUGIN* CreatePlugin( ); // функция создания экземпляра плагина
