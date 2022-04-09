/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "VST2_Header.h"

#ifdef WIN32
ImFont*    VST2_PLUGIN::LoadGlobalFontFromResources( u32 ResNum, f32 Fonst_Size) {
    HRSRC Fresource = FindResourceA( VST2_Window_Instance, MAKEINTRESOURCEA(ResNum), RT_RCDATA );
    if (!Fresource) return nullptr;

    u64 ResourceSize = SizeofResource( VST2_Window_Instance, Fresource);

    HGLOBAL ResourceGlobal =  LoadResource( VST2_Window_Instance, Fresource);

    ImFont* res = nullptr;

    if (ResourceGlobal) {

        LPVOID ResourceData = LockResource(ResourceGlobal);
        if ( ResourceData ) {  
            // normal
            void* file_data = ImGui::MemAlloc(ResourceSize);
            memcpy(file_data, ResourceData, ResourceSize);

            ImFontConfig config;
            config.OversampleH = 2; // для токо что нас будут ресайзить делаем большего размера текстуру шрифта
            config.OversampleV = 2;
            config.GlyphExtraSpacing.x = 1.0f;
            config.RasterizerMultiply = 1.0f; // делаем ярче

            res = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(file_data, ResourceSize, Fonst_Size, &config);
        }
    }
    FreeResource(ResourceGlobal);

    return res;
}
#endif

void     VST2_PLUGIN::UI_TextCentered( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen ) {
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;

    u32 Stringlen       = TextLen;
    if (Stringlen == 0) Stringlen = (u32)strlen(Text);
    PConstStr TextEnd   = Text + Stringlen;
    // auto scale
    ImGui::SetWindowFontScale(1.0); // 

    ImVec2  TextSize2    = ImGui::CalcTextSize(Text, TextEnd);

    f32 Scalebase =  min((W/TextSize2.x), (H/TextSize2.y))*0.3 ; // какого хуя коэфицент ??????/  

    ImGui::SetWindowFontScale( Scalebase); 

    ImGui::PushFont(Font);

    ImVec2  TextSize    = ImGui::CalcTextSize(Text, TextEnd);

    {
        ImU32 col = ImColor(0, 0, 0, 255);
        ImGui::PushStyleColor(ImGuiCol_Text, col);

        ImGui::SetCursorPos(ImVec2(round(X + W*0.5f) - 0.1f, round(Y + H*0.5f) - 0.1f) + ImVec2(round(-TextSize.x*0.5f) + 0.2, round(-TextSize.y*0.5f) + 0.2));
        ImGui::TextUnformatted(Text, TextEnd);

        ImGui::PopStyleColor();
    }


    ImGui::SetCursorPos(ImVec2(round(X + W*0.5f), round(Y + H*0.5f)) + ImVec2(round(-TextSize.x*0.5f), round(-TextSize.y*0.5f)));
    ImGui::TextUnformatted(Text, TextEnd);

    ImGui::PopFont();
}

void     VST2_PLUGIN::UI_TextCenteredLogo(ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen) {
    static float val = 4.5;
    static float val2 = 0.3;
	f32 X = Block.PosMin.x;
	f32 Y = Block.PosMin.y;
	f32 W = Block.Size.x;
	f32 H = Block.Size.y;

	u32 Stringlen = TextLen;
	if (Stringlen == 0) Stringlen = (u32)strlen(Text);
	PConstStr TextEnd = Text + Stringlen;
	// auto scale
	ImGui::SetWindowFontScale(1.0); // 

	ImVec2  TextSize2 = ImGui::CalcTextSize(Text, TextEnd);

	f32 Scalebase = min((W / TextSize2.x), (H / TextSize2.y)) * 0.3; // какого хуя коэфицент ??????/  

	ImGui::SetWindowFontScale(Scalebase);

	ImGui::PushFont(Font);

	ImVec2  TextSize = ImGui::CalcTextSize(Text, TextEnd);

// 	{
// 		ImU32 col = ImColor(0, 0, 0, 255);
// 		ImGui::PushStyleColor(ImGuiCol_Text, col);
// 
// 		ImGui::SetCursorPos(ImVec2(round(X + W * 0.5f) - 0.1f, round(Y + H * 0.5f) - 0.1f) + ImVec2(round(-TextSize.x * 0.5f) + 0.2, round(-TextSize.y * 0.5f) + 0.2));
// 		ImGui::TextUnformatted(Text, TextEnd);
// 
// 		ImGui::PopStyleColor();
// 	}
	ImGui::SetCursorPos(ImVec2(round(X + W * 0.5f), round(Y + H * 0.5f)) + ImVec2(round(-TextSize.x * 0.5f), round(-TextSize.y * 0.5f) /*+ (val * Scalebase)*/));
	ImGui::TextUnformatted(Text, TextEnd);

	ImGui::PopFont();
}

