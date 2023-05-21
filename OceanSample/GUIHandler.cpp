#include "GUIHandler.h"

GUIHandler::GUIHandler(Shader* shader)
    : GuiMenu()
{
    auto ctx = ImGui::GetCurrentContext();

    ImGui::SetCurrentContext(ctx);

    m_shader = shader;
}

// Inherited via GuiMenu
void GUIHandler::display()
{
    ImGui::SetNextWindowPos({ 10, 10 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ 300, 250 }, ImGuiCond_Once);
    ImGui::Begin("Gretsner wave Controller", 0, ImGuiWindowFlags_NoResize);


    //m_shader->setValue("waveLength", 2.f);
    //m_shader->setValue("waveSpeed", 5.0f);

    ImGui::PushItemWidth(100);

    static float amplitude = 0.5f;

    if (ImGui::InputFloat("amplitude", &amplitude))
        m_shader->setValue("amplitude", amplitude);

    static auto waveDir = glm::vec2(1, 0);

    if (ImGui::InputFloat2("wave Direction (x,y)", glm::value_ptr(waveDir)))
        m_shader->setValue("waveDirection", waveDir);

    static float waveLength = 2.0f;

    if (ImGui::InputFloat("wave Length", &waveLength))
        m_shader->setValue("waveLength", waveLength);

    static float waveSpeed = 5.0f;

    if (ImGui::InputFloat("wave Speed", &waveSpeed))
        m_shader->setValue("waveSpeed", waveSpeed);

    ImGui::PopItemWidth();
    //static Shift shift = Shift::CW;
    //ImGui::RadioButton("CW", (int*)&shift, 0);
    //ImGui::RadioButton("CCW", (int*)&shift, 1);

    ImGui::Spacing();


    ImGui::End();


}