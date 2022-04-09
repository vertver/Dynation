#pragma once

namespace ImGui {
    bool KnobFloat(const char* label, float* p_value, float v_min, float v_max,float v_step, float radius_outer = 20.0f);

}

#define   SynthUI_Width (100)
#define   SynthUI_Width2 (70)

#define   SynthUI_ID_AUTO    -1
#define   SynthUI_blockpadding    2


#define   SynthUI_ADSR_LOG_MIN                  -4.605170     // log (0.01);
#define   SynthUI_ADSR_LOG_MAX                  9.21034       // log (10000);
#define   SynthUI_ADSR_LOG_RAGE                 ((SynthUI_ADSR_LOG_MAX - SynthUI_ADSR_LOG_MIN)/(1.0 - 0.0))
// сделал не экспонентой такк она слишком быстрая а просто 3 степенью
#define   SynthUI_ADSR_LOG_TO_LINE(POS_0_1)     (f32)(POS_0_1*POS_0_1*POS_0_1*0.99*10000) // (exp(SynthUI_ADSR_LOG_MIN + SynthUI_ADSR_LOG_RAGE*POS_0_1) )
#define   SynthUI_ADSR_LINE_TO_LOG_(inval)      (f32)(pow(inval/10000.0/0.99, 1.0/3.0)) // ((log(inval) -  SynthUI_ADSR_LOG_MIN) /  SynthUI_ADSR_LOG_RAGE)
#define   SynthUI_ADSR_SLIDER_H                 130
#define   SynthUI_ADSR_SLIDER_W                 18


i32         SynthUI_ID(PConstStr Name, u32 Adduid);

//  блоки с разными содержимым
void        SynthUI_Block_Begin(); 
void        SynthUI_Block_End(bool border=true);
void        SynthUI_Block_Column();//следущий блок в строку
void        SynthUI_Block_Line();//следущий блок  с новой линни

bool        SynthUI_ButtonCheck(PConstStr Name, i32 ID, bool* State);
bool        SynthUI_VSlider(PConstStr Name, i32 ID, f32* State, f32 min=0.0, f32 max=1.0);


bool        SynthUI_ADSR_Logarithmic(PConstStr Name, i32 ID, f32* A, f32* D, f32* S, f32* R);

bool        SynthUI_FILTER(PConstStr Name, i32 ID, u32* typewave, f32* cutoff, f32* reson, bool* srez24db);


bool        SynthUI_OptionsUp(PConstStr Name, i32 ID, u32* Polyphone, u32* Voices_);  //todooooos

bool        SynthUI_OSC(PConstStr Name, i32 ID, 
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
);