void    VST2_PLUGIN::UI_TextLeft( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen) {
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;

    
    u32 Stringlen       = TextLen;
    if (Stringlen == 0) Stringlen = (u32)strlen(Text);
    PConstStr TextEnd   = Text + Stringlen;
    // auto scale
    ImGui::SetWindowFontScale(1.0); // 

    ImVec2  TextSize2    = ImGui::CalcTextSize(Text, TextEnd);

    f32 Scalebase =  min((W/TextSize2.x), (H/TextSize2.y))*0.3 ; // какого хуя коэфицент ??????/  

    ImGui::SetWindowFontScale( Scalebase); 

    ImGui::PushFont(Font);

    ImVec2  TextSize    = ImGui::CalcTextSize(Text, TextEnd);
    ImGui::SetCursorPos(ImVec2(round(X), round(Y + H*0.5f)) + ImVec2(0, round(-TextSize.y*0.5f)));
    ImGui::TextUnformatted(Text, TextEnd);

    ImGui::PopFont();
}

void   VST2_PLUGIN::UI_TextRight( ImFont* Font, UI_DIVIDE& Block, PConstStr Text, u32 TextLen) {
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;

    u32 Stringlen       = TextLen;
    if (Stringlen == 0) Stringlen = (u32)strlen(Text);
    PConstStr TextEnd   = Text + Stringlen;
    // auto scale
    ImGui::SetWindowFontScale(1.0); // 

    ImVec2  TextSize2    = ImGui::CalcTextSize(Text, TextEnd);

    f32 Scalebase =  min((W/TextSize2.x), (H/TextSize2.y))*0.3 ; // какого хуя коэфицент ??????/  

    ImGui::SetWindowFontScale( Scalebase); 

    ImGui::PushFont(Font);

    ImVec2  TextSize    = ImGui::CalcTextSize(Text, TextEnd);
    ImGui::SetCursorPos(ImVec2(round(X + W), round(Y + H*0.5f)) + ImVec2(round(-TextSize.x), round(-TextSize.y*0.5f)));
    ImGui::TextUnformatted(Text, TextEnd);

    ImGui::PopFont();
}



void    VST2_PLUGIN::UI_Begin( ) {
    auto& style = ImGui::GetStyle();
    style.WindowBorderSize = 0;
    style.WindowRounding = 0.0;
    style.WindowPadding.x = 0;
    style.WindowPadding.y = 0;
    style.AntiAliasedLines = false;
    style.AntiAliasedFill = true;
    style.AntiAliasedLinesUseTex = true;
    UI_Save_MainSize    = ImGui::GetWindowSize();
    UI_Save_CursorPos   = ImGui::GetCursorPos();
}

void    VST2_PLUGIN::UI_End( ) {

}

void     VST2_PLUGIN::UI_BlockWRou( UI_DIVIDE* Block, f32 Perc) {
    f32 W  =  Block->Size.x * Perc;
    f32 H  =  Block->Size.x * Perc;
    Block->Offset.x += W;
    Block->Size.x   -= W*2.0;
    Block->Offset.y += H;
    Block->Size.y   -= H*2.0;
    Block->PosMin   = Block->Offset;
    Block->PosMax   = Block->Offset + Block->Size;
}



f32     VST2_PLUGIN::UI_BlockWAllign( UI_DIVIDE* Block, f32 Pixels ) {
    f32 newx        = (u32)(Block->Size.x / Pixels)*Pixels;
    f32 Offset      = newx - Block->Size.x;

    Block->Size.x   = newx;
    Block->PosMax   = Block->Offset + Block->Size;

    return Offset;
}


void    VST2_PLUGIN::UI_Divide_Width( UI_DIVIDE* Parent, u32 Elements, f32* PersentList, UI_DIVIDE* Outs ) {
    if ( !Parent ) {
        UI_DIVIDE DividerMain;
        DividerMain.Offset  = ImVec2(0, 0);
        DividerMain.Size    = UI_Save_MainSize;
        DividerMain.PosMin  = ImVec2(0, 0);
        DividerMain.PosMax  = UI_Save_MainSize;
        Parent = &DividerMain;
    }
    f32 PercentSum = 0.0;
    for ( u32 i = 0; i < Elements; i++ ) {
        f32 Percent = PersentList[i]; //  100%
        Outs[i].Offset  = Parent->Offset + ImVec2(round(Parent->Size.x*PercentSum), 0);
        Outs[i].Size    = ImVec2(round(Parent->Size.x*Percent), Parent->Size.y);
        Outs[i].PosMin  = Outs[i].Offset;
        Outs[i].PosMax  = Outs[i].Offset + Outs[i].Size;
        PercentSum      += Percent;
    }
}

void    VST2_PLUGIN::UI_Divide_Height( UI_DIVIDE* Parent, u32 Elements, f32* PersentList, UI_DIVIDE* Outs ) {
    if ( !Parent ) {
        UI_DIVIDE DividerMain;
        DividerMain.Offset  = ImVec2(0, 0);
        DividerMain.Size    = UI_Save_MainSize;
        DividerMain.PosMin  = ImVec2(0, 0);
        DividerMain.PosMax  = UI_Save_MainSize;
        Parent = &DividerMain;
    }
    f32 PercentSum = 0.0;
    for ( u32 i = 0; i < Elements; i++ ) {
        f32 Percent = PersentList[i]; //  100%
        Outs[i].Offset  = Parent->Offset + ImVec2(0, round(Parent->Size.y*PercentSum));
        Outs[i].Size    = ImVec2(Parent->Size.x, round(Parent->Size.y*Percent));
        Outs[i].PosMin  = Outs[i].Offset;
        Outs[i].PosMax  = Outs[i].Offset + Outs[i].Size;
        PercentSum      += Percent;
    }
}

void    VST2_PLUGIN::UI_Pading( UI_DIVIDE* Parent, UI_DIVIDE* Out, f32 PadingPercent) {
    *Out            = *Parent;
    f32 Pading      = round(min(Parent->Size.x, Parent->Size.y)*PadingPercent);
    Out->Offset     = Parent->Offset + ImVec2(Pading, Pading);
    Out->Size       = Parent->Size - ImVec2(Pading*2.0, Pading*2.0);
    Out->PosMin     = Out->Offset;
    Out->PosMax     = Out->Offset + Out->Size;
}


#include "VST2_Base_UI_UVMetter_Imp.h"

void    VST2_PLUGIN::UI_DrawHeightUV_1( ImFont* Font, UI_DIVIDE* Out, 
                                        u32 CountValues, 
                                        f32* Values, 
                                        f32* ValuesPeak, 
                                        bool* ValuesPeakTrue, 
                                        ImU32 Color1,  ImU32 Color2, 
                                        ImU32 Color3,  ImU32 Color4, 
                                        ImU32 Color5,  ImU32 Color6) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    f32 WScale = 1.0f / ((f32)CountValues);

    f32 W_Offset = Out->Size.x*WScale;
    f32 W_Size   = W_Offset*(35.0/45.0); // 90%

    f32 W_OffsetSum = Out->PosMin.x;

    for ( u32 i = 0; i < CountValues; i++ ) {
        
        f32 ValueLine  = Values[i];
        f32 ValuePLine = ValuesPeak[i];

        if (ValueLine > 1.0)  ValueLine = (ValueLine - 1.0)*0.5 + 1.0;
        if (ValuePLine > 1.0) ValuePLine = (ValuePLine - 1.0)*0.5 + 1.0;

        ValueLine   = sqrt(ValueLine);
        ValuePLine  = sqrt(ValuePLine);

        // convert linear to line 0..1
        ValueLine  = ValueLine * IMGUI_LEVELS_PART2_PERCENT;
        ValuePLine = ValuePLine * IMGUI_LEVELS_PART2_PERCENT;

        ValueLine = max(ValueLine, 0.0f);
        ValueLine = min(ValueLine, 1.0f);
        ValuePLine = max(ValuePLine, 0.0f);
        ValuePLine = min(ValuePLine, 1.0f);
            
        f32 XX      = round(W_OffsetSum);
        f32 XX_Next = round(W_OffsetSum + W_Offset);
        f32 WW_Real = XX_Next - XX - 1.0;
        f32 WW      =  XX + WW_Real;
        
        ImGui_Levels_(
            ImVec2(XX, Out->PosMin.y), 
            ImVec2(WW, Out->PosMax.y), 
            ValueLine, 
            ValuePLine, 
            ValuesPeakTrue[i],
            Color1, Color2, Color3, Color4,Color5, Color6
        );

        W_OffsetSum += W_Offset;
    }

    
    ImGui::SetCurrentFont(Font);
    // zna4enie
    ImDrawList* DrawList        = ImGui::GetWindowDrawList(); 

    ImGui::SetWindowFontScale(1.0); 
    ImVec2  TextSize2    = ImGui::CalcTextSize("-00.0", nullptr);

    f32 Scalebase =  min((Out->Size.x/TextSize2.x), (Out->Size.y/TextSize2.y)); // какого хуя коэфицент ??????/  
    
    TextSize2    = ImGui::CalcTextSize("-00.0", nullptr);

    ImGui::SetWindowFontScale( Scalebase ); 

    f32 GridLineTextLen     = 12.0f;
    f32 GridLineLen         = 8.0f;
    f32 GridLineMiniLen     = 4.0f;
    f32 GridLineHeightPart  = Out->Size.y / 20.0f;
    f32 TextSize            = DrawList->_Data->FontSize * 0.8f; 
    
    u32 AdaptiveNum         = Out->Size.y / TextSize2.y / 3.0;
    
    f32 PL = 3.0;

    for (u32 i = 0; i < 14; i++) {
        f32 Offset = pow(10, PL/20.0);
        
        if (Offset > 1.0)  Offset = (Offset - 1.0)*0.5 + 1.0;

        Offset = sqrt(Offset);

        Offset *= IMGUI_LEVELS_PART2_PERCENT;

        Offset = 1.0 - Offset;

        f32 AddY = Out->Size.y * Offset;

        string64 StrTemp;
        //    
        //f32 ValueLi = (1.0 - AddY/Out->Size.y) / IMGUI_LEVELS_PART2_PERCENT;  // (0..0.9) -> 0..1.0
        //
        //ValueLi = ValueLi * ValueLi;

        //if (ValueLi > 1.0)  ValueLi = (ValueLi - 1.0)*2.0 + 1.0;

        //if (ValueLi <= 0.001)   vst_strncpy (StrTemp, "-oo", kVstMaxParamStrLen);
        //else                    sprintf_s( StrTemp, kVstMaxParamStrLen, "%.1f", (f32)(20. * log10 (ValueLi))); 

        sprintf_s( StrTemp, kVstMaxParamStrLen, "%.0f", PL); 

        ImVec2  TextSize3    = ImGui::CalcTextSize(StrTemp, nullptr);

        DrawList->AddText(nullptr, TextSize, 
            ImVec2(Out->PosMin.x + Out->Size.x*0.5 - TextSize3.x*0.5, Out->PosMin.y + AddY - TextSize3.y*0.5),  
            ImColor(ImGui::GetStyle().Colors[ImGuiCol_Text]),  
            StrTemp, nullptr );

        if (PL > 0) PL -= 3.0;
        else {
            if (PL <= -39)  PL -= 9.0;
            if (PL <= -27)  PL -= 6.0;
            else if (PL <= -10)  PL -= 3.0;
            else PL -= 3.0;
        }
    }

    ImGui::SetCurrentFont(ImGui::GetDefaultFont());
}




void     VST2_PLUGIN::UI_CrutlkaDraw( UI_DIVIDE& Block, f32 Scale, f32 AngleMin, f32 AngleMax, ImColor Color, ImColor ColorBG, ImColor ColorPoint) {
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImGui::SetCursorPos(ImVec2(X, Y));

    f32  Radius     = min(W, H)*0.45f;
    f32  CircleW    = W/2.0;
    f32  CircleH    = H/2.0*0.95;
    f32  CircleWOfs = W*0.01;
    f32  CircleHOfs = H*0.08;

    i32 Segment = 16 + Radius*0.5;

    {
        ImU32 col = ImColor(ColorBG.Value.x, ColorBG.Value.y, ColorBG.Value.z, ColorBG.Value.w*0.25);

        ImVec2 CenterTen = ImVec2(CircleW + X + CircleWOfs, CircleH + Y + CircleHOfs);
        draw_list->AddCircleFilled( CenterTen, Radius*1.01, col, Segment);
    }

    {
        ImVec2 Center = ImVec2(CircleW + X, CircleH + Y);
        draw_list->AddCircle( Center, Radius*1.01, ColorBG, Segment, 1.0);
    }

    {
        ImVec2 Center = ImVec2(CircleW + X, CircleH + Y);
        draw_list->AddCircleFilled( Center, Radius, Color, Segment);
    }

    {
        f32 AngleF = (Scale*(AngleMax - AngleMin) + AngleMin)*M_PI/180.0;
        f32 cos_a = cosf(AngleF);
        f32 sin_a = sinf(AngleF);

        f32 XSp = 0.0; 
        f32 YSp = -0.6;

        f32 NXSp = (cos_a*XSp - sin_a*YSp)*Radius;
        f32 NYSp = (sin_a*XSp + cos_a*YSp)*Radius;

        ImVec2 Center = ImVec2(CircleW + X + NXSp, CircleH + Y + NYSp);

        draw_list->AddCircleFilled( Center, Radius*0.10, ColorPoint, Segment/2);
    }
}




bool     VST2_PLUGIN::UI_TextButton( ImFont* Font, PConstStr TextAndID, u32 TextAllign, UI_DIVIDE& Block, ImColor Color1, ImColor Color2, ImColor Color3, bool PrevState) {
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;

    u32 TextLen                 = (u32)strlen(TextAndID);
    PConstStr TextEnd           = TextAndID + TextLen;
    ImGuiContext& g             = *GImGui;
    const ImGuiStyle& style     = g.Style;
    ImGuiWindow* window         = ImGui::GetCurrentWindow();
    const ImGuiID id            = window->GetID(TextAndID, TextEnd);
    ImVec2 Size                 =  ImVec2(W, H);
    const ImRect bb(ImVec2(X, Y), ImVec2(X, Y) + Size);
    ImGui::ItemSize(Size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    const ImU32 col = ImGui::GetColorU32((held && hovered) ? (ImU32)Color3 : hovered ? (ImU32)Color2 : (PrevState ?  (ImU32)Color3 : (ImU32)Color1));

    ImGui::PushStyleColor(ImGuiCol_Text,  col);

    if (TextAllign == 0) UI_TextCentered( Font, Block, TextAndID, TextLen );
    else if (TextAllign == 1) UI_TextLeft( Font, Block, TextAndID, TextLen );
    else if (TextAllign == 2) UI_TextRight( Font, Block, TextAndID, TextLen );

    ImGui::PopStyleColor();

    return pressed;
}

bool                    VST2_PLUGIN::UI_InvisibleButton( PConstStr ID, UI_DIVIDE& Block, bool* Phovered) {
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;

    u32 TextLen                 = (u32)strlen(ID);
    PConstStr TextEnd           = ID + TextLen;
    ImGuiContext& g             = *GImGui;
    const ImGuiStyle& style     = g.Style;
    ImGuiWindow* window         = ImGui::GetCurrentWindow();
    const ImGuiID id            = window->GetID(ID, TextEnd);
    ImVec2 Size                 =  ImVec2(W, H);
    const ImRect bb(ImVec2(X, Y), ImVec2(X, Y) + Size);
    ImGui::ItemSize(Size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    if (Phovered) *Phovered = hovered;
    return pressed;
}


bool                    VST2_PLUGIN::UI_Crutlka( 
    PConstStr ID, i32 Index,
    UI_DIVIDE& Block, 
    f32* Value, f32 DefValue, 
    f32 Steps, 
    bool* OldState,
    ImColor Color, ImColor ColorBG, ImColor ColorPoint,
    f32 AngleMin, f32 AngleMax,
    char* name, 
    char* out
)
{
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;

    //
    u32 TextLen                 = (u32)strlen(ID);
    PConstStr TextEnd           = ID + TextLen;
    ImGuiContext& g             = *GImGui;
    const ImGuiStyle& style     = g.Style;
    ImGuiWindow* window         = ImGui::GetCurrentWindow();
    const ImGuiID id            = window->GetID(ID, TextEnd);
    ImVec2 Size                 =  ImVec2(W, H);
    const ImRect bb(ImVec2(X, Y), ImVec2(X, Y) + Size);
    ImGui::ItemSize(Size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    //
    UI_CrutlkaDraw( Block, *Value, AngleMin, AngleMax, Color, ColorBG, ColorPoint);
    //
    ImGuiIO&        io      = ImGui::GetIO();

    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();

    if (is_active && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f ))   {
        if (Steps <= 0.0f) Steps = 100.0f;
        f32 Step = 1.0f / Steps;
        Step *= 0.5f;
        *Value += io.MouseDelta.x * Step;
        *Value -= io.MouseDelta.y * Step;
        if (*Value < 0.0f) *Value = 0.0f;
        if (*Value > 1.0f) *Value = 1.0f;
        value_changed = true;
    } else if (is_hovered && (io.MouseDoubleClicked[0] || io.MouseClicked[2] || io.KeyAlt && io.MouseClicked[0])) {
        *Value = DefValue;  // reset value
        value_changed = true;
    }

	if (is_hovered && io.MouseWheel != 0.f) {
		f32 Step = 1.0f / Steps;
		f32 ValueBooster = io.KeyShift ? 4.f : 16.f;
		*Value += io.MouseWheel * Step * ValueBooster;
		if (*Value < 0.0f) *Value = 0.0f;
		if (*Value > 1.0f) *Value = 1.0f;
		value_changed = true;
    }

    // send automize
    if ( !( *OldState ) && is_active ) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 10, Index, 0);
        *OldState  = true;
    } 

    if (value_changed) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 12, Index, (LPARAM)Value);
    }

    if ( ( *OldState ) && !is_active ) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 11, Index, 0);
        *OldState  = false;
    }

    return value_changed;
}


bool                    VST2_PLUGIN::UI_InvisibleCrutilka(  PConstStr ID, i32 Index, UI_DIVIDE& Block,  f32* Value, f32 DefValue,  f32 Steps,  bool* OldState) 
{
    f32 X = Block.PosMin.x;
    f32 Y = Block.PosMin.y;
    f32 W = Block.Size.x;
    f32 H = Block.Size.y;
    
    //
    u32 TextLen                 = (u32)strlen(ID);
    PConstStr TextEnd           = ID + TextLen;
    ImGuiContext& g             = *GImGui;
    const ImGuiStyle& style     = g.Style;
    ImGuiWindow* window         = ImGui::GetCurrentWindow();
    const ImGuiID id            = window->GetID(ID, TextEnd);
    ImVec2 Size                 =  ImVec2(W, H);
    const ImRect bb(ImVec2(X, Y), ImVec2(X, Y) + Size);
    ImGui::ItemSize(Size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    //
    
    ImGuiIO&        io      = ImGui::GetIO();

    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();

    if (is_active && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f ))   {
        if (Steps <= 0.0f) Steps = 100.0f;
        f32 Step = 1.0f / Steps;      
        Step *= 0.5f;
        *Value += io.MouseDelta.x * Step;
        *Value -= io.MouseDelta.y * Step;
        if (*Value < 0.0f) *Value = 0.0f;
        if (*Value > 1.0f) *Value = 1.0f;
        value_changed = true;
    } else if (is_hovered && (io.MouseDoubleClicked[0] || io.MouseClicked[2])) {
        *Value = DefValue;  // reset value
        value_changed = true;
    }

	if (is_hovered && io.MouseWheel != 0.f) {
		f32 Step = 1.0f / Steps;
        f32 ValueBooster = io.KeyShift ? 4.f : 16.f;
		*Value += io.MouseWheel * Step * ValueBooster;
		if (*Value < 0.0f) *Value = 0.0f;
		if (*Value > 1.0f) *Value = 1.0f;
		value_changed = true;
	}

    if (!isAbleton) {
        // send automize
        if (!(*OldState) && is_active) {
            PostMessageW(Plugin_Window->Window_HWND, WM_USER + 10, Index, 0);
            *OldState = true;
        }

        if (value_changed) {
            PostMessageW(Plugin_Window->Window_HWND, WM_USER + 12, Index, (LPARAM)Value);
        }

        if ((*OldState) && !is_active) {
            PostMessageW(Plugin_Window->Window_HWND, WM_USER + 11, Index, 0);
            *OldState = false;
        }
    } else {
        if (!(*OldState) && is_active) {
            Plugin_Base->beginEdit(Index);
			return 1;
		}

        if ((*OldState) && !is_active) {
            Plugin_Base->endEdit(Index);
			return 1;
		}

        if (value_changed) {
            Plugin_Base->setParameterAutomated(Index, *Value);
			return 1;
		}
    }

    return value_changed;
}

void        VST2_PLUGIN::UpdateForceOneParameter(u32 Index, f32* Value) {
    Plugin_Base->setParameter(Index, *Value);
    if (!isAbleton) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 10, Index, 0);
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 11, Index, 0);
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 12, Index, (LPARAM)Value);
    } else {
        Plugin_Base->beginEdit(Index);
        Plugin_Base->setParameterAutomated(Index, *Value);
        Plugin_Base->endEdit(Index);
    }

}

/*

bool                    VST2_PLUGIN::UI_ImageKnob(f32 X, f32 Y, i32 Index, PConstStr Name, GLImage& Img, f32 MinAngle, f32 MaxAngle, f32 Steps, f32* Value, bool* OldState, f32 DefaultValue ) {
    ImGui::SetCursorPos(UI_BeginCursorPos + ImVec2(X, Y)*Ui_Scale);
    
    ImGuiIO&        io      = ImGui::GetIO();
    ImVec2          CurPos  = ImGui::GetCursorScreenPos();
    ImVec2          Size    = ImVec2(Img.Width, Img.Height)*Ui_Scale;
    ImVec2          Center  = CurPos + Size*0.5f;

    ImGui::InvisibleButton(Name, Size);

    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();

    if (is_active && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f ))   {
        if (Steps <= 0.0f) Steps = 100.0f;
        f32 Step = 1.0f / Steps;
        *Value += io.MouseDelta.x * Step;
        *Value -= io.MouseDelta.y * Step;
        if (*Value < 0.0f) *Value = 0.0f;
        if (*Value > 1.0f) *Value = 1.0f;
        value_changed = true;
    } else if (is_hovered && (io.MouseDoubleClicked[0] || io.MouseClicked[2])) {
        *Value = DefaultValue;  // reset value
        value_changed = true;
    }

    // draw rotate image
    f32 DrawAngle = (MinAngle + (MaxAngle - MinAngle)*(*Value)) * 3.14159265358979323846 / 180.f;
    ImGui::ImageRotated((ImTextureID)(intptr_t)Img.Image, Center, Size, DrawAngle);

    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();

    if (is_active && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f ))   {
    if (Steps <= 0.0f) Steps = 100.0f;
    f32 Step = 1.0f / Steps;
    *Value += io.MouseDelta.x * Step;
    *Value -= io.MouseDelta.y * Step;
    if (*Value < 0.0f) *Value = 0.0f;
    if (*Value > 1.0f) *Value = 1.0f;
    value_changed = true;
    } else if (is_hovered && (io.MouseDoubleClicked[0] || io.MouseClicked[2])) {
    *Value = DefaultValue;  // reset value
    value_changed = true;
    }

    // send automize
    if ( !( *OldState ) && is_active ) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 10, Index, 0);
        *OldState  = true;
    } 

    if (value_changed) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 12, Index, (LPARAM)Value);
    }

    if ( ( *OldState ) && !is_active ) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 11, Index, 0);
        *OldState  = false;
    } 

    return value_changed;
}

bool                    VST2_PLUGIN::UI_ImageNumberU328( f32 X, f32 Y, i32 Index, PConstStr Name, GLImage& Img, f32 MinValue, f32 MaxValue, f32 Steps, f32* Value, bool* OldState, f32 DefaultValue, u32 N , f32 UIScale2) {
    ImGui::SetCursorPos(UI_BeginCursorPos + ImVec2(X, Y)*Ui_Scale);
    // дефолтные настройки для текущего семейства плагинов с такой крутилкой
    f32 CharWidthOffset     = 3.0f;        // смещение ширины от начала текстуры 13
    f32 CharHeightOffset    = 3.0f;         // смещение высоты от начала текстуры 0 
    f32 CharWidth           = 40.0f;        // ширина символа 54
    f32 CharHeight          = (f32)Img.Height - CharHeightOffset;    // высота символа равна высоте кустуры

    f32 Udt                 = 1.0f/(f32)Img.Width;
    f32 Vdt                 = 1.0f/(f32)Img.Height;
    f32 OffsetU             = Udt*CharWidthOffset;
    f32 OffsetV             = Vdt*CharHeightOffset;
    f32 CharU               = Udt*CharWidth;       
    f32 CharV               = Vdt*CharHeight;   

    f32 CharsWidthOffset    = 0.0f*Ui_Scale*UIScale2; // смещение между символами
    f32 CharsWidth          = (f32)CharWidth*Ui_Scale*UIScale2;
    f32 CharsHeight         = (f32)CharHeight*Ui_Scale*UIScale2;

    f32 CharsWidthDrawOffset = CharsWidth + CharsWidthOffset;

    ImGuiIO&        io      = ImGui::GetIO();
    ImVec2          CurPos  = ImGui::GetCursorScreenPos();
    ImVec2          Size    = ImVec2(CharWidth*N , CharHeight)*Ui_Scale*UIScale2;
    ImVec2          Center  = CurPos + Size*0.5f;

    ImGui::InvisibleButton(Name, Size);

    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();

    if (is_active && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f ))   {
        if (Steps <= 0.0f) Steps = 100.0f;
        f32 Step = 1.0f / Steps;
        *Value += io.MouseDelta.x * Step;
        *Value -= io.MouseDelta.y * Step;
        if (*Value < 0.0f) *Value = 0.0f;
        if (*Value > 1.0f) *Value = 1.0f;
        value_changed = true;
    } else if (is_hovered && (io.MouseDoubleClicked[0] || io.MouseClicked[2])) {
        *Value = DefaultValue;  // reset value
        value_changed = true;
    }

    // draw rotate image
    i64 ValueDraw0 = (*Value*(MaxValue - MinValue) + MinValue);
    u32 ValueDraw = abs(ValueDraw0);

    u32 i = 0;
    while ( i < N ) {
        f32 CurrentChar = (ValueDraw % 10);
        ValueDraw = ValueDraw / 10;

        ImVec2 uv0 = ImVec2(OffsetU, OffsetV) + ImVec2(CharU*CurrentChar, 0); 
        ImVec2 uv1 = uv0 + ImVec2(CharU*1.0f, CharV*1.0f); 

        ImGui::SetCursorScreenPos(CurPos + ImVec2(CharsWidthDrawOffset*(N - 1 - i), 0)); 
        ImGui::Image((ImTextureID)(intptr_t)Img.Image, ImVec2(CharsWidth, CharsHeight), uv0, uv1);

        i++;

        if ( ValueDraw == 0 ) {
            if (ValueDraw0 < 0) {
                CurrentChar = 10;
                ImVec2 uv0 = ImVec2(OffsetU, OffsetV) + ImVec2(CharU*CurrentChar, 0); 
                ImVec2 uv1 = uv0 + ImVec2(CharU*1.0f, CharV*1.0f); 

                ImGui::SetCursorScreenPos(CurPos + ImVec2(CharsWidthDrawOffset*(N - 1 - i), 0)); 
                ImGui::Image((ImTextureID)(intptr_t)Img.Image, ImVec2(CharsWidth, CharsHeight), uv0, uv1);
            }
            break;
        }
    }

#ifdef UI_DEBUG_ZONE
    ImDrawList* draw_list2 = ImGui::GetWindowDrawList();
    draw_list2->AddRect(UI_BeginCursorPos + ImVec2(X, Y)*Ui_Scale, UI_BeginCursorPos + ImVec2(X + CharsWidthDrawOffset*N, Y + CharHeight)*Ui_Scale, ImColor(1.0f, 0.0f, 0.0f, 1.0f));
#endif

    // send automize
    if ( !( *OldState ) && is_active ) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 10, Index, 0);
        *OldState  = true;
    } 

    if (value_changed) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 12, Index, (LPARAM)Value);
    }

    if ( ( *OldState ) && !is_active ) {
        PostMessageW(Plugin_Window->Window_HWND, WM_USER + 11, Index, 0);
        *OldState  = false;
    } 

    return value_changed;
}














//#define UI_DEBUG_ZONE 1
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
GLuint                  VST2_PLUGIN::UI_LoadTexturePNGFromResource( u32 ResourceID, u32* GetW, u32* GetH) {
HRSRC Fresource = FindResourceA( VST2_Window_Instance, MAKEINTRESOURCEA(ResourceID), RT_RCDATA );
if (!Fresource) return 0;

u64 ResourceSize = SizeofResource( VST2_Window_Instance, Fresource);

HGLOBAL ResourceGlobal =  LoadResource( VST2_Window_Instance, Fresource);
if (!ResourceGlobal) return 0;

LPVOID ResourceData = LockResource(ResourceGlobal);
if ( !ResourceData ) {  FreeResource(ResourceGlobal); return 0; }

GLuint ImageGl = UI_LoadTexturePNGFromMemory( ResourceData, ResourceSize, GetW, GetH);

FreeResource(ResourceGlobal);

return ImageGl;
}


struct DataReso {
u8* Ptr;
u64 Pos;
u64 Size;
};

void pnglib_userReadData(png_structp pngPtr, png_bytep data, png_size_t length) {
DataReso* Res = (DataReso*)png_get_io_ptr(pngPtr);

if (Res->Pos >= Res->Size)          length = 0;
if (length > Res->Size - Res->Pos)  length = Res->Size - Res->Pos;

if (length > 0) { 
memcpy(data, Res->Ptr + Res->Pos, length);
Res->Pos += length;
}
}

GLuint                  VST2_PLUGIN::UI_LoadTexturePNGFromMemory( void* Data, u64 Size, u32* GetW, u32* GetH) {

u32 Image_Width, Image_Height;
u8* Img_Data;
bool outHasAlpha;

png_structp png_ptr;
png_infop info_ptr;
unsigned int sig_read = 0;
int color_type, interlace_type;
unsigned int row_bytes;

__try {

png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
if (png_ptr == NULL) return 0;

info_ptr = png_create_info_struct(png_ptr);
if (info_ptr == NULL) {
png_destroy_read_struct(&png_ptr, NULL, NULL);
return 0;
}

if (setjmp(png_jmpbuf(png_ptr))) {
png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
return 0;
}

DataReso Res =  { (u8*)Data, 0, Size };

png_set_read_fn( png_ptr, &Res, pnglib_userReadData);

png_set_sig_bytes(png_ptr, sig_read);

png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

png_uint_32 width, height;
int bit_depth;
png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
outHasAlpha = (color_type == PNG_COLOR_TYPE_RGBA);
Image_Width = width;
Image_Height = height;

row_bytes = png_get_rowbytes(png_ptr, info_ptr);
Img_Data = (unsigned char*) malloc(row_bytes * Image_Height);

png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

//for (int i = 0; i < Image_Height; i++)  memcpy(Img_Data + (row_bytes * (Image_Height-1-i)), row_pointers[i], row_bytes);

for (int i = 0; i < Image_Height; i++)  memcpy(Img_Data + (row_bytes * (i)), row_pointers[i], row_bytes);


png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
} __except ( EXCEPTION_EXECUTE_HANDLER ) {
return 0;
}

// Create Texture
GLuint textureID;
glGenTextures(1, &textureID);
glBindTexture(GL_TEXTURE_2D, textureID); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
glTexParameteri(GL_TEXTURE_2D,  0x8191, GL_TRUE);// GL_GENERATE_MIPMAP
glTexImage2D(GL_TEXTURE_2D, 0, outHasAlpha ? GL_RGBA : GL_RGB, Image_Width, Image_Height, 0, outHasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, Img_Data);

free(Img_Data);

if (GetW) *GetW = Image_Width;
if (GetH) *GetH = Image_Height;

return textureID;
}

void                    VST2_PLUGIN::UI_FreeTexture( GLuint Texture ) {
if (Texture == 0) return;
glDeleteTextures(1, &Texture);
}

bool                    VST2_PLUGIN::UI_LoadTexture( u32 ResourceID, GLImage& Img ) {
Img.Image = UI_LoadTexturePNGFromResource( ResourceID, &Img.Width, &Img.Height);
if (!Img.Image) return false;
return true;
}

void                    VST2_PLUGIN::UI_FreeTexture( GLImage& Img ) {
UI_FreeTexture(Img.Image);
}

void                    VST2_PLUGIN::UI_Image( f32 X, f32 Y, GLImage& Img ) {
ImGui::SetCursorPos(UI_BeginCursorPos + ImVec2(X, Y)*Ui_Scale);
ImGui::Image((ImTextureID)(intptr_t)Img.Image, ImVec2(Img.Width, Img.Height)*Ui_Scale);
}

void                    VST2_PLUGIN::UI_TextCenteredParams( i32 Index, UI_DIVIDE& Block, bool isBigFont ) {
string32 TextValueBuffer;
GetParametrDisplay8(Index, TextValueBuffer);
u8 Stringlen = strlen(TextValueBuffer);
TextValueBuffer[Stringlen] = ' ';
GetParameterLabel8(Index, &TextValueBuffer[Stringlen + 1]);
Stringlen = strlen(TextValueBuffer);

UI_TextCentered(X, Y, W, H, &TextValueBuffer[0], isBigFont, Stringlen);
}

namespace ImGui { 
void                ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle) {
ImDrawList* draw_list = ImGui::GetWindowDrawList();

float cos_a = cosf(angle);
float sin_a = sinf(angle);

ImVec2 pos[4] = {
center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
};

ImVec2 uvs[4] =  { 
ImVec2(0.0f, 0.0f), 
ImVec2(1.0f, 0.0f), 
ImVec2(1.0f, 1.0f), 
ImVec2(0.0f, 1.0f) 
};

draw_list->AddImageQuad(tex_id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);
}
}

*/
