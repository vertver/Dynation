
#include "VST2_Header.h"

u32     String_ElfHash(PConstStr s){
    u32 h = 0;
    u32 high;
    while ( *s ) {
        h = ( h << 4 ) + *s++;
        if (high = h & 0xF0000000) h ^= high >> 24;
        h &= ~high;
    }
    return h;
}


void        SynthUI_Block_Begin(){
    ImGui::BeginGroup();
    ImGui::Dummy(ImVec2(0, SynthUI_blockpadding));
    ImGui::Dummy(ImVec2(SynthUI_blockpadding/2, 0));
    ImGui::SameLine();
    ImGui::BeginGroup();
}

void        SynthUI_Block_End(bool border){
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(SynthUI_blockpadding/2, 0));
    ImGui::Dummy(ImVec2(0, SynthUI_blockpadding));
    ImGui::EndGroup();
    if (border) ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0, 255));
}

void        SynthUI_Block_Column(){
    ImGui::SameLine();
}

void        SynthUI_Block_Line(){
    ImGui::NewLine();
}


bool        SynthUI_ButtonCheck(PConstStr Name, i32 ID, bool* State){
    ImGui::PushID(ID);
    bool res = ImGui::Checkbox(Name, State);
    ImGui::PopID();
    return  res;
}


bool        SynthUI_VSlider(PConstStr Name, i32 ID, f32* State, f32 min, f32 max){
    ImGui::PushID(ID);
    bool res = ImGui::VSliderFloat("##v", ImVec2(SynthUI_ADSR_SLIDER_W,SynthUI_ADSR_SLIDER_H), State, min, max, "");
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())  ImGui::SetTooltip("%.3f", *State);
    ImGui::PopID();
    return  res;
}



i32         SynthUI_ID(PConstStr Name, u32 Adduid){
    return String_ElfHash(Name) + Adduid;
}


bool        SynthUI_ADSR_Logarithmic(PConstStr Name, i32 ID, f32* A, f32* D, f32* S, f32* R){
    bool res  = false;

    SynthUI_Block_Begin();

    i32 ID_A = SynthUI_ID(Name, 1) + ID;
    i32 ID_D = ID_A + 1;
    i32 ID_S = ID_A + 2;
    i32 ID_R = ID_A + 3;

    f32 TempValue;

    ImGui::Text("%s", Name);

    ImGui::BeginGroup();

    ImGui::PushID(ID_A);
    TempValue = SynthUI_ADSR_LINE_TO_LOG_(*A);

    ImGui::BeginGroup();
    res |= ImGui::VSliderFloat("##v", ImVec2(SynthUI_ADSR_SLIDER_W,SynthUI_ADSR_SLIDER_H), &TempValue, 0.0, 1.0, "");
    ImGui::Text("  A");
    ImGui::EndGroup();

    *A = SynthUI_ADSR_LOG_TO_LINE(TempValue);
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())  ImGui::SetTooltip("%.2f ms", *A);
    ImGui::PopID();

    ImGui::SameLine();


    ImGui::PushID(ID_D);
    TempValue = SynthUI_ADSR_LINE_TO_LOG_(*D);

    ImGui::BeginGroup();
    res |= ImGui::VSliderFloat("##v", ImVec2(SynthUI_ADSR_SLIDER_W,SynthUI_ADSR_SLIDER_H), &TempValue, 0.0, 1.0, "");
    ImGui::Text("  D");
    ImGui::EndGroup();

    *D = SynthUI_ADSR_LOG_TO_LINE(TempValue);
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())  ImGui::SetTooltip("%.2f ms", *D);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID(ID_S);

    ImGui::BeginGroup();
    res |= ImGui::VSliderFloat("##v", ImVec2(SynthUI_ADSR_SLIDER_W,SynthUI_ADSR_SLIDER_H), S, 0.0, 1.0, "");
    ImGui::Text("  S");
    ImGui::EndGroup();

    if (ImGui::IsItemActive() || ImGui::IsItemHovered())  ImGui::SetTooltip("%.2f", *S);
    ImGui::PopID();

    ImGui::SameLine();


    ImGui::PushID(ID_R);
    TempValue = SynthUI_ADSR_LINE_TO_LOG_(*R);

    ImGui::BeginGroup();
    res |= ImGui::VSliderFloat("##v", ImVec2(SynthUI_ADSR_SLIDER_W,SynthUI_ADSR_SLIDER_H), &TempValue, 0.0, 1.0, "");
    ImGui::Text("  R");
    ImGui::EndGroup();

    *R = SynthUI_ADSR_LOG_TO_LINE(TempValue);
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())  ImGui::SetTooltip("%.2f ms", *R);
    ImGui::PopID();

    ImGui::EndGroup();

    SynthUI_Block_End();

    return res;
}



bool        SynthUI_FILTER(PConstStr Name, i32 ID, u32* typewave, f32* cutoff, f32* reson, bool* srez24db){
    SynthUI_Block_Begin();
    bool res  = false;

    i32 ID_TY = SynthUI_ID(Name, 1) + ID;
    i32 ID_CU = ID_TY + 1;
    i32 ID_RE = ID_TY + 2;
    i32 ID_SR = ID_TY + 3;

    ImGui::Text("%s", Name);
    //
    ImGui::BeginGroup();

    ImGui::PushItemWidth (SynthUI_Width);
    ImGui::PushID(ID_TY);
    i32 tempty =  *typewave;
    const char* items[] = { "Disable", "Lowpass", "Highpass", "Bandpass" };
    res |= ImGui::Combo("##C", &tempty, items, IM_ARRAYSIZE(items)); 
    *typewave =   tempty;
    ImGui::PopID();
    ImGui::PopItemWidth ();
    ///ImGui::SameLine();

    ImGui::PushID(ID_CU);
    res |= ImGui::KnobFloat("Cutoff", cutoff, 0.0, 1.0, 128, 20.0f);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID(ID_RE);
    res |= ImGui::KnobFloat("Res", reson, 0.0f, 0.9f, 128, 20.0f);
    ImGui::PopID();

    ////ImGui::SameLine();

    res |= SynthUI_ButtonCheck("-24 db", ID_SR, srez24db);

    ImGui::EndGroup();
    //
    SynthUI_Block_End();

    return res;
}


bool        SynthUI_OptionsUp(PConstStr Name, i32 ID, u32* Polyphone, u32* Voices_){
    SynthUI_Block_Begin();
    bool res  = false;

    i32 ID_1 = SynthUI_ID(Name, 1) + ID;
    i32 ID_2 = ID_1 + 1;
    i32 ID_3 = ID_1 + 2;
    i32 ID_4 = ID_1 + 3;
    //
    ImGui::BeginGroup();

    ImGui::PushID(ID_1);
    i32 tempty =  *Polyphone;
    ImGui::InputInt("Polyphone", &tempty);
    *Polyphone =   tempty;
    ImGui::PopID();
    ImGui::SameLine();
    /*
    ImGui::PushID(ID_2);
    tempty =  *Voices_;
    ImGui::InputInt("Voices", &tempty);
    *Voices_ =   tempty;
    ImGui::PopID();
    */
    ImGui::EndGroup();
    //
    SynthUI_Block_End();

    return res;
}



