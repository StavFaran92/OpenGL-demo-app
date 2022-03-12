#include "Menu.h"
#include "Graphics/Lighting/PointLight.h"

static void ShowExampleAppDockSpace();
static void ShowExampleAppLog();

static bool ShowLightCreatorWindow = false;
static bool showModelCreatorWindow = false;
static bool showModelInspectorWindow = false;
static bool showPrimitiveCreatorWindow = false;

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
    LightCreatorWindow();
    ShowModelCreatorWindow();
    ShowModelInspectorWindow();
    ShowPrimitiveCreatorWindow();

    //bool show_demo_window = true;
    //bool show_another_window = true;
    //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;

    //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &show_another_window);

    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //    ImGui::End();
    //}

    //const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
    //static int item_current_idx = 0; // Here we store our selection data as an index.
    //if (ImGui::BeginListBox("", { -FLT_MIN , 0}))
    //{
    //    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
    //    {
    //        const bool is_selected = (item_current_idx == n);
    //        if (ImGui::Selectable(items[n], is_selected))
    //        {
    //            item_current_idx = n;
    //            std::cout << "pick element " + std::to_string(n) << std::endl;
    //        }

    //        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
    //        if (is_selected)
    //            ImGui::SetItemDefaultFocus();
    //    }
    //    ImGui::EndListBox();
    //}

    //// 3. Show another simple window.
    //if (show_another_window)
    //{
    //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        show_another_window = false;
    //    ImGui::End();
    //}

    //ShowExampleAppDockSpace();
    //ShowExampleAppLog();
}

static void HelpMarker(const char* desc)
{
ImGui::TextDisabled("(?)");
if (ImGui::IsItemHovered())
{
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
}
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
        showModelCreatorWindow = true;
    }

    if (ImGui::MenuItem("Add Light"))
    {
        ShowLightCreatorWindow = true;
    }

    if (ImGui::MenuItem("Add Primitive"))
    {
        showPrimitiveCreatorWindow = true;
    }

    if (ImGui::MenuItem("Quit", "Alt+F4"))
    {
        Application::Get().Close();
    }
}

//void LoadModel()
//{
//    auto filePath = FileUtil::OpenFile("All formats (*.obj *.blend *.fbx *.dae)\0*.obj;*.blend;*.fbx;*.dae\0obj files (*.obj)\0*.obj\0Blender 3D (*.blend)\0*.blend\0Autodesk 3D (*.fbx)\0*.fbx\0Collada (*dae)\0*.dae\0");
//    if (!filePath.empty())
//    {
//        logInfo("Open file: " + filePath);
//
//        std::shared_ptr<Model> model = std::make_shared<Model>(filePath);
//        model->FlipTexture(true);
//        model->LoadModelFromFile();
//        std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
//        model->UseMaterial(material);
//
//        Application::Get().GetContext()->AddModel(model);
//    }
//}

void SaveFile()
{
    auto filePath = FileUtil::SaveFile("obj files (*.obj)\0*.obj\0");
    if (!filePath.empty())
    {
        logDebug("Save file: " + filePath);
    }
}

enum class LightType {
    DirectionalLight = 0,
    PointLight = 1
};

