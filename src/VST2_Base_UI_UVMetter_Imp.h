#pragma once

#define     IMGUI_LEVELS_PART1_PERCENT     0.50f
#define     IMGUI_LEVELS_PART2_PERCENT     0.90f

ImU32   InterpColor( ImU32 A, ImU32 B, f32 ff ) {
    u8 RA = A & 0xFF;
    u8 GA = (A >> 8) & 0xFF;
    u8 BA = (A >> 16) & 0xFF;

    u8 AA = (A >> 24) & 0xFF;

    u8 RB = B & 0xFF;
    u8 GB = (B >> 8) & 0xFF;
    u8 BB = (B >> 16) & 0xFF;

    u8 RC = RA + (RB - RA)*ff;
    u8 GC = GA + (GB - GA)*ff;
    u8 BC = BA + (BB - BA)*ff;
    return (AA << 24) | (BC << 16) | (GC << 8) | RC; 
}


void        ImGui_Levels_(
    ImVec2 PosStart, ImVec2 PosEnd, 
    f32 LinearLevel, f32 PeakLevel, bool PeakL,
    ImU32 Color1,  ImU32 Color2, 
    ImU32 Color3,  ImU32 Color4, 
    ImU32 Color5,  ImU32 Color6
    ) 
{
    ImDrawList* DrawList        = ImGui::GetWindowDrawList(); 
    f32 W                       = PosEnd.x - PosStart.x;
    f32 H                       = PosEnd.y - PosStart.y;
    f32     PeakInvHeight       = H * 0.5 * 0.01;
    f32     LevelsX             = PosStart.x;
    f32     LevelsY             = PosStart.y + PeakInvHeight;
    f32     LevelsX2            = PosEnd.x;
    f32     LevelsY2            = PosEnd.y;

    // draw level L R
    ImU32 ColorMin, ColorMax;
    ImVec2 LevelLStart, LevelLEnd, LevelRStart, LevelREnd;

    f32 Part0Scale = min(1.0f, max(0.0f, LinearLevel)*(1.0f / IMGUI_LEVELS_PART1_PERCENT));
    f32 Part1Scale = min(1.0f, max(0.0f, LinearLevel - IMGUI_LEVELS_PART1_PERCENT)*(1.0f / (IMGUI_LEVELS_PART2_PERCENT - IMGUI_LEVELS_PART1_PERCENT)));
    f32 Part2Scale = min(1.0f, max(0.0f, LinearLevel - IMGUI_LEVELS_PART2_PERCENT)*(1.0f / (1.00f - IMGUI_LEVELS_PART2_PERCENT)));

    f32 Part0SizeScale = 1.0 - min(IMGUI_LEVELS_PART1_PERCENT, LinearLevel);
    f32 Part1SizeScale = 1.0 - min(IMGUI_LEVELS_PART2_PERCENT, LinearLevel);
    f32 Part2SizeScale = 1.0 - LinearLevel;

    ColorMin        = Color1;
    ColorMax        = InterpColor(Color1, Color2, Part0Scale);

    LevelLStart     = ImVec2(LevelsX,      LevelsY2);
    LevelLEnd       = ImVec2(LevelsX2,     LevelsY + (LevelsY2 - LevelsY)*Part0SizeScale);

    DrawList->AddRectFilledMultiColor(LevelLStart, LevelLEnd,  ColorMin, ColorMin, ColorMax, ColorMax);

    // 2
    if (LinearLevel > IMGUI_LEVELS_PART1_PERCENT) {
        ColorMin    = Color3;
        ColorMax    = InterpColor(Color3, Color4, Part1Scale);

        LevelLStart = ImVec2(LevelsX,      LevelsY + (LevelsY2 - LevelsY)*Part0SizeScale);
        LevelLEnd   = ImVec2(LevelsX2,     LevelsY + (LevelsY2 - LevelsY)*Part1SizeScale);

        DrawList->AddRectFilledMultiColor(LevelLStart, LevelLEnd,  ColorMin, ColorMin, ColorMax, ColorMax); 
    }

    // 3
    if (LinearLevel > IMGUI_LEVELS_PART2_PERCENT) {
        ColorMin    = Color5;
        ColorMax    = InterpColor(Color5, Color6, Part2Scale);
        
        LevelLStart = ImVec2(LevelsX,      LevelsY + (LevelsY2 - LevelsY)*Part1SizeScale);
        LevelLEnd   = ImVec2(LevelsX2,     LevelsY + (LevelsY2 - LevelsY)*Part2SizeScale);

        DrawList->AddRectFilledMultiColor(LevelLStart, LevelLEnd,  ColorMin, ColorMin, ColorMax, ColorMax); 
    }

    // draw peeak level
    DrawList->AddLine(
        ImVec2(LevelsX-1,   LevelsY + (LevelsY2 - LevelsY)*(1.0 - PeakLevel)),
        ImVec2(LevelsX2-1,  LevelsY + (LevelsY2 - LevelsY)*(1.0 - PeakLevel)),
        ImColor(220, 220, 220),
        H * 0.5 * 0.01
    );

    // draw peek
    if (PeakL) DrawList->AddRectFilled(
        ImVec2(LevelsX, LevelsY - PeakInvHeight), 
        ImVec2(LevelsX2, LevelsY), 
        ImColor(245, 70, 70)
    );

}
