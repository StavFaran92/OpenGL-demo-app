#include "Menu.h"

// Helper to wire demo markers located in code to a interactive browser
typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiDemoMarkerCallback  GImGuiDemoMarkerCallback;
extern void* GImGuiDemoMarkerCallbackUserData;
ImGuiDemoMarkerCallback         GImGuiDemoMarkerCallback = NULL;
void* GImGuiDemoMarkerCallbackUserData = NULL;
#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)

void DisplayMenu()
{
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

    ShowAppMainMenuBar();
}

void ShowAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ShowMenuFile()
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
    if (ImGui::MenuItem("Load Model"))
    {
        OpenFile();
    }

    if (ImGui::MenuItem("Quit", "Alt+F4")) 
    {
        Application::Get().Close();
    }
}

void OpenFile() 
{
    auto filePath = FileUtil::OpenFile("obj files (*.obj)\0*.obj\0");
    if (!filePath.empty())
    {
        logInfo("Open file: " + filePath);

        std::shared_ptr<Model> model = std::make_shared<Model>(filePath);
        model->loadModel();

        Application::Get().GetContext()->AddModel(model);
    }
}

void SaveFile()
{
    auto filePath = FileUtil::SaveFile("obj files (*.obj)\0*.obj\0");
    if (!filePath.empty())
    {
        logDebug("Save file: " + filePath);
    }
}