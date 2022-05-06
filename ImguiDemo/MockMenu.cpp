#include "MockMenu.h"

MockMenu::MockMenu()
    : GuiMenu()
{
    auto ctx = ImGui::GetCurrentContext();

    ImGui::SetCurrentContext(ctx);

    //void** p_alloc_func;
    //void* p_free_func;
    //void** p_user_data;
    //ImGui::GetAllocatorFunctions(p_alloc_func, p_free_func, p_user_data);

    //ImGui::SetAllocatorFunctions(p_alloc_func, p_free_func, p_user_data);
}

void MockMenu::display()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                //OpenFile();
            }
            if (ImGui::MenuItem("Save As..", "Ctrl+S"))
            {
                //SaveFile();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
