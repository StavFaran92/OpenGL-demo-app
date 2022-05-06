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
    ImGui::SetNextWindowSize({ 200, 200 }, ImGuiCond_Appearing);
    ImGui::Begin("Rubiks Cube Controller");

    const char* axis[] = { "X", "Y", "Z"};
    static int axis_current = 0;
    ImGui::ListBox("", &axis_current, axis, IM_ARRAYSIZE(axis), 3);

    static int index = 0;
    ImGui::InputInt("", &index);

    static Shift shift = Shift::CW;
    ImGui::RadioButton("CW", (int*)&shift, 0);
    ImGui::RadioButton("CCW", (int*)&shift, 1);

    if (ImGui::Button("Rotate"))
    {
        Axis actualAxis = Axis::X;
        switch (axis_current)
        {
        case 0:
            actualAxis = Axis::X;
            break;
        case 1:
            actualAxis = Axis::Y;
            break;
        case 2:
            actualAxis = Axis::Z;
            break;
        }

        m_rubiksCube->rotateFace(actualAxis, index, shift);
    }

    ImGui::End();


}
