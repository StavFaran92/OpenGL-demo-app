#include "EntryPoint.h"
#include "sge.h"

// Define a structure to represent an object in the scene hierarchy
struct SceneObject {
	std::string name;
	// Add any other properties as needed
};

// Define a vector to store scene objects
std::vector<SceneObject> sceneObjects
{
	{"a"},{"b"},{"c"}
};

auto style = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse;

void RenderSceneHierarchyWindow(float width, float height) 
{
	float windowWidth = width * 0.2f;
	ImVec2 windowPos(5, 25); // Adjust vertical position to make space for the menu bar
	ImVec2 windowSize(windowWidth, height * 0.8f);
	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);
	ImGui::Begin("Scene Hierarchy", nullptr, style);

	// Calculate the size of the list box accounting for padding
	ImVec2 listBoxSize(windowSize.x - 10, windowSize.y - 10);

	// Render list view
	if (ImGui::BeginListBox("Objects", listBoxSize)) 
	{
		// Iterate through each scene object and render it as a selectable item in the list
		for (const auto& obj : sceneObjects) 
		{
			if (ImGui::Selectable(obj.name.c_str())) 
			{
				// Handle selection of the object
				// For example, you can highlight the selected object in the view window
			}
		}
		ImGui::EndListBox();
	}

	ImGui::End();
}

void RenderViewWindow(float width, float height) 
{
	float windowWidth = width * 0.6f - 10;
	float startX = width * 0.2f + 10; // Add a gap of 10 pixels
	ImGui::SetNextWindowPos(ImVec2(startX, 25)); // Adjust vertical position to make space for the menu bar
	ImGui::SetNextWindowSize(ImVec2(windowWidth, height * 0.8f));
	ImGui::Begin("View", nullptr, style | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	// Display the texture
	ImVec2 imageSize(windowWidth, height * 0.8f);
	ImGui::Image(reinterpret_cast<ImTextureID>(Engine::get()->getContext()->getActiveScene()->getRenderTarget()), imageSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void RenderInspectorWindow(float width, float height) {
	float windowWidth = width * 0.2f - 5;
	float startX = width * 0.8f + 5; // Add a gap of 5 pixels
	ImGui::SetNextWindowPos(ImVec2(startX, 25)); // Adjust vertical position to make space for the menu bar
	ImGui::SetNextWindowSize(ImVec2(windowWidth - 5, height * 0.8f));
	ImGui::Begin("Inspector", nullptr, style);
	// Render inspector content here
	ImGui::End();
}

void RenderAssetViewWindow(float width, float height) {
	float windowWidth = width - 10;
	float startX = 5; // Add a gap of 5 pixels
	float startY = height * 0.8f + 30; // Adjust vertical position to place it below the "Scene Hierarchy" window
	ImGui::SetNextWindowPos(ImVec2(startX, startY));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, height * 0.2f - 35)); // Adjust height as needed
	ImGui::Begin("Asset View", nullptr, style);
	// Render asset view content here
	ImGui::End();
}


class GUI_Helper : public GuiMenu {
	// Inherited via GuiMenu
	virtual void display() override {
		// Get the total screen size
		int screenWidth = Engine::get()->getWindow()->getWidth();
		int screenHeight = Engine::get()->getWindow()->getHeight();

		ImGui::StyleColorsDark(); // Change the style to a dark theme


		// Render menu bar
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				// File menu items
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				// Edit menu items
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help")) {
				// Help menu items
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Render UI
		RenderSceneHierarchyWindow(screenWidth, screenHeight);
		RenderViewWindow(screenWidth, screenHeight);
		RenderInspectorWindow(screenWidth, screenHeight);
		RenderAssetViewWindow(screenWidth, screenHeight); // Add the Asset View window

	}
};





class EditorApp : public Application
{
public:

	void start() override
	{
		Skybox::CreateSkybox({ SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/right.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/left.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/top.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/bottom.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/front.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/back.jpg" });

		auto camera = Engine::get()->getContext()->getActiveScene()->getActiveCamera();
		camera->lookAt(0, 5, 0);
		camera->setPosition(25, 225, 35);

		sceneObjects.clear();
		for (auto&& [entity, obj] : Engine::get()->getContext()->getActiveScene()->getRegistry().view<ObjectComponent>().each())
		{
			sceneObjects.emplace_back(SceneObject{ obj.name });
		}

		auto gui = new GUI_Helper();
		Engine::get()->getImguiHandler()->addGUI(gui);
	}
};

Application* CreateApplication()
{
	return new EditorApp();
}