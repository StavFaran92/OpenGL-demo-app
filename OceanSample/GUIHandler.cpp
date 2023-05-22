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

    static float steepness = .5f;

    if (ImGui::SliderFloat("Steepness", &steepness, 0, 1))
        m_shader->setValue("steepness", steepness);

    ImGui::PopItemWidth();


    ImGui::End();


}