/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "Header.h"
#include "ImGuiComboMod.h"
i32             PluginDistortion::GetWidthDefault() {
	//return 1049*NewUIScale; 
	return 2098 * NewUIScale;
};

i32             PluginDistortion::GetHeightDefault() {
	return 1252 * NewUIScale;
};

i32             PluginDistortion::GetMaxWidthDefault() {
	return GetWidthDefault() * 2.0;
}

i32             PluginDistortion::GetMaxHeightDefault() {
	return GetHeightDefault() * 2.0;
}


PConstStr       PluginDistortion::GetWindowName() {
	return GetName();
};


ImFont* Font_FuturaMediumItalic = nullptr;
ImFont* Font_RobotoLight = nullptr;
ImFont* Font_RobotoMedium = nullptr;
ImFont* Font_RobotoRegular = nullptr;
ImFont* icon_font = nullptr;

bool isReaper = false;
bool isReaperFull = false;

extern unsigned char FuturaDemiC_Italic[22088];
extern unsigned char Poppins_Medium[156480];

bool            PluginDistortion::CreateUI() {
	//memcpy(FuturaFont, FuturaDemiC_Italic, 22088);
	//memcpy(PoppinsFont, Poppins_Medium, 156480);

	ImFontConfig config;
	config.OversampleH = 2; // для токо что нас будут ресайзить делаем большего размера текстуру шрифта
	config.OversampleV = 2;
	config.GlyphExtraSpacing.x = 1.0f;
	config.RasterizerMultiply = 1.0f; // делаем ярче

	Font_FuturaMediumItalic = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(FuturaDemiC_Italic, 22088, 40.f, &config);
	icon_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(Poppins_Medium, 156480, 40.f, &config);

	string128 str; str[127] = 0;
	isReaper = Plugin_Base->getHostProductString(&str[0]) && (strcmp(str, "REAPER") == 0);

	SelectTheme_Black();
	return true;
}

void            PluginDistortion::DestroyUI() {

	Font_FuturaMediumItalic = nullptr;
	Font_RobotoLight = nullptr;
	Font_RobotoMedium = nullptr;
	Font_RobotoRegular = nullptr;
}

void            PluginDistortion::SelectTheme_Light() {
	Color_BG = ImColor(235, 235, 235, 255); // фон всего
	Color_SettingBG = ImColor(31, 31, 31, 255); // фон нижней панельки
	Color_BodyBG = ImColor(252, 252, 252, 255);  // фон основной
	Color_BodyZoneBG = ImColor(26, 26, 26, 255); // фон под выделеными учасками

	Color_ButtonResize = ImColor(59, 59, 59, 255); // цвета кнопки ресайза
	Color_ButtonResize_Hovered = ImColor(5, 109, 209, 255);
	Color_ButtonResize_Dawn = ImColor(81, 31, 209, 255);

	Color_Crutilka = ImColor(255, 255, 255);
	Color_CrutilkaBG = ImColor(100, 100, 100);
	Color_CrutilkaPoint = ImColor(0, 0, 0);

	Color_Text1 = ImColor(255, 255, 255, 255); // def color in imgui
	Color_Text2 = ImColor(6, 93, 181, 255); // цветная надпись
	Color_Text3 = ImColor(207, 207, 207, 255); // у крутилок
	Color_Text4 = ImColor(6, 93, 181, 128); //hovered

	Color_Monitor_Line1 = Color_Text3; //
	Color_Monitor_Line2 = ImColor(255, 255, 255, 255);
	Color_Monitor_Line3 = Color_Text2; //ImColor(255, 255, 255, 255); // 
	Color_Monitor_BGLine1 = ImColor(255 - 6, 255 - 93, 255 - 181, 255); // 
	Color_Monitor_BGLine2 = ImColor(6, 93, 181, 255); // 

	Color_UV_Color1 = ImColor(4, 107, 209, 255); // 
	Color_UV_Color2 = ImColor(7, 105, 209, 255); // 
	Color_UV_Color3 = ImColor(7, 105, 209, 255); // 
	Color_UV_Color4 = ImColor(83, 29, 209, 255); // 
	Color_UV_Color5 = ImColor(71, 4, 137, 255); // 
	Color_UV_Color6 = ImColor(55, 17, 137, 255); // 
	isDark = false;
}

void            PluginDistortion::SelectTheme_Black() {
	Color_BG = ImColor(26, 26, 26, 255); // фон всего
	Color_SettingBG = ImColor(31, 31, 31, 255); // фон нижней панельки
	Color_BodyBG = ImColor(34, 34, 34, 255);  // фон основной
	Color_BodyZoneBG = ImColor(25, 25, 25, 255); // фон под выделеными учасками

	Color_ButtonResize = ImColor(59, 59, 59, 255); // цвета кнопки ресайза
	Color_ButtonResize_Hovered = ImColor(103, 21, 41, 255);
	Color_ButtonResize_Dawn = ImColor(196, 20, 41, 255);

	Color_Crutilka = ImColor(40, 40, 40);
	Color_CrutilkaBG = ImColor(0, 0, 0);
	Color_CrutilkaPoint = ImColor(0, 0, 0);

	Color_Text1 = ImColor(170, 170, 170, 255); // def color in imgui
	Color_Text2 = ImColor(188, 24, 51, 255); // цветная надпись
	Color_Text3 = ImColor(154, 154, 154, 255); // у крутилок
	Color_Text4 = ImColor(188, 24, 51, 128); //hovered

	Color_Monitor_Line1 = Color_Text3;
	Color_Monitor_Line2 = ImColor(255, 255, 255, 255); // 
	Color_Monitor_Line3 = Color_Text2;
	Color_Monitor_BGLine1 = ImColor(255 - 6, 255 - 93, 255 - 181, 255); // 
	Color_Monitor_BGLine2 = ImColor(6, 93, 181, 255); // 

	Color_UV_Color1 = ImColor(103, 21, 41, 255); // 
	Color_UV_Color2 = ImColor(109, 20, 41, 255); // 
	Color_UV_Color3 = ImColor(109, 20, 41, 255); // 
	Color_UV_Color4 = ImColor(196, 20, 41, 255); // 
	Color_UV_Color5 = ImColor(130, 3, 27, 255); // 
	Color_UV_Color6 = ImColor(147, 0, 27, 255); // 
	isDark = true;
}

