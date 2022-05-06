#include "RubiksCubeGui.h"
#include "RubiksCubeConfigurations.h"

RubiksCubeGui::RubiksCubeGui(RubiksCube* rubiksCube)
    : GuiMenu()
{
    auto ctx = ImGui::GetCurrentContext();

    ImGui::SetCurrentContext(ctx);

    m_rubiksCube = rubiksCube;

    //void** p_alloc_func;
    //void* p_free_func;
    //void** p_user_data;
    //ImGui::GetAllocatorFunctions(p_alloc_func, p_free_func, p_user_data);

    //ImGui::SetAllocatorFunctions(p_alloc_func, p_free_func, p_user_data);
}

void RubiksCubeGui::display()
{
    ImGui::SetNextWindowPos({ 10, 10 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ 200, 250 }, ImGuiCond_Once);
    ImGui::Begin("Rubiks Cube Controller", 0, ImGuiWindowFlags_NoResize);

    if (ImGui::Button("Shuffle"))
    {
        m_rubiksCube->shuffle();
    }

    ImGui::Separator();

    const char* axis[] = { "X", "Y", "Z"};
    static Axis axis_current = Axis::X;
    ImGui::ListBox("", (int*)&axis_current, axis, IM_ARRAYSIZE(axis), 3);

    static int index = 0;
    ImGui::InputInt("", &index);

    static Shift shift = Shift::CW;
    ImGui::RadioButton("CW", (int*)&shift, 0);
    ImGui::RadioButton("CCW", (int*)&shift, 1);

    ImGui::Spacing();

    if (ImGui::Button("Rotate"))
    {
        m_rubiksCube->rotateFace(axis_current, index, shift);
    }

    ImGui::End();


}