void LightCreatorWindow()
{
    if(ShowLightCreatorWindow)
    {
        ImGui::SetNextWindowSize({400, 300}, ImGuiCond_Appearing);
        ImGui::Begin("Light Creator");

        static LightType lightType =  LightType::DirectionalLight ;
        static float ambientIntensity = 0.2f;
        static float diffuseIntensity = 0.5f;
        static glm::vec3 color(1.f, 1.f, 1.f);
        static glm::vec3 dir(0.f, 0.f, 0.f);
        static glm::vec3 pos(0.f, 0.f, 0.f);
        static Attenuation attenuation;

        // Light Type
        ImGui::RadioButton("Directional Light", (int*)&lightType, 0);
        ImGui::RadioButton("Point Light", (int*)&lightType, 1);

        ImGui::ColorEdit3("Color", (float*)&color); 
        ImGui::SliderFloat("Ambient intensity", &ambientIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse intensity", &diffuseIntensity, 0.0f, 1.0f);

        if (lightType == LightType::DirectionalLight)
        {
            ImGui::InputFloat3("Direction", (float*)&dir);
        }
        if (lightType == LightType::PointLight)
        {
            ImGui::InputFloat3("Position", (float*)&pos);
            ImGui::LabelText("", "Attenuation");
            ImGui::SliderFloat("constant", (float*)&attenuation.constant, 0.f, 1.f);
            ImGui::SliderFloat("linear", (float*)&attenuation.linear, 0.f, 1.f);
            ImGui::SliderFloat("quadratic", (float*)&attenuation.quadratic, 0.f, 1.f);
        }

        if(ImGui::Button("Ok"))
        {
            if (lightType == LightType::DirectionalLight) 
            {
                
                std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>(color, dir, ambientIntensity, diffuseIntensity);
                Application::Get().GetContext()->AddDirectionalLight(light);

            }
            else if (lightType == LightType::PointLight)
            {
                std::shared_ptr<PointLight> light = std::make_shared<PointLight>(color, pos, ambientIntensity, diffuseIntensity, attenuation);
                Application::Get().GetContext()->AddPointLight(light);
            }

            ShowLightCreatorWindow = false;

            logInfo("Added light successfully.");
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ShowLightCreatorWindow = false;
        }

        ImGui::End();
    }

}

void ShowModelCreatorWindow()
{
    if (showModelCreatorWindow)
    {
        ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
        ImGui::Begin("Model Creator");

        static bool flipTexture = false;
        static ImGuiTextBuffer modelPath;
        static std::string path = "";
        static glm::vec3 pos(0.f, 0.f, 0.f);
        static glm::vec3 rotation(0.f, 0.f, 0.f);
        static glm::vec3 scale(1.f, 1.f, 1.f);



        ImGui::LabelText("", "Model path");
        if (ImGui::Button("Browse"))
        {
            auto filePath = FileUtil::OpenFile(g_supportedFormats);
            if (!filePath.empty())
            {
                modelPath.clear();
                modelPath.append(filePath.c_str());
                path = modelPath.c_str();
            }
        } 
        ImGui::SameLine();
        ImGui::TextUnformatted(modelPath.begin(), modelPath.end());
        
        ImGui::LabelText("", "Texture");
        ImGui::Checkbox("Flip Texture", &flipTexture);

        ImGui::LabelText("", "Transformation");
        ImGui::InputFloat3("Position", (float*)&pos);
        ImGui::InputFloat3("Rotation", (float*)&rotation);
        ImGui::InputFloat3("Scale", (float*)&scale);

        if (ImGui::Button("Ok"))
        {
            //todo validate input

            logInfo("Open file: " + path);

            auto model = Model::LoadModelFromFile(modelPath.c_str(), flipTexture);
            std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
            model->UseMaterial(material);
            model->GetTransformation()->SetPosition(pos);
            model->GetTransformation()->SetScale(scale);

            Application::Get().GetContext()->AddModel(model);

            showModelCreatorWindow = false;

            logInfo("Added Model successfully.");
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            showModelCreatorWindow = false;
        }

        ImGui::End();
    }
}

void ShowModelInspectorWindow()
{
    if (showModelInspectorWindow)
    {
        static uint32_t id = 0;
        static glm::vec3 pos(0.f, 0.f, 0.f);
        static glm::vec3 rotation(0.f, 0.f, 0.f);
        static glm::vec3 scale(1.f, 1.f, 1.f);

        ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
        ImGui::Begin("Model Inspector");

        ImGui::LabelText("", "Info");
        ImGui::LabelText("Model id: ", std::to_string(id).c_str());



        ImGui::LabelText("", "Transformation");
        if (ImGui::InputFloat3("Position", (float*)&pos))
        {

        }
        ImGui::InputFloat3("Rotation", (float*)&rotation);
        ImGui::InputFloat3("Scale", (float*)&scale);

        ImGui::End();
    }
}

enum class Shape {
    Quad = 0,
    Cube = 1
};

void ShowPrimitiveCreatorWindow()
{
    if (showPrimitiveCreatorWindow)
    {
        ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
        ImGui::Begin("Primitve Creator");

        static bool flipTexture = false;
        static ImGuiTextBuffer texturePath;
        static std::string path = "";
        static glm::vec3 pos(0.f, 0.f, 0.f);
        static glm::vec3 rotation(0.f, 0.f, 0.f);
        static glm::vec3 scale(1.f, 1.f, 1.f);
        static Model::PrimitiveType shape = Model::PrimitiveType::Quad;
        //static Model:: type = Model::PrimitiveType::Quad;


        ImGui::LabelText("", "Shape");
        ImGui::RadioButton("Quad", (int*)&shape, 0);
        ImGui::RadioButton("Cube", (int*)&shape, 1);

        //ImGui::RadioButton("Texture", (int*)&type, 0);
        //ImGui::RadioButton("Reflection", (int*)&type, 1);
        //ImGui::RadioButton("Refractive", (int*)&type, 2);
        ImGui::LabelText("", "Texture");
        if (ImGui::Button("Browse"))
        {
            auto filePath = FileUtil::OpenFile(g_supportedFormats);
            if (!filePath.empty())
            {
                texturePath.clear();
                texturePath.append(filePath.c_str());
                path = texturePath.c_str();
            }
        }
        ImGui::SameLine();
        ImGui::TextUnformatted(texturePath.begin(), texturePath.end());

        ImGui::Checkbox("Flip Texture", &flipTexture);

        ImGui::LabelText("", "Transformation");
        ImGui::InputFloat3("Position", (float*)&pos);
        ImGui::InputFloat3("Rotation", (float*)&rotation);
        ImGui::InputFloat3("Scale", (float*)&scale);

        if (ImGui::Button("Ok"))
        {
            //todo validate input

            logInfo("Open file: " + path);

            //auto texture = Texture::LoadTextureFromFile(texturePath.c_str(), flipTexture);

            std::shared_ptr<Model> model = nullptr;

            model = Model::CreatePrimitiveModel(shape);
            model->FlipTexture(flipTexture);
            std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
            model->UseMaterial(material);
            model->GetTransformation()->SetPosition(pos);
            model->GetTransformation()->SetScale(scale);

            Application::Get().GetContext()->AddModel(model);

            showPrimitiveCreatorWindow = false;

            logInfo("Added Model successfully.");
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            showModelCreatorWindow = false;
        }

        ImGui::End();
    }
}

//void ShowSkyboxCreatorWindow()
//{
//    if (showSkyboxCreatorWindow)
//    {
//        ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
//        ImGui::Begin("Skybox Creator");
//
//        static ImGuiTextBuffer texturePath;
//        static std::string path = "";
//
//        ImGui::LabelText("", "Shape");
//        ImGui::RadioButton("Quad", (int*)&shape, 0);
//        ImGui::RadioButton("Cube", (int*)&shape, 1);
//
//        ImGui::LabelText("", "Texture");
//        if (ImGui::Button("Browse"))
//        {
//            auto filePath = FileUtil::OpenFile(g_supportedFormats);
//            if (!filePath.empty())
//            {
//                texturePath.clear();
//                texturePath.append(filePath.c_str());
//                path = texturePath.c_str();
//            }
//        }
//        ImGui::SameLine();
//        ImGui::TextUnformatted(texturePath.begin(), texturePath.end());
//
//        ImGui::Checkbox("Flip Texture", &flipTexture);
//
//        ImGui::LabelText("", "Transformation");
//        ImGui::InputFloat3("Position", (float*)&pos);
//        ImGui::InputFloat3("Rotation", (float*)&rotation);
//        ImGui::InputFloat3("Scale", (float*)&scale);
//
//        if (ImGui::Button("Ok"))
//        {
//            //todo validate input
//
//            logInfo("Open file: " + path);
//
//            //auto texture = Texture::LoadTextureFromFile(texturePath.c_str(), flipTexture);
//
//            std::shared_ptr<Model> model = nullptr;
//
//            model = Model::CreatePrimitiveModel(shape);
//            model->FlipTexture(flipTexture);
//            std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
//            model->UseMaterial(material);
//            model->GetTransformation()->SetPosition(pos);
//            model->GetTransformation()->SetScale(scale);
//
//            Application::Get().GetContext()->AddModel(model);
//
//            showPrimitiveCreatorWindow = false;
//
//            logInfo("Added Model successfully.");
//        }
//        ImGui::SameLine();
//        if (ImGui::Button("Cancel"))
//        {
//            showModelCreatorWindow = false;
//        }
//
//        ImGui::End();
//    }
//}