void            PluginDistortion::DrawUI() {

	Plugin_Window->SSAA_Enable = true; // по умолчанию всегда включено будет ибо на ноуте моем скалинг дисплея 125% и все искажается без сглаживания

	// Color preset


	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (!window->SkipItems) {
		UI_Begin();
		ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text1);

		// calc body main bloks sizes
		UI_DIVIDE       Height1Bloock[4];
		UI_DIVIDE       Width1Bloock[3];
		UI_DIVIDE       Width2Bloock[7];
		bool            HideCompessor = false;
		UI_DIVIDE       Height2Bloock[3];
		UI_DIVIDE       BaseBlock;

		f32 Height1BloockPercent[4] = { 1.91f * 0.01f, 92.5f * 0.01f, 1.754f * 0.01f, 3.827f * 0.01f };
		UI_Divide_Height(nullptr, 4, &Height1BloockPercent[0], &Height1Bloock[0]);

		f32 Width1BloockPercent[3] = { 1 * 0.01f, 98 * 0.01f, 1 * 0.01f };
		UI_Divide_Width(&Height1Bloock[1], 3, &Width1BloockPercent[0], &Width1Bloock[0]);

		f32 Width2BloockPercent[7] = { // я подрулил до целых процентов так проще
			5.0 * 0.01f,
			1.0 * 0.01f,
			37.0 * 0.01f,  // 1
			1.0 * 0.01f,
			50.0 * 0.01f,  // 2
			1.0 * 0.01f,
			5.0 * 0.01f
		};

		UI_Divide_Width(&Width1Bloock[1], 7, &Width2BloockPercent[0], &Width2Bloock[0]);

		// bkground
		draw_list->AddRectFilled(ImVec2(0, 0), UI_Save_MainSize, Color_BG);

		const f32 LowBloockH_Percent2[3] = {
			0.85,
			0.05,
			0.05
		};

		// setting low
		draw_list->AddRectFilled(Height1Bloock[3].PosMin, Height1Bloock[3].PosMax, Color_SettingBG);
		{
			ImVec2 NewSize = ImGui::GetIO().DisplaySize;

			ImU32   Resize_Color = Color_ButtonResize;
			if (true /*!isReaper*/) {
				ImGuiID Resize_id = window->GetID("Resizebutton");
				const ImRect bb(ImVec2(Height1Bloock[3].PosMax.x - Height1Bloock[3].Size.y, Height1Bloock[3].PosMin.y), Height1Bloock[3].PosMax);
				ImGui::ItemSize(ImVec2(Height1Bloock[3].Size.y, Height1Bloock[3].Size.y), 0);
				if (ImGui::ItemAdd(bb, Resize_id)) {
					bool hovered, held;
					bool pressed = ImGui::ButtonBehavior(bb, Resize_id, &hovered, &held, 0);
					if (hovered) {
						Resize_Color = Color_ButtonResize_Hovered;
						ResizeStartPos = ImGui::GetIO().DisplaySize - ImGui::GetIO().MousePos;
					}
					if (held) {
						Resize_Color = Color_ButtonResize_Dawn;
						NewSize = ImGui::GetIO().MousePos + ResizeStartPos;
					}
				}

				ImVec2 p1 = ImVec2(Height1Bloock[3].PosMax.x - Height1Bloock[3].Size.y, Height1Bloock[3].PosMax.y);
				ImVec2 p2 = ImVec2(Height1Bloock[3].PosMax.x, Height1Bloock[3].PosMin.y);
				ImVec2 p3 = Height1Bloock[3].PosMax;
				draw_list->AddTriangleFilled(p1, p2, p3, Resize_Color);
			}

			if (NewSize.x != ImGui::GetIO().DisplaySize.x || NewSize.y != ImGui::GetIO().DisplaySize.y) {
				if (NewSize.x < GetWidthDefault()) NewSize.x = GetWidthDefault();
				if (NewSize.y < GetHeightDefault()) NewSize.y = GetHeightDefault();
				if (NewSize.x > GetMaxWidthDefault()) NewSize.x = GetMaxWidthDefault();
				if (NewSize.y > GetMaxHeightDefault()) NewSize.y = GetMaxHeightDefault();

				Plugin_Window->ImGui_SendWH(NewSize);
			}
		}

		UI_Divide_Height(&Height1Bloock[3], 1, (f32*)&LowBloockH_Percent2[0], &BaseBlock);
		
		{
			string128 StrBuffer = {};

			// attack 
			GetParametrDisplay8(9, StrBuffer);	
			size_t newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(9, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (attackChanged) {
				SetNewStatus(" Attack", StrBuffer);
			}


			// release
			GetParametrDisplay8(10, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(10, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (releaseChanged) {
				SetNewStatus(" Release", StrBuffer);
			}

			// ratio
			GetParametrDisplay8(11, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(11, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (thresholdChanged) {
				SetNewStatus(" Threshold", StrBuffer);
			}

			// threshold
			GetParametrDisplay8(12, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(12, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (ratioChanged) {
				SetNewStatus(" Ratio", StrBuffer);
			}

			// gain
			GetParametrDisplay8(13, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(13, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (gainChanged) {
				SetNewStatus(" Gain", StrBuffer);
			}

			// input boost
			GetParametrDisplay8(1, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(1, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (inChanged) {
				SetNewStatus(" Input", StrBuffer);
			}

			GetParametrDisplay8(2, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(2, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (outChanged) {
				SetNewStatus(" Output", StrBuffer);
			}

			GetParametrDisplay8(3, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(3, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (driveChanged) {
				SetNewStatus(" Drive", StrBuffer);
			}

			GetParametrDisplay8(6, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(6, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (tiltChanged) {
				SetNewStatus(" Tilt EQ", StrBuffer);
			}

			GetParametrDisplay8(7, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(7, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (drywetChanged) {
				SetNewStatus(" Dry/Wet", StrBuffer);
			}


			GetParametrDisplay8(4, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(4, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (samplebitChanged) {
				SetNewStatus(" Bitrate", StrBuffer);
			}


			GetParametrDisplay8(5, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(5, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (samplerateChanged) {
				SetNewStatus(" Samplerate", StrBuffer);
			}

			GetParametrDisplay8(8, StrBuffer);
			newBuf = strlen(StrBuffer) + 1;
			GetParameterLabel8(8, &StrBuffer[newBuf]);
			StrBuffer[strlen(StrBuffer)] = ' ';
			if (hardnessChanged) {
				SetNewStatus(" Hardness", StrBuffer);
			}
		}

		if (TimeoutIdle && TimeoutIdle + TimeoutPeriod < GetDeltaTime()) {
			memset(PrintString, 0, sizeof(PrintString));
			TimeoutIdle = 0;
		} else {
			UI_TextLeft(Font_FuturaMediumItalic, BaseBlock, PrintString);
		}

		// body bkground
		draw_list->AddRectFilled(Width1Bloock[1].PosMin, Width1Bloock[1].PosMax, Color_BodyBG, 10.0f);

		// main
		UI_DIVIDE Main_Bloock[9];
		const f32 Main_Bloock_Percent[9] = {
			24.0 / 1158.0,
			109.0 / 1158.0,  // logo
			24.0 / 1158.0,
			72.0 / 1158.0,  // preset zone
			542.0 / 1158.0, // body
			72.0 / 1158.0,  // mode
			24.0 / 1158.0,
			266.0 / 1158.0, // static paramers
			25.0 / 1158.0,
		};
		UI_Divide_Height(&Width2Bloock[2], 9, (f32*)&Main_Bloock_Percent[0], &Main_Bloock[0]);

		static f32 NameColorDivK[3] = { 0.135, 0.067, 0.60 };

		// logo text
		{
			draw_list->AddRectFilled(Main_Bloock[1].PosMin, Main_Bloock[1].PosMax, Color_BodyZoneBG, 10.0f);
			UI_BlockWRou(&Main_Bloock[1], 0.01);

			UI_DIVIDE NameColorDiv[3];
			UI_Divide_Width(&Main_Bloock[1], 3, &NameColorDivK[0], &NameColorDiv[0]);

			UI_TextCentered(icon_font, NameColorDiv[0], "A");

			ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text2);
			UI_TextCenteredLogo(Font_FuturaMediumItalic, NameColorDiv[2], "DYNATION");
			ImGui::PopStyleColor();

			if (UI_InvisibleButton("SettingOpen", Main_Bloock[1])) {
				//SelectTheme_Black();
				//SelectOtherTheme();
				ByPass = !ByPass;
				if (ByPass == false) {
					Resume();
				} else {
					Suspend();
				}
			}

			//draw_list->AddRect(NameColorDiv[0].PosMin, NameColorDiv[0].PosMax, Color_Text2, 1.0f);
			//draw_list->AddRect(NameColorDiv[1].PosMin, NameColorDiv[1].PosMax, Color_Text2, 1.0f);
			//draw_list->AddRect(NameColorDiv[2].PosMin, NameColorDiv[2].PosMax, Color_Text2, 1.0f);
		}


		//ImGui::SliderFloat3("slider", NameColorDivK, 0, 1);

		// preset zone 
		{
			draw_list->AddRectFilled(Main_Bloock[3].PosMin, Main_Bloock[3].PosMax, Color_BodyZoneBG, 10.0f);
			
			UI_DIVIDE BloockW[3];
			const f32 BloockW_Percent[3] = {
				0.15,
				0.7,
				0.15
			};
			UI_Divide_Width(&Main_Bloock[3], 3, (f32*)&BloockW_Percent[0], &BloockW[0]);


			bool Phovered = false;

			////if (UI_InvisibleButton("PresetSelect", BloockW[1], &Phovered))
			////{ // begin select preset
			////  // todo: startup popup seletable
			////}
			
			if (OldState_Pre)
				draw_list->AddRectFilled(BloockW[0].PosMin, BloockW[0].PosMax, Color_Text4, 10.0f, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft );

			if (OldState_Post)
				draw_list->AddRectFilled(BloockW[2].PosMin, BloockW[2].PosMax, Color_Text4, 10.0f, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);
			 

			UI_BlockWRou(&BloockW[0], 0.05);
			UI_BlockWRou(&BloockW[1], 0.005);
			UI_BlockWRou(&BloockW[2], 0.05);

			if (UI_TextButton(
				Font_FuturaMediumItalic,
				"Pre ",
				0,
				BloockW[0],
				Color_Text1, Color_Text2, Color_BodyZoneBG,
				OldState_Pre)) 
			{
				OldState_Pre = true;
				OldState_Post = false;
				Compressor_Post = false;
				CurrentPreset->Params[15] = 0.0f;
				UpdateForceOneParameter(15, &CurrentPreset->Params[15]);
			}

			if (UI_TextButton(
				Font_FuturaMediumItalic,
				"Post",
				0,
				BloockW[2],
				Color_Text1, Color_Text2, Color_BodyZoneBG,
				OldState_Post)) 
			{
				OldState_Pre = false;
				OldState_Post = true;
				Compressor_Post = true;
				CurrentPreset->Params[15] = 1.0f;
				UpdateForceOneParameter(15, &CurrentPreset->Params[15]);
			}

			if (Phovered) ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text2);
			UI_TextCentered(Font_FuturaMediumItalic, BloockW[1], CurrentPreset->Name);
			if (Phovered) ImGui::PopStyleColor();
			
			if (1) {
				bool phovered;
				bool pressed = UI_InvisibleButton("comboselectpresetbutt", BloockW[1], &phovered);

				ImGui::PushFont(Font_FuturaMediumItalic);
				{
					if (ImGui::BeginComboMod("comboselectpreset", pressed, ImVec2(BloockW[1].Offset.x, BloockW[1].Offset.y), ImVec2(BloockW[1].Size.x, BloockW[1].Size.y))) {

						bool value_changed = false;
						for (int i = 0; i < ARRAYSIZE(PresetsDefault); i++) {
							ImGui::PushID((void*)(intptr_t)i);
							const bool item_selected = (i == CurrentPresetNumber);

							const char* item_text = PresetsDefault[i].Name;
							
							if (ImGui::Selectable(item_text, item_selected)) {
								value_changed = true;
								CurrentPresetNumber = i;
							}
							if (item_selected) ImGui::SetItemDefaultFocus();
							ImGui::PopID();
						}

						ImGui::EndCombo();

						if (value_changed) {
							SetPreset(CurrentPresetNumber);
						}
					}
				}
				ImGui::SetWindowFontScale(1.0);
				ImGui::PopFont();
			}
		}

		// body 
		{
			UI_Pading(&Main_Bloock[4], &Main_Bloock[4], 0.05);

			UI_DIVIDE BloockW[3];
			const f32 BloockW_Percent[3] = {
				0.25,
				0.5,
				0.25
			};
			UI_Divide_Width(&Main_Bloock[4], 3, (f32*)&BloockW_Percent[0], &BloockW[0]);

			UI_DIVIDE BloockHLeft[3];
			UI_DIVIDE BloockHRight[3];
			UI_DIVIDE BloockHCenter[3];
			const f32 BloockH_Percent[3] = {
				1.0 / 3.0,
				1.0 / 3.0,
				1.0 / 3.0
			};

			const f32 BloockH_Percent2[3] = {
				0.05,
				0.90,
				0.05
			};

			UI_Divide_Height(&BloockW[0], 3, (f32*)&BloockH_Percent[0], &BloockHLeft[0]);
			UI_Divide_Height(&BloockW[2], 3, (f32*)&BloockH_Percent[0], &BloockHRight[0]);
			UI_Divide_Height(&BloockW[1], 3, (f32*)&BloockH_Percent2[0], &BloockHCenter[0]);

			UI_DIVIDE BloockH2[2];
			const f32 BloockH2_Percent[2] = { 158.0 / 212.0, 54.0 / 212.0 };

			ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text3);

			//
			UI_Divide_Height(&BloockHLeft[0], 2, (f32*)&BloockH2_Percent[0], &BloockH2[0]);
			UI_Pading(&BloockH2[0], &BloockH2[0], 0.02);
			tiltChanged = UI_Crutlka("Tilt_EQ_Crutilka", 6, BloockH2[0], &CurrentPreset->Params[6], 0.5, 300, &OldState_TiltEq, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint);
			if (tiltChanged) SetParametr(6, CurrentPreset->Params[6]);
			UI_TextCentered(Font_FuturaMediumItalic, BloockH2[1], "Tilt EQ");

			//
			UI_Divide_Height(&BloockHLeft[2], 2, (f32*)&BloockH2_Percent[0], &BloockH2[0]);
			UI_Pading(&BloockH2[0], &BloockH2[0], 0.02);
			inChanged = UI_Crutlka("In_Crutilka", 1, BloockH2[0], &CurrentPreset->Params[1], 0.5, 300, &OldState_In, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint);
			if (inChanged) SetParametr(1, CurrentPreset->Params[1]);
			UI_TextCentered(Font_FuturaMediumItalic, BloockH2[1], "In");

			//
			UI_Divide_Height(&BloockHRight[0], 2, (f32*)&BloockH2_Percent[0], &BloockH2[0]);
			UI_Pading(&BloockH2[0], &BloockH2[0], 0.02);
			drywetChanged = UI_Crutlka("DryWet_Crutilka", 7, BloockH2[0], &CurrentPreset->Params[7], 1.0, 300, &OldState_DryWet, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint);
			if (drywetChanged) SetParametr(7, CurrentPreset->Params[7]);
			UI_TextCentered(Font_FuturaMediumItalic, BloockH2[1], "Dry/Wet");

			//
			UI_Divide_Height(&BloockHRight[2], 2, (f32*)&BloockH2_Percent[0], &BloockH2[0]);
			UI_Pading(&BloockH2[0], &BloockH2[0], 0.02);
			outChanged = UI_Crutlka("Out_Crutilka", 2, BloockH2[0], &CurrentPreset->Params[2], 0.5, 300, &OldState_Out, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint);
			if (outChanged) SetParametr(2, CurrentPreset->Params[2]);
			UI_TextCentered(Font_FuturaMediumItalic, BloockH2[1], "Out");

			//
			UI_Divide_Height(&BloockHCenter[1], 2, (f32*)&BloockH2_Percent[0], &BloockH2[0]);
			UI_Pading(&BloockH2[0], &BloockH2[0], 0.02);
			driveChanged = UI_Crutlka("Drive_Crutilka", 3, BloockH2[0], &CurrentPreset->Params[3], 0.0, 300, &OldState_Drive, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint);
			if (driveChanged) SetParametr(3, CurrentPreset->Params[3]);
			UI_TextCentered(Font_FuturaMediumItalic, BloockH2[1], " Drive ");

			ImGui::PopStyleColor();
		}

		// mode
		{
			draw_list->AddRectFilled(Main_Bloock[5].PosMin, Main_Bloock[5].PosMax, Color_BodyZoneBG, 10.0f);

			UI_DIVIDE Mode_Bloock[3];
			const f32 Mode_Bloock_Percent[3] = {
				0.18,
				0.64,
				0.18
			};
			UI_Divide_Width(&Main_Bloock[5], 3, (f32*)&Mode_Bloock_Percent[0], &Mode_Bloock[0]);

			// draw left
			if (TypeDistortion != 0) {
				if (UI_TextButton(Font_FuturaMediumItalic, "<", 0,
					Mode_Bloock[0],
					Color_Text1, Color_Text4, Color_Text2,
					false))
				{
					TypeDistortion--;
					CurrentPreset->Params[0] = (f32)TypeDistortion / (6.0f - 1.0f);
					UpdateForceOneParameter(0, &CurrentPreset->Params[0]);
				}
			}

			// draw name
			ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text1);
			UI_TextCentered(Font_FuturaMediumItalic, Mode_Bloock[1], NameTypesDist[TypeDistortion]);
			ImGui::PopStyleColor();

			// draw right
			if (TypeDistortion != 6 - 1) {
				if (UI_TextButton(Font_FuturaMediumItalic, ">", 0,
					Mode_Bloock[2],
					Color_Text1, Color_Text4, Color_Text2,
					false))
				{
					TypeDistortion++;
					CurrentPreset->Params[0] = (f32)TypeDistortion / (ARRAYSIZE(NameTypesDist) - 1.0f);
					UpdateForceOneParameter(0, &CurrentPreset->Params[0]);
				}
			}

			// select type combo
			if (1) {
				bool phovered;
				bool pressed = UI_InvisibleButton("comboselecttypebutt", Mode_Bloock[1], &phovered);

				ImGui::PushFont(Font_FuturaMediumItalic);
				{
					if (ImGui::BeginComboMod("comboselecttype", pressed, ImVec2(Mode_Bloock[1].Offset.x, Mode_Bloock[1].Offset.y), ImVec2(Mode_Bloock[1].Size.x, Mode_Bloock[1].Size.y))) {

						bool value_changed = false;
						for (int i = 0; i < ARRAYSIZE(NameTypesDist); i++) {
							ImGui::PushID((void*)(intptr_t)i);
							const bool item_selected = (i == TypeDistortion);

							const char* item_text = NameTypesDist[i];

							if (ImGui::Selectable(item_text, item_selected)) {
								value_changed = true;
								TypeDistortion = i;
							}
							if (item_selected) ImGui::SetItemDefaultFocus();
							ImGui::PopID();
						}

						ImGui::EndCombo();

						if (value_changed) {
							CurrentPreset->Params[0] = (f32)TypeDistortion / (ARRAYSIZE(NameTypesDist) - 1.0f);
							UpdateForceOneParameter(0, &CurrentPreset->Params[0]);
						}
					}
				}
				ImGui::SetWindowFontScale(1.0);
				ImGui::PopFont();
			}
		}

		// static params
		{
			draw_list->AddRectFilled(Main_Bloock[7].PosMin, Main_Bloock[7].PosMax, Color_BodyZoneBG, 10.0f);

			UI_DIVIDE StPar_BloockW[3];
			const f32 StPar_BloockW_Percent[3] = {
				1.0 / 2.0,
				1.0 / 2.0
			};
			UI_Divide_Width(&Main_Bloock[7], 3, (f32*)&StPar_BloockW_Percent[0], &StPar_BloockW[0]);

			UI_DIVIDE StPar_Bloock[3];
			const f32 StPar_Bloock_Percent[3] = {
				1.0 / 3.0,
				1.0 / 3.0,
				1.0 / 3.0
			};

			UI_Divide_Height(&StPar_BloockW[0], 3, (f32*)&StPar_Bloock_Percent[0], &StPar_Bloock[0]);
			for (u32 i = 0; i < 3; i++)  UI_Pading(&StPar_Bloock[i], &StPar_Bloock[i], 16.0 / 105.0);

			UI_TextLeft(Font_FuturaMediumItalic, StPar_Bloock[0], "Samplerate");
			UI_TextLeft(Font_FuturaMediumItalic, StPar_Bloock[1], "Bitrate");
			UI_TextLeft(Font_FuturaMediumItalic, StPar_Bloock[2], "Hardness");

			UI_Divide_Height(&StPar_BloockW[1], 3, (f32*)&StPar_Bloock_Percent[0], &StPar_Bloock[0]);
			for (u32 i = 0; i < 3; i++)  UI_Pading(&StPar_Bloock[i], &StPar_Bloock[i], 16.0 / 105.0);


			ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text2);

			string128 StrBuffer;

			samplerateChanged = UI_InvisibleCrutilka("Samp_krtilka", 5, StPar_Bloock[0], &CurrentPreset->Params[5], 1.0, 200, &OldState_SampleRate);
			if (samplerateChanged) SetParametr(5, CurrentPreset->Params[5]);
			
			GetParametrDisplay8(5, StrBuffer);
			UI_TextRight(Font_FuturaMediumItalic, StPar_Bloock[0], StrBuffer);

			samplebitChanged = UI_InvisibleCrutilka("Bit_krtilka", 4, StPar_Bloock[1], &CurrentPreset->Params[4], 1.0, 200, &OldState_BitRate);
			if (samplebitChanged) SetParametr(4, CurrentPreset->Params[4]);
			
			GetParametrDisplay8(4, StrBuffer);
			UI_TextRight(Font_FuturaMediumItalic, StPar_Bloock[1], StrBuffer);

			hardnessChanged = UI_InvisibleCrutilka("Curv_krtilka", 8, StPar_Bloock[2], &CurrentPreset->Params[8], 0.0, 200, &OldState_Curv);
			if (hardnessChanged) SetParametr(8, CurrentPreset->Params[8]);
			
			GetParametrDisplay8(8, StrBuffer);
			UI_TextRight(Font_FuturaMediumItalic, StPar_Bloock[2], StrBuffer);

			ImGui::PopStyleColor();
		}

		//  compressor
		{
			UI_DIVIDE Compressor_Bloock[5];
			const f32 Compressor_Bloock_Percent[5] = {
				24.0 / 1158.0,
				820.0 / 1158.0, // display
				236.0 / 1158.0,  // setting zone
				54.0 / 1158.0, // body
				25.0 / 1158.0
			};
			UI_Divide_Height(&Width2Bloock[4], 5, (f32*)&Compressor_Bloock_Percent[0], &Compressor_Bloock[0]);


			if (UI_InvisibleButton("settings", Compressor_Bloock[1])) {
				DrawSettings = !DrawSettings;
			}

			// monitor zone
			if (!DrawSettings)
			{
				draw_list->AddRectFilled(Compressor_Bloock[1].PosMin, Compressor_Bloock[1].PosMax, Color_BodyZoneBG, 10.0f);

				ImGui::SetCursorPos(Compressor_Bloock[1].PosMin);

				UI_Pading(&Compressor_Bloock[1], &Compressor_Bloock[1], 0.02);

				Metter_Draw(Compressor_Bloock[1].Offset.x, Compressor_Bloock[1].Offset.y, Compressor_Bloock[1].Size.x, Compressor_Bloock[1].Size.y);

				draw_list->AddPolyline(&PoleLines_In_Vec[0], POLYLINEMETERCOUNT, Color_Monitor_Line1, false, 1.0);
				draw_list->AddPolyline(&PoleLines_Out_Vec[0], POLYLINEMETERCOUNT, Color_Monitor_Line2, false, 1.0);
				draw_list->AddPolyline(&PoleLines_Comp_Vec[0], POLYLINEMETERCOUNT, Color_Monitor_Line3, false, 1.0);

				f32 TrashLine = 1.0f - Compressor_Trashold;
				if (TrashLine < 0.0) TrashLine = 0.0f;
				if (TrashLine > 1.0) TrashLine = 1.0f;
				draw_list->AddLine(
					ImVec2(Compressor_Bloock[1].PosMin.x, Compressor_Bloock[1].PosMin.y + TrashLine * Compressor_Bloock[1].Size.y),
					ImVec2(Compressor_Bloock[1].PosMax.x, Compressor_Bloock[1].PosMin.y + TrashLine * Compressor_Bloock[1].Size.y),
					Color_Monitor_BGLine1);
			} else {
				const f32 Params_Percent1[3] = {
					0.15,
					0.7,
					0.15
				};

				UI_DIVIDE BloockW1[3];
				UI_Divide_Width(&Compressor_Bloock[1], 3, (f32*)&Params_Percent1[0], &BloockW1[0]);

				draw_list->AddRectFilled(Compressor_Bloock[1].PosMin, Compressor_Bloock[1].PosMax, Color_BodyZoneBG, 10.0f);

				ImGui::SetCursorPos(Compressor_Bloock[1].PosMin);

				UI_Pading(&Compressor_Bloock[1], &Compressor_Bloock[1], 0.02);

				const char* tt =
					"Dynation\n"
					"MIT License\n"
					"\n"
					"Copyright: Anton Kovalev \n(c) 2018-2020. All rights reserved.\n"
					"\n"
					"Developers:\n"
					"Anton Kovalev (vertver) \n"
					"   - Lead project programmer\n"
					"Vladislav Shatrov (frowrik) \n"
					"   - UI and support programmer\n"
					"Sergey Batarin (Last Day Alive) \n"
					"   - UI design and QA\n"
					"\n"
					"VST Plugin technology by Steinberg.\n"
					"\n"
					"Used libraies:\n"
					"ImGui - occornut \n(https://github.com/ocornut/imgui)\n";
				
				UI_TextCentered(Font_FuturaMediumItalic, BloockW1[1], tt);

				//ImGui::SliderFloat2("s", BloockH_Percent1, 0, 1);
			}

			// 5 + 4
			const f32 Params_Percent[9] = {
				0.15,
				0.0625,
				0.15,
				0.0625,
				0.15,
				0.0625,
				0.15,
				0.0625,
				0.15
			};

			//Params zone
			{
				UI_DIVIDE BloockW[9];
				UI_Divide_Width(&Compressor_Bloock[2], 9, (f32*)&Params_Percent[0], &BloockW[0]);

				//draw_list->AddRectFilled(Compressor_Bloock[2].PosMin + ImVec2(0, 20 * NewUIScale), Compressor_Bloock[2].PosMax - ImVec2(0, 60 * NewUIScale), Color_BodyZoneBG, 10.0f);

				ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text3);

				UI_DIVIDE BloockH[2];
				const f32 BloockH_Percent[2] = { 158.0 / 212.0, 54.0 / 212.0 };
				//
				UI_Divide_Height(&BloockW[0], 2, (f32*)&BloockH_Percent[0], &BloockH[0]);
				UI_Pading(&BloockH[0], &BloockH[0], 0.02);
				attackChanged = UI_Crutlka("Attack_Crutilka", 9, BloockH[0], &CurrentPreset->Params[9], 0.5, 300, &OldState_Release, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint, -120.f, 120.f, const_cast<char*>("Attack"), PrintString);
				if (attackChanged) SetParametr(9, CurrentPreset->Params[9]);
				UI_TextCentered(Font_FuturaMediumItalic, BloockH[1], " Attack ");
				//
				UI_Divide_Height(&BloockW[2], 2, (f32*)&BloockH_Percent[0], &BloockH[0]);
				UI_Pading(&BloockH[0], &BloockH[0], 0.02);
				releaseChanged = UI_Crutlka("Release_Crutilka", 10, BloockH[0], &CurrentPreset->Params[10], 0.5, 300, &OldState_Release, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint, -120.f, 120.f, const_cast<char*>("Release"), PrintString);
				if (releaseChanged) SetParametr(10, CurrentPreset->Params[10]);
				UI_TextCentered(Font_FuturaMediumItalic, BloockH[1], " Release ");
				//
				UI_Divide_Height(&BloockW[4], 2, (f32*)&BloockH_Percent[0], &BloockH[0]);
				UI_Pading(&BloockH[0], &BloockH[0], 0.02);
				thresholdChanged = UI_Crutlka("Trashold_Crutilka", 11, BloockH[0], &CurrentPreset->Params[11], 0.5, 300, &OldState_Release, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint, -120.f, 120.f, const_cast<char*>("Treshold"), PrintString);
				if (thresholdChanged) SetParametr(11, CurrentPreset->Params[11]);
				UI_TextCentered(Font_FuturaMediumItalic, BloockH[1], " Treshold");
				//
				UI_Divide_Height(&BloockW[6], 2, (f32*)&BloockH_Percent[0], &BloockH[0]);
				UI_Pading(&BloockH[0], &BloockH[0], 0.02);
				ratioChanged = UI_Crutlka("Ratio_Crutilka", 12, BloockH[0], &CurrentPreset->Params[12], 0.5, 300, &OldState_Release, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint, -120.f, 120.f, const_cast<char*>("Ratio"), PrintString);
				if (ratioChanged) SetParametr(12, CurrentPreset->Params[12]);
				UI_TextCentered(Font_FuturaMediumItalic, BloockH[1], "  Ratio  ");
				//
				UI_Divide_Height(&BloockW[8], 2, (f32*)&BloockH_Percent[0], &BloockH[0]);
				UI_Pading(&BloockH[0], &BloockH[0], 0.02);
				gainChanged = UI_Crutlka("Gain_Crutilka", 13, BloockH[0], &CurrentPreset->Params[13], 0.5, 300, &OldState_Release, Color_Crutilka, Color_CrutilkaBG, Color_CrutilkaPoint, -120.f, 120.f, const_cast<char*>("Gain"), PrintString);
				if (gainChanged) SetParametr(13, CurrentPreset->Params[13]);
				UI_TextCentered(Font_FuturaMediumItalic, BloockH[1], "   Gain  ");
				ImGui::PopStyleColor();
			}

			//Params value zone
			{
				draw_list->AddRectFilled(Compressor_Bloock[3].PosMin, Compressor_Bloock[3].PosMax, Color_BodyZoneBG, 10.0f);

				UI_DIVIDE BloockW[9];
				UI_Divide_Width(&Compressor_Bloock[3], 9, (f32*)&Params_Percent[0], &BloockW[0]);

				ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)Color_Text2);

				string128 StrBuffer = {};
				GetParametrDisplay8(9, StrBuffer);

				size_t newBuf = strlen(StrBuffer) + 1;

				// attack 
				GetParameterLabel8(9, &StrBuffer[newBuf]);
				StrBuffer[strlen(StrBuffer)] = ' ';

				UI_TextCentered(Font_FuturaMediumItalic, BloockW[0], StrBuffer);

				GetParametrDisplay8(10, StrBuffer);

				// release
				newBuf = strlen(StrBuffer) + 1;
				GetParameterLabel8(10, &StrBuffer[newBuf]);
				StrBuffer[strlen(StrBuffer)] = ' ';

				UI_TextCentered(Font_FuturaMediumItalic, BloockW[2], StrBuffer);

				GetParametrDisplay8(11, StrBuffer);

				// ratio
				newBuf = strlen(StrBuffer) + 1;
				GetParameterLabel8(11, &StrBuffer[newBuf]);
				StrBuffer[strlen(StrBuffer)] = ' ';

				UI_TextCentered(Font_FuturaMediumItalic, BloockW[4], StrBuffer);

				GetParametrDisplay8(12, StrBuffer);

				// threshold
				newBuf = strlen(StrBuffer) + 1;
				GetParameterLabel8(12, &StrBuffer[newBuf]);
				StrBuffer[strlen(StrBuffer)] = ' ';

				UI_TextCentered(Font_FuturaMediumItalic, BloockW[6], StrBuffer);

				GetParametrDisplay8(13, StrBuffer);

				// gain
				newBuf = strlen(StrBuffer) + 1;
				GetParameterLabel8(13, &StrBuffer[newBuf]);
				StrBuffer[strlen(StrBuffer)] = ' ';

				UI_TextCentered(Font_FuturaMediumItalic, BloockW[8], StrBuffer);

				ImGui::PopStyleColor();
			}
		}

		// uv1
		UV_In_Count = GetInputCount();
		UV_Out_Count = GetOutputCount();

		for (u32 i = 0; i < UV_In_Count; i++) {
			UV_In_Values[i] = InputEnvelops[i].GetEnvelop();
			UV_In_PeakValues[i] = InputEnvelops[i].GetEnvelopMax();
			if (UV_In_PeakValues[i] > 1.0 && UV_In_Peak[i] == false) UV_In_Peak[i] = true;
		}

		for (u32 i = 0; i < UV_Out_Count; i++) {
			UV_Out_Values[i] = OutputEnvelops[i].GetEnvelop();
			UV_Out_PeakValues[i] = OutputEnvelops[i].GetEnvelopMax();
			if (UV_Out_PeakValues[i] > 1.0 && UV_Out_Peak[i] == false) UV_Out_Peak[i] = true;
		}

		UI_DIVIDE UVIN_Block;
		UI_Pading(&Width2Bloock[0], &UVIN_Block, 11.0 / 105.0);

		draw_list->AddRectFilled(Width2Bloock[0].PosMin, Width2Bloock[0].PosMax, Color_BodyZoneBG, 10.0f);

		UI_DrawHeightUV_1(Font_FuturaMediumItalic, &UVIN_Block,
			UV_In_Count,
			&UV_In_Values[0],
			&UV_In_PeakValues[0],
			&UV_In_Peak[0],
			Color_UV_Color1, Color_UV_Color2, Color_UV_Color3, Color_UV_Color4, Color_UV_Color5, Color_UV_Color6
		);


		// uv1
		UI_DIVIDE OUTIN_Block;
		UI_Pading(&Width2Bloock[6], &OUTIN_Block, 11.0 / 105.0);

		draw_list->AddRectFilled(Width2Bloock[6].PosMin, Width2Bloock[6].PosMax, Color_BodyZoneBG, 10.0f);
		UI_DrawHeightUV_1(Font_FuturaMediumItalic, &OUTIN_Block,
			UV_Out_Count,
			&UV_Out_Values[0],
			&UV_Out_PeakValues[0],
			&UV_Out_Peak[0],
			Color_UV_Color1, Color_UV_Color2, Color_UV_Color3, Color_UV_Color4, Color_UV_Color5, Color_UV_Color6
		);

		ImGui::PopStyleColor();
		UI_End();
	}
}


void            PluginDistortion::Metter_Update(f32 LinearIn, f32 LinearOut, f32 LinearZip) {
	if (LinearIn < 0.0) LinearIn = 0.0f;
	if (LinearIn > 1.0) LinearIn = 1.0f;
	if (LinearOut < 0.0) LinearOut = 0.0f;
	if (LinearOut > 1.0) LinearOut = 1.0f;
	if (LinearZip < 0.0) LinearZip = 0.0f;
	if (LinearZip > 1.0) LinearZip = 1.0f;
	
	if (PoleLinesOffset < PoleLinesSizePerOne) {

		PoleLines_In[0]		= max(PoleLines_In[0], LinearIn);
		PoleLines_Out[0]	= max(PoleLines_Out[0], LinearOut);
		PoleLines_Comp[0]	= min(PoleLines_Comp[0], LinearZip);

		/*PoleLines_In[0]		= (PoleLines_In[0] * (f32)PoleLinesOffset +  LinearIn) / (f32)(PoleLinesOffset+1);
		PoleLines_Out[0]	= (PoleLines_Out[0] * (f32)PoleLinesOffset + LinearOut) / (f32)(PoleLinesOffset + 1);
		PoleLines_Comp[0]	= (PoleLines_Comp[0] * (f32)PoleLinesOffset + LinearZip) / (f32)(PoleLinesOffset + 1);*/

		PoleLinesOffset++;
		return;
	}

	PoleLinesOffset = 0;

	for (u32 i = POLYLINEMETERCOUNT - 1; i > 0; i--) {
		PoleLines_In[i] = PoleLines_In[i - 1];
		PoleLines_Out[i] = PoleLines_Out[i - 1];
		PoleLines_Comp[i] = PoleLines_Comp[i - 1];
	}

	if (LinearIn < 0.0) LinearIn = 0.0;
	if (LinearIn > 1.0) LinearIn = 1.0;

	if (LinearOut < 0.0) LinearOut = 0.0;
	if (LinearOut > 1.0) LinearOut = 1.0;

	if (LinearZip < 0.0) LinearZip = 0.0;
	if (LinearZip > 1.0) LinearZip = 1.0;

	PoleLines_In[0] = LinearIn ;
	PoleLines_Out[0] = LinearOut ;
	PoleLines_Comp[0] = LinearZip ;

	PoleLines_In[0] = (LinearIn * 0.1 + PoleLines_In[1] * 0.9);
	PoleLines_Out[0] = (LinearOut * 0.1 + PoleLines_Out[1] * 0.9);
	PoleLines_Comp[0] = (LinearZip * 0.1 + PoleLines_Comp[1] * 0.9);

	PoleLinesOffset++;
}

void            PluginDistortion::Metter_Draw(f32 X, f32 Y, f32 W, f32 H) {
	X = round(X);
	Y = round(Y);
	f32 WAddX = W * (1.0 / (f32)POLYLINEMETERCOUNT);

	for (u32 i = 0; i < POLYLINEMETERCOUNT; i++) {
		PoleLines_In_Vec[i] = ImVec2(
			round(X + WAddX * (f32)i),
			round(Y + H * (1.0 - PoleLines_In[i]))
		);

		PoleLines_Out_Vec[i] = ImVec2(
			round(X + WAddX * (f32)i),
			round(Y + H * (1.0 - PoleLines_Out[i]))
		);

		PoleLines_Comp_Vec[i] = ImVec2(
			round(X + WAddX * (f32)i),
			round(Y + H * (1.0 - PoleLines_Comp[i]))
		);
	}

}