bool        SynthUI_OSC (PConstStr Name, i32 ID,
                          i32* Main_Voices,
                          i32* Main_NoteNumberAdd,
                          i32* Main_NoteOctaveAdd,
                          i32* Main_WaveForm,
                          f32* Main_Volume,
                          f32* Main_Pan,
                          f32* Main_SideScale,
                          f32* Main_Phase,
                          f32* Main_Detune,
                          f32* Main_FinePitch,
                          bool* Main_Inverse,
                          bool* Main_Retring
)
{
    SynthUI_Block_Begin();
    bool res  = false;

    i32 ID_1 = SynthUI_ID(Name, 1) + ID;
    i32 ID_2 = ID_1 + 1;
    i32 ID_3 = ID_1 + 2;
    i32 ID_4 = ID_1 + 3;
    i32 ID_5 = ID_1 + 4;
    i32 ID_6 = ID_1 + 5;
    i32 ID_7 = ID_1 + 6;
    i32 ID_8 = ID_1 + 7;
    i32 ID_9 = ID_1 + 8;
    i32 ID_10 = ID_1 + 9;
    i32 ID_11 = ID_1 + 10;
    i32 ID_12 = ID_1 + 11;

    ImGui::Text("%s", Name);
    ImGui::BeginGroup();
    //..
    ImGui::BeginGroup();
    ImGui::PushItemWidth (SynthUI_Width2);
        ImGui::PushID(ID_1);
        res |= ImGui::InputInt("Voices", Main_Voices);
        ImGui::PopID();

        ImGui::PushID(ID_2);
        res |= ImGui::InputInt("Note", Main_NoteNumberAdd);
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(ID_3);
        res |= ImGui::InputInt("Octave", Main_NoteOctaveAdd);
        ImGui::PopID();
    ImGui::PopItemWidth ();
    ImGui::EndGroup();

    ImGui::BeginGroup();
    const char* items[] = { "Sin", "Triangle", "Saw", "Square", "Square short 1", "Square short 2", "Noise" }; 
    ImGui::PushID(ID_4);
    res |= ImGui::Combo("Combo", Main_WaveForm, items, IM_ARRAYSIZE(items)); 
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::PushID(ID_5);
    res |= ImGui::KnobFloat("Volume", Main_Volume, 0.0, 1.0, 128, 20.0f);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::PushID(ID_6);
    res |= ImGui::KnobFloat("Pan", Main_Pan, -1.0, 1.0, 128, 20.0f);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::PushID(ID_7);
    res |= ImGui::KnobFloat("Stereo", Main_SideScale, 0.0, 1.0, 128, 20.0f);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::PushID(ID_8);
    res |= ImGui::KnobFloat("Phase", Main_Phase, 0.0, 1.0, 128, 20.0f);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::PushID(ID_9);
    res |= ImGui::KnobFloat("Detune", Main_Detune, 0.0, 2.0, 128, 20.0f);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::PushID(ID_10);
    res |= ImGui::KnobFloat("Fine", Main_FinePitch, -1.0, 1.0, 128, 20.0f);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::BeginGroup();
    res |= SynthUI_ButtonCheck("Inverse", ID_11, Main_Inverse);
    ImGui::SameLine();
    res |= SynthUI_ButtonCheck("Retring", ID_12, Main_Retring);
    ImGui::EndGroup();
    //..
    ImGui::EndGroup();
    SynthUI_Block_End();

    return res;
}


bool ImGui::KnobFloat(const char* label, float* p_value, float v_min, float v_max,float v_step, float radius_outer) {
    //@ocornut https://github.com/ocornut/imgui/issues/942
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
    float line_height = ImGui::GetTextLineHeight();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float ANGLE_MIN = 3.141592f * 0.75f;
    float ANGLE_MAX = 3.141592f * 2.25f;

    ImGui::InvisibleButton(label, ImVec2(radius_outer*2, radius_outer*2 + line_height + style.ItemInnerSpacing.y));
    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();
    if (is_active && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f ))   {
        if (v_step<=0) v_step=50.f;
        float step = (v_max - v_min) / v_step;
        *p_value += io.MouseDelta.x * step;
        *p_value += -io.MouseDelta.y * step;


        if (*p_value < v_min) *p_value = v_min;
        if (*p_value > v_max) *p_value = v_max;
        value_changed = true;
    }
    else if (is_hovered && (io.MouseDoubleClicked[0] || io.MouseClicked[2])) {
        *p_value = (v_max + v_min) * 0.5f;  // reset value
        value_changed = true;
    }

    float t = (*p_value - v_min) / (v_max - v_min);
    float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t;
    float angle_cos = cosf(angle), angle_sin = sinf(angle);
    float radius_inner = radius_outer*0.0f; // 0.4

    draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
    draw_list->AddLine(ImVec2(center.x + angle_cos*radius_inner, center.y + angle_sin*radius_inner), ImVec2(center.x + angle_cos*(radius_outer-2), center.y + angle_sin*(radius_outer-2)), ImGui::GetColorU32( ImGuiCol_ButtonActive), 2.0f); // ImGuiCol_SliderGrabActive
                                                                                                                                                                                                                                              //////draw_list->AddCircleFilled(center, radius_inner, ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
    draw_list->AddText(ImVec2(pos.x, pos.y + radius_outer * 2 + style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

    if (is_active || is_hovered)    {
        ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x, pos.y - line_height - style.ItemInnerSpacing.y - style.WindowPadding.y));
        ImGui::BeginTooltip();
        ImGui::Text("%.3f", *p_value);
        ImGui::EndTooltip();
    }

    return value_changed;
}