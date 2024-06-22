#include "EntryPoint.h"
#include "sge.h"

#include "Window.h"
#include "commdlg.h"

#include <filesystem>

#include "ImGuizmo.h"
#include "imgui_internal.h"

#include "tinyfiledialogs.h"

namespace fs = std::filesystem;

static bool ShowLightCreatorWindow = false;
static bool showModelCreatorWindow = false;
static bool showModelInspectorWindow = false;
static bool showPrimitiveCreatorWindow = false;
static bool showMeshSelector = false;

Entity g_primaryCamera;
Entity g_editorCamera;

static void addTextureEditWidget(std::shared_ptr<Material> mat, const std::string& name, Texture::Type ttype);
void AddColoredLabel(const char* label);
static void displayTransformation(Transformation& transform, bool& isChanged);
static void displaySelectMeshWindow();

template<typename T> 
static void displayComponent(const std::string& componentName, std::function<void(T&)> func)
{
	if (selectedEntity.HasComponent<T>())
	{
		ImVec2 startPos = ImGui::GetCursorScreenPos();
		ImVec2 startPosCursor = ImGui::GetCursorPos(); // Capture the initial cursor position

		AddColoredLabel(componentName.c_str());
		auto& component = selectedEntity.getComponent<T>();

		ImVec2 cursorPos = ImGui::GetCursorPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		if (!std::is_same<T, Transformation>::value)
		{
			ImGui::SetCursorPos(ImVec2(windowSize.x - 24.0f, cursorPos.y - ImGui::GetTextLineHeightWithSpacing() - 7.0f));
			ImGui::PushID(componentName.c_str());
			if (ImGui::Button("X")) {
				selectedEntity.RemoveComponent<T>();
				ImGui::EndGroup();
				updateScene();
				return;
			}
			ImGui::PopID();
		}

		ImGui::Indent(5); // Indent by 10 pixels
		func(component);
		ImGui::Unindent(5); // Remove the indent

		ImVec2 endPosCursor = ImGui::GetCursorPos(); // Capture the cursor position before adding the separator
		ImVec2 endPos = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y + (endPosCursor.y - startPosCursor.y));

		ImGui::Dummy(ImVec2(0, 4));

		ImGui::Separator();

		ImGui::Dummy(ImVec2(0, 4));

		// Adjust the rectangle to the correct end position
		ImGui::GetWindowDrawList()->AddRect(startPos, endPos, ImGui::GetColorU32(ImGuiCol_Header), 0.f, 0, 2.f);
	}
}

void RenderSimulationControlView(float width, float height)
{
	float windowWidth = width * 0.6f - 10;
	float windowHeight = 35;
	float startX = width * 0.2f + 10; // Add a gap of 10 pixels
	ImGui::SetNextWindowPos(ImVec2(startX, 25)); // Adjust vertical position to make space for the menu bar
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	ImGui::Begin("Centered Buttons", nullptr, ImGuiWindowFlags_NoDecoration);

	// Center align the buttons
	ImGui::SetCursorPosX((windowWidth - 100) * 0.5f);

	static bool startButtonPressed = false;

	// Draw the button based on the current state
	if (startButtonPressed) {
		if (ImGui::Button("STOP", ImVec2(70, 0))) {
			// Handle stop button click
			startButtonPressed = false; // Toggle the state
			Engine::get()->getContext()->getActiveScene()->stopSimulation();

			Engine::get()->getContext()->getActiveScene()->setPrimaryCamera(g_editorCamera);
		}
	}
	else {
		if (ImGui::Button("START", ImVec2(70, 0))) {
			// Handle start button click
			startButtonPressed = true; // Toggle the state
			Engine::get()->getContext()->getActiveScene()->startSimulation();

			Engine::get()->getContext()->getActiveScene()->setPrimaryCamera(g_primaryCamera);
		}
	}

	ImGui::End(); // End the window
}

static void displayTransformation(Transformation& transform, bool& isChanged)
{
	glm::vec3& pos = transform.getLocalPosition();
	glm::vec3& rotation = transform.getLocalRotationVec3();
	glm::vec3& scale = transform.getLocalScale();

	// Position slider
	if (ImGui::DragFloat3("Position", glm::value_ptr(pos))) 
	{
		transform.setLocalPosition(pos);
		isChanged = true;
	}

	// Rotation slider (Euler angles)
	if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation))) 
	{
		transform.setLocalRotation(rotation);
		isChanged = true;
	}

	// Scale slider
	if (ImGui::DragFloat3("Scale", glm::value_ptr(scale))) 
	{
		transform.setLocalScale(scale);
		isChanged = true;
	}
}

static void displaySelectMeshWindow(std::string& uuid)
{
	if (showMeshSelector) 
	{
		ImGui::Begin("Select Mesh", &showMeshSelector, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Available Meshes:");
		ImGui::Separator();

		static int selectedMeshIndex = -1;

		auto& meshList = Engine::get()->getMemoryPool<Mesh>()->getAll();

		for (int i = 0; i < meshList.size(); i++) 
		{
			bool isSelected = (selectedMeshIndex == i);
			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Change background color
			}
			if (!isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Default color
			}

			if (ImGui::Selectable(meshList[i].c_str())) 
			{
				selectedMeshIndex = i;
			}

			ImGui::PopStyleColor();
		}

		ImGui::Separator();

		if (ImGui::Button("OK")) {
			if (selectedMeshIndex >= 0 && selectedMeshIndex < meshList.size()) 
			{
				uuid = meshList[selectedMeshIndex];
				
			}
			showMeshSelector = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			showMeshSelector = false;
		}

		ImGui::End();
	}
}

// Define a structure to represent an object in the scene hierarchy
struct SceneObject {
	std::string name;
	Entity e;
	// Add any other properties as needed
};

// Define a vector to store scene objects
std::vector<SceneObject> sceneObjects
{};

void updateScene()
{
	sceneObjects.clear();
	for (auto&& [entity, obj] : Engine::get()->getContext()->getActiveScene()->getRegistry().get().view<ObjectComponent>().each())
	{
		sceneObjects.emplace_back(SceneObject{ obj.name, obj.e });
	}
}



Entity  selectedEntity = Entity::EmptyEntity;

auto style = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse;

static const char* g_supportedFormats = "All formats (*.obj *.blend *.fbx *.dae)\0*.obj;*.blend;*.fbx;*.dae\0obj files (*.obj)\0*.obj\0Blender 3D (*.blend)\0*.blend\0Autodesk 3D (*.fbx)\0*.fbx\0Collada (*dae)\0*.dae\0";
static const char* g_supportedTextureFormats = "All formats (*.png *.jpg)\0";

void AddColoredLabel(const char* label) 
{
	// Draw a blue background using ImGuiCol_Header color
	ImVec2 startPos = ImGui::GetCursorScreenPos();
	ImVec2 endPos = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y + ImGui::GetTextLineHeightWithSpacing() + 2);
	ImGui::GetWindowDrawList()->AddRectFilled(startPos, endPos, ImGui::GetColorU32(ImGuiCol_Header));

	// Calculate the vertical offset to center the text within the rectangle
	float offsetY = (ImGui::GetTextLineHeightWithSpacing() - ImGui::GetFrameHeight()) * 0.5f;

	// Calculate padding values
	float paddingX = 5.0f;
	float paddingY = 2.0f;

	// Adjust the text position to center it vertically and add padding
	ImVec2 textPos = ImVec2(startPos.x + paddingX, startPos.y + offsetY + paddingY);

	// Render the label text
	ImGui::SetCursorScreenPos(textPos);
	ImGui::TextUnformatted(label);

	ImGui::Dummy(ImVec2(0.0f, 2.0f)); // Add a vertical gap
}

std::string OpenFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = (HWND)Engine::get()->getWindow()->GetNativeWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

std::string SaveFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = (HWND)Engine::get()->getWindow()->GetNativeWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

enum class LightType {
	DirectionalLight = 0,
	PointLight = 1
};

void LightCreatorWindow()
{
	if (ShowLightCreatorWindow)
	{
		ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
		ImGui::Begin("Light Creator");

		static LightType lightType = LightType::DirectionalLight;
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

		if (ImGui::Button("Ok"))
		{
			if (lightType == LightType::DirectionalLight)
			{
				auto e = Engine::get()->getContext()->getActiveScene()->createEntity();
				e.addComponent<DirectionalLight>(color, dir, ambientIntensity, diffuseIntensity);

			}
			else if (lightType == LightType::PointLight)
			{
				auto e = Engine::get()->getContext()->getActiveScene()->createEntity();
				e.addComponent<PointLight>(color, ambientIntensity, diffuseIntensity, attenuation);
			}

			ShowLightCreatorWindow = false;

			updateScene();

			//logInfo("Added light successfully.");
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ShowLightCreatorWindow = false;
		}

		ImGui::End();
	}

}

static void addAssetLoadWidget(const std::string& name, ImGuiTextBuffer& textBuffer, const char* assetSupportedFormats)
{
	ImGui::LabelText("", name.c_str());
	if (ImGui::Button(std::string("Browse##" + name).c_str()))
	{
		auto albedoFilePath = OpenFile(assetSupportedFormats);
		if (!albedoFilePath.empty())
		{
			textBuffer.clear();
			textBuffer.append(albedoFilePath.c_str());
		}
	}
	ImGui::SameLine();
	ImGui::TextUnformatted(textBuffer.begin(), textBuffer.end());
}

void ShowModelCreatorWindow()
{
	if (showModelCreatorWindow)
	{
		ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
		ImGui::Begin("Import model", false, ImGuiWindowFlags_AlwaysAutoResize);

		static ImGuiTextBuffer modelPathBuffer;

		static glm::vec3 pos(0.f, 0.f, 0.f);
		static glm::vec3 rotation(0.f, 0.f, 0.f);
		static glm::vec3 scale(1.f, 1.f, 1.f);

		// Model
		addAssetLoadWidget("Model", modelPathBuffer, g_supportedFormats);

		ImGui::Separator();
		ImGui::LabelText("", "Texture");

		// Textures
		static std::shared_ptr<Material> mat;
		if(!mat) mat = std::make_shared<Material>(*Engine::get()->getDefaultMaterial().get());

		addTextureEditWidget(mat, "Albedo", Texture::Type::Albedo);
		addTextureEditWidget(mat, "Normal", Texture::Type::Normal);
		addTextureEditWidget(mat, "Metallic", Texture::Type::Metallic);
		addTextureEditWidget(mat, "Roughness", Texture::Type::Roughness);
		addTextureEditWidget(mat, "Occlusion", Texture::Type::AmbientOcclusion);

		ImGui::Separator();

		if (ImGui::Button("Ok"))
		{
			if (modelPathBuffer.empty())
			{
				logError("Model Path not specified.")
				showModelCreatorWindow = false;
				return;
			}

			//todo validate input

			auto entity = Engine::get()->getModelImporter()->loadModelFromFile(modelPathBuffer.c_str(), Engine::get()->getContext()->getActiveScene().get());

			entity.getComponent<MaterialComponent>().addMaterial(mat);
			mat = nullptr;
			modelPathBuffer.clear();

			showModelCreatorWindow = false;

			updateScene();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			showModelCreatorWindow = false;
		}

		ImGui::End();
	}
}

void RenderSceneHierarchyWindow(float width, float height) 
{
	float windowWidth = width * 0.2f;
	ImVec2 windowPos(5, 25); // Adjust vertical position to make space for the menu bar
	ImVec2 windowSize(windowWidth, height * 0.8f);
	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);
	ImGui::Begin("Scene Hierarchy", nullptr, style);

	if (ImGui::Button("+", ImVec2(windowWidth, 0)))
	{
		ImGui::OpenPopup("AddObjectToScenePopup");
	}

	if (ImGui::BeginPopup("AddObjectToScenePopup")) 
	{
		if (ImGui::MenuItem("Entity"))
		{
			Engine::get()->getContext()->getActiveScene()->createEntity();
			updateScene();
			selectedEntity = sceneObjects[0].e;
		}
		if (ImGui::BeginMenu("Primitive")) 
		{ // Begin the submenu
			if (ImGui::MenuItem("Cube")) 
			{
				ShapeFactory::createBox(&Engine::get()->getContext()->getActiveScene()->getRegistry());
				updateScene();
				selectedEntity = sceneObjects[0].e;
			}
			if (ImGui::MenuItem("Sphere")) 
			{
				ShapeFactory::createSphere(&Engine::get()->getContext()->getActiveScene()->getRegistry());
				updateScene();
				selectedEntity = sceneObjects[0].e;
			}
			if (ImGui::MenuItem("Quad")) 
			{
				ShapeFactory::createQuad(&Engine::get()->getContext()->getActiveScene()->getRegistry());
				updateScene();
				selectedEntity = sceneObjects[0].e;
			}

			ImGui::EndMenu(); // End the submenu
		}

		if (ImGui::BeginMenu("Light"))
		{ // Begin the submenu
			if (ImGui::MenuItem("Directional Light"))
			{
				auto e = Engine::get()->getContext()->getActiveScene()->createEntity();
				e.addComponent<DirectionalLight>(glm::vec3{ 0,0,0 }, glm::vec3{0,-1,0}, 1.f, 1.f);
				updateScene();
				selectedEntity = sceneObjects[0].e;
			}
			if (ImGui::MenuItem("Point Light"))
			{

				auto e = Engine::get()->getContext()->getActiveScene()->createEntity();
				e.addComponent<PointLight>(glm::vec3{ 0,0,0 }, 1.f, 1.f, Attenuation());
				updateScene();
				selectedEntity = sceneObjects[0].e;
			}

			ImGui::EndMenu(); // End the submenu
		}

		// Add more submenus or menu items as needed
		ImGui::EndPopup();
	}

	// Calculate the size of the list box accounting for padding
	ImVec2 listBoxSize(windowSize.x - 10, windowSize.y - 10);

	// Render list view
	if (ImGui::BeginListBox("Objects", listBoxSize)) 
	{
		// Iterate through each scene object and render it as a selectable item in the list
		static int selected = -1;
		for (int i=0; i<sceneObjects.size(); i++) 
		{
			bool isSelected = (selected == i);
			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Change background color
			}
			if (!isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Default color
			}

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				// I have no idea why i-1 is used here, it's just works..
				selectedEntity = sceneObjects[i-1].e;

				selected = i - 1;

				ImGui::OpenPopup("SceneObjectContextPopup");
			}

			if (selected == i)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				if (ImGui::BeginPopup("SceneObjectContextPopup"))
				{
					//if (ImGui::MenuItem("Rename"))
					//{
					//	sceneObjects[i].name
					//	updateScene();
					//}
					if (sceneObjects[i].e.HasComponent<CameraComponent>())
					{
						if (ImGui::MenuItem("Set as Primary Camera"))
						{
							auto scene = Engine::get()->getContext()->getActiveScene();
							scene->setPrimaryCamera(sceneObjects[i].e);
						}
					}
					if (ImGui::MenuItem("Delete"))
					{
						sceneObjects[i].e.remove();
						updateScene();
						selectedEntity = Entity::EmptyEntity;
						selected = -1;
					}

					ImGui::EndPopup();
				}
				ImGui::PopStyleColor();
			}

			if (ImGui::Selectable(sceneObjects[i].name.c_str()))
			{
				selectedEntity = sceneObjects[i].e;

				selected = i;
			}

			ImGui::PopStyleColor();
		}
		ImGui::EndListBox();
	}

	ImGui::End();
}

void RenderViewWindow(float width, float height) 
{
	float windowWidth = width * 0.6f - 10;
	float windowHeight = height * 0.8f;
	float startX = width * 0.2f + 10; // Add a gap of 10 pixels
	ImGui::SetNextWindowPos(ImVec2(startX, 65)); // Adjust vertical position to make space for the menu bar
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight - 40));
	ImGui::Begin("View", nullptr, style | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration);

	// Display the texture
	ImVec2 imageSize(windowWidth, windowHeight);
	ImGui::Image(reinterpret_cast<ImTextureID>(Engine::get()->getContext()->getActiveScene()->getRenderTarget()), imageSize, ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		// Check if the mouse is within the window bounds
		if (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y)
		{
			// We alter the mouse position from small window into full screen (the renderered object pick texture)
			int alteredX = (mousePos.x - startX) / windowWidth * Engine::get()->getWindow()->getWidth();
			int alteredY = (mousePos.y - 65) / windowHeight * Engine::get()->getWindow()->getHeight();
			int selectedID = Engine::get()->getObjectPicker()->pickObject(alteredX, alteredY);

			for (auto& sceneObj : sceneObjects)
			{
				if (sceneObj.e.handlerID() == selectedID)
				{
					selectedEntity = sceneObj.e;
					break;
				}
			}

			logDebug(std::to_string(selectedID));
		}
	}

	if (selectedEntity != Entity::EmptyEntity)
	{
		// Define the size and position of the inner window
		float innerWindowWidth = windowWidth - 10;
		float innerWindowHeight = 35.0f;
		ImGui::SetNextWindowPos(ImVec2(startX + 7, 72)); // Adjust position as needed
		ImGui::SetNextWindowSize(ImVec2(innerWindowWidth, innerWindowHeight)); // Adjust size as needed

		// Transformation mode enum and current mode variable
		enum TransformMode { TRANSLATE, ROTATE, SCALE, UNIVERSAL };
		static TransformMode currentMode = TRANSLATE;

		// Gizmo mode variable
		static ImGuizmo::MODE currentGizmoMode = ImGuizmo::LOCAL;

		// Snap options
		static bool useSnap = false;
		static float snapValues[3] = { 1.0f, 1.0f, 1.0f };

		auto& transform = selectedEntity.getComponent<Transformation>();

		if (ImGui::BeginChild("TransformWindow", ImVec2(innerWindowWidth, innerWindowHeight), true))
		{
			// Radio buttons for transformation mode
			ImGui::RadioButton("Translate", (int*)&currentMode, TRANSLATE);
			ImGui::SameLine();
			ImGui::RadioButton("Rotate", (int*)&currentMode, ROTATE);
			ImGui::SameLine();
			ImGui::RadioButton("Scale", (int*)&currentMode, SCALE);
			ImGui::SameLine();
			ImGui::RadioButton("Universal", (int*)&currentMode, UNIVERSAL);

			// Button to toggle between local and world gizmo modes
			ImGui::SameLine();
			if (ImGui::Button(currentGizmoMode == ImGuizmo::LOCAL ? "Local" : "World"))
			{
				currentGizmoMode = (currentGizmoMode == ImGuizmo::LOCAL) ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
			}

			// Checkbox for snap
			ImGui::SameLine();
			ImGui::Checkbox("Snap", &useSnap);

			// Input fields for snap values
			ImGui::SameLine();
			float snapInputWidth = 80.0f;
			if (currentMode == TRANSLATE)
			{
				ImGui::SetNextItemWidth(snapInputWidth * 3);
				ImGui::InputFloat3("Snap Translate", snapValues);
			}
			else if (currentMode == ROTATE)
			{
				ImGui::SetNextItemWidth(snapInputWidth);
				ImGui::InputFloat("Snap Angle", &snapValues[0]);
			}
			else if (currentMode == SCALE)
			{
				ImGui::SetNextItemWidth(snapInputWidth);
				ImGui::InputFloat("Snap Scale", &snapValues[0]);
			}

			

			ImGui::EndChild(); // End the inner window
		}

		glm::mat4 glmMat = transform.getLocalTransformation();
		float* matrixPtr = glm::value_ptr(glmMat);

		auto camView = Engine::get()->getContext()->getActiveScene()->getActiveCameraView();
		const float* camViewPtr = glm::value_ptr(camView);

		auto projection = Engine::get()->getContext()->getActiveScene()->getProjection();
		const float* projectionPtr = glm::value_ptr(projection);

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// Set the operation mode based on the selected radio button
		ImGuizmo::OPERATION operationMode = ImGuizmo::TRANSLATE;
		switch (currentMode)
		{
		case TRANSLATE:
			operationMode = ImGuizmo::TRANSLATE;
			break;
		case ROTATE:
			operationMode = ImGuizmo::ROTATE;
			break;
		case SCALE:
			operationMode = ImGuizmo::SCALE;
			break;
		case UNIVERSAL:
			operationMode = ImGuizmo::UNIVERSAL;
			break;
		}

		ImGuizmo::Manipulate(camViewPtr, projectionPtr, operationMode, currentGizmoMode, matrixPtr, NULL, useSnap ? &snapValues[0] : NULL, NULL, NULL);

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrixPtr, matrixTranslation, matrixRotation, matrixScale);

		transform.setLocalPosition(glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
		transform.setLocalRotation(glm::vec3(Constants::toRadians * matrixRotation[0], Constants::toRadians* matrixRotation[1], Constants::toRadians* matrixRotation[2]));
		transform.setLocalScale(glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]));
	}

	ImGui::End();
}

static const char* rigidyBodyTypesStrList[]{ 
	"Static",
	"Dynamic",
	"Kinematic"
};

static const char* renderTechniqueStrList[]{
	"Forward",
	"Defererred"
};

static void addTextureEditWidget(std::shared_ptr<Material> mat, const std::string& name, Texture::Type ttype)
{
	unsigned int tid = 0;
	if (mat->hasTexture(ttype))
	{
		tid = mat->getTexture(ttype).get()->getID();
	}

	ImVec2 imageSize(20, 20);
	ImGui::Image(reinterpret_cast<ImTextureID>(tid), imageSize, ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

	if (ImGui::IsItemClicked()) {

		const char* supportedImageFormats[4] = { "*.png", "*.jpg", "*.bmp", "*.tga" };
		const char* texturetoUsePath = tinyfd_openFileDialog(
			"Select A texture to load",
			"",
			4,
			supportedImageFormats,
			"image files",
			0);

		if (texturetoUsePath) {
			// Load the new texture and set it to the material
			auto& tex = Texture::create2DTextureFromFile(texturetoUsePath, false);

			mat->setTexture(ttype, tex);
		}
	}

	ImGui::SameLine();
	ImGui::Text(name.c_str());
}

static void addSkyboxTextureEditWidget(SkyboxComponent& skybox)
{
	

	ImVec2 imageSize(50, 50);
	ImGui::Image(reinterpret_cast<ImTextureID>(skybox.skyboxImage.get()->getID()), imageSize, ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

	//if (ImGui::IsItemClicked()) {

	//	const char* supportedImageFormats[4] = { "*.png", "*.jpg", "*.bmp", "*.tga" };
	//	const char* texturetoUsePath = tinyfd_openFileDialog(
	//		"Select A texture to load",
	//		"",
	//		4,
	//		supportedImageFormats,
	//		"image files",
	//		0);

	//	if (texturetoUsePath) {
	//		// Load the new texture and set it to the material
	//		auto& tex = Texture::create2DTextureFromFile(texturetoUsePath, false);

	//		mat->setTexture(ttype, tex);
	//	}
	//}
}

void RenderInspectorWindow(float width, float height) 
{
	float windowWidth = width * 0.2f - 5;
	float startX = width * 0.8f + 5; // Add a gap of 5 pixels
	ImGui::SetNextWindowPos(ImVec2(startX, 25)); // Adjust vertical position to make space for the menu bar
	ImGui::SetNextWindowSize(ImVec2(windowWidth - 5, height * 0.8f));
	ImGui::Begin("Inspector", nullptr, style | ImGuiWindowFlags_NoScrollbar);

	if (selectedEntity != Entity::EmptyEntity)
	{
		displayComponent<Transformation>("Transformation", [](Transformation& transform) {
			bool isChanged = false;
			displayTransformation(transform, isChanged);
		});

		displayComponent<RigidBodyComponent>("RigidBody", [](RigidBodyComponent& rBody) {
			ImGui::Combo("##Type", (int*)&rBody.type, rigidyBodyTypesStrList, IM_ARRAYSIZE(rigidyBodyTypesStrList));
			ImGui::InputFloat("Mass", &rBody.mass);
		});

		displayComponent<CollisionBoxComponent>("Collision Box", [](CollisionBoxComponent& collisionBox) {
			ImGui::InputFloat("Half Extent", &collisionBox.halfExtent);
			});

		displayComponent<CollisionSphereComponent>("Collision Sphere", [](CollisionSphereComponent& collisionBox) {
			ImGui::InputFloat("Radius", &collisionBox.radius);
			});

		displayComponent<CollisionMeshComponent>("Collision Mesh", [](CollisionMeshComponent& collisionMesh) {
			});

		displayComponent<MeshComponent>("Mesh", [](MeshComponent& meshComponent) {
			if (meshComponent.mesh.isEmpty()) return;

			ImGui::Text("Number of vertices: %d", (int)meshComponent.mesh.get()->getNumOfVertices());

			// Button to trigger some action
			if (ImGui::Button("Select Mesh")) 
			{
				showMeshSelector = true;
			}

			std::string selectedMeshUID;
			displaySelectMeshWindow(selectedMeshUID);

			if (!selectedMeshUID.empty())
			{
				meshComponent.mesh = Resource<Mesh>(selectedMeshUID);
			}

			ImGui::SameLine();

			// Text display field
			ImGui::Text(meshComponent.mesh.getUID().c_str());
			});

		displayComponent<RenderableComponent>("Renderer", [](RenderableComponent& renderComponent) {
			ImGui::Combo("##Technique", (int*)&renderComponent.renderTechnique, renderTechniqueStrList, IM_ARRAYSIZE(renderTechniqueStrList));
			});

		displayComponent<CameraComponent>("Camera", [](CameraComponent& cameraComponent) {
			// TBD
			});

		displayComponent<NativeScriptComponent>("Script", [](NativeScriptComponent& nsc) {
			// TBD
			});

		displayComponent<MaterialComponent>("Materials", [](MaterialComponent& materials) {
			int index = 0;
			for (auto& mat : materials)
			{
				// Start a new collapsible header for each material
				if (ImGui::CollapsingHeader(("Material " + std::to_string(index)).c_str()))
				{
					// Add texture edit widgets for the material
					addTextureEditWidget(mat, "Albedo", Texture::Type::Albedo);
					addTextureEditWidget(mat, "Normal", Texture::Type::Normal);
					addTextureEditWidget(mat, "Metallic", Texture::Type::Metallic);
					addTextureEditWidget(mat, "Roughness", Texture::Type::Roughness);
					addTextureEditWidget(mat, "Occlusion", Texture::Type::AmbientOcclusion);
				}
				++index;
			}
			});

		displayComponent<DirectionalLight>("Directional Light", [](DirectionalLight& dLight) {
			auto& color = dLight.getColor();
			if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) {
				dLight.SetColor(color);
			}
			});

		displayComponent<PointLight>("Point Light", [](PointLight& pLight) {
			auto& color = pLight.getColor();
			if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) {
				pLight.SetColor(color);
			}

			Attenuation& attenuation = pLight.getAttenuation();
			ImGui::LabelText("", "Attenuation");
			ImGui::DragFloat("constant", &attenuation.constant, 0.01f);
			ImGui::DragFloat("linear", &attenuation.linear, 0.01f);
			ImGui::DragFloat("quadratic", &attenuation.quadratic, 0.01f);
			pLight.SetAttenuation(attenuation);
		});

		displayComponent<InstanceBatch>("Instance Batch", [](InstanceBatch& instanceBatch) {
			auto& transformations = instanceBatch.getTransformations();

			if (ImGui::Button("Add Transformation")) {
				instanceBatch.addTransformation(std::make_shared<Transformation>(selectedEntity));
			}

			if (ImGui::BeginChild("Transformations List", ImVec2(0, 200), true)) {
				bool isChanged = false;
				for (size_t i = 0; i < transformations.size(); ++i) {
					if (ImGui::TreeNode((void*)(intptr_t)i, "Transformation %zu", i)) {
						displayTransformation(*transformations[i], isChanged);
						
						ImGui::TreePop();
					}
				}
				if (isChanged)
				{
					instanceBatch.build();
				}
				ImGui::EndChild();
			}
			});

		displayComponent<SkyboxComponent>("Skybox", [](SkyboxComponent& skybox) {
			addSkyboxTextureEditWidget(skybox);
			});

		if (ImGui::Button("Add Component", ImVec2(windowWidth, 0)))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			if (ImGui::MenuItem("Transformation"))
			{
				selectedEntity.addComponent<Transformation>(selectedEntity);
			}

			if (ImGui::MenuItem("RigidBody"))
			{
				selectedEntity.addComponent<RigidBodyComponent>();
			}

			if (ImGui::MenuItem("Collision Box"))
			{
				selectedEntity.addComponent<CollisionBoxComponent>();
			}

			if (ImGui::MenuItem("Collision Sphere"))
			{
				selectedEntity.addComponent<CollisionSphereComponent>();
			}

			if (ImGui::MenuItem("Collision Mesh"))
			{
				auto meshComponent = selectedEntity.tryGetComponent<MeshComponent>();
				if (meshComponent)
				{
					auto& meshCollisions = selectedEntity.addComponent<CollisionMeshComponent>();
					meshCollisions.mesh = meshComponent->mesh;
				}
			}

			if (ImGui::MenuItem("Mesh"))
			{
				selectedEntity.addComponent<MeshComponent>();
			}

			if (ImGui::MenuItem("Camera"))
			{
				selectedEntity.addComponent<CameraComponent>();
			}

			if (ImGui::MenuItem("Script"))
			{
				selectedEntity.addComponent<NativeScriptComponent>();
			}

			if (ImGui::MenuItem("InstanceBatch"))
			{
				auto meshComponent = selectedEntity.tryGetComponent<MeshComponent>();
				if (meshComponent)
				{
					selectedEntity.addComponent<InstanceBatch>(std::vector<std::shared_ptr<Transformation>>{}, meshComponent->mesh);
				}
			}

			ImGui::EndPopup();
		}
		
	}

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
	ImVec2 listBoxSize(windowWidth, height * 0.2f - 35);

	auto& meshList = Engine::get()->getMemoryPool<Mesh>()->getAll();

	if (ImGui::TreeNode("Meshes")) {
		for (int i = 0; i < meshList.size(); i++) {
			if (ImGui::Selectable(meshList[i].c_str())) {
				// Do something when a mesh is selected
			}
		}
		ImGui::TreePop();
	}

	auto& textureList = Engine::get()->getMemoryPool<Texture>()->getAll();

	if (ImGui::TreeNode("Textures")) {
		for (int i = 0; i < textureList.size(); i++) {
			if (ImGui::Selectable(textureList[i].c_str())) {
				// Do something when a mesh is selected
			}
		}
		ImGui::TreePop();
	}
	// Render asset view content here
	ImGui::End();
}


class GUI_Helper : public GuiMenu {
	// Inherited via GuiMenu
	virtual void display() override {

		RenderCommand::clear();

		// Get the total screen size
		int screenWidth = Engine::get()->getWindow()->getWidth();
		int screenHeight = Engine::get()->getWindow()->getHeight();

		ImGui::StyleColorsDark(); // Change the style to a dark theme


		// Render menu bar
		if (ImGui::BeginMainMenuBar()) 
		{
			if (ImGui::BeginMainMenuBar()) 
			{
				if (ImGui::BeginMenu("File")) 
				{ // Start of File dropdown
					if (ImGui::MenuItem("Open Project", "Ctrl+O")) {


						const char* lTheSelectFolderName = tinyfd_selectFolderDialog(
							"let us just select a directory", "../../");

						//nfdchar_t* outPath = NULL;
						//nfdresult_t result = NFD_PickFolder(NULL, &outPath);
						if (lTheSelectFolderName)
						{
							Engine::get()->loadProject(lTheSelectFolderName);
							selectedEntity = Entity::EmptyEntity;
							updateScene();
						}
					}
					if (ImGui::MenuItem("Save Project", "Ctrl+S")) {
						Engine::get()->saveProject();
						
					}
					if (ImGui::MenuItem("Import")) {
						showModelCreatorWindow = true;
					}
					ImGui::Separator(); // Optional: Add a separator
					if (ImGui::MenuItem("Quit", "Alt+F4")) {
						Engine::get()->stop();
					}
					ImGui::EndMenu(); // End of File dropdown
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
			
			ImGui::EndMainMenuBar();
		}

		// Render UI
		RenderSimulationControlView(screenWidth, screenHeight);
		RenderViewWindow(screenWidth, screenHeight);
		RenderSceneHierarchyWindow(screenWidth, screenHeight);
		RenderInspectorWindow(screenWidth, screenHeight);
		RenderAssetViewWindow(screenWidth, screenHeight); // Add the Asset View window

		ShowModelCreatorWindow();
		
	}
};





class EditorApp : public Application
{
public:

	void start() override
	{
		
		ImGui::SetCurrentContext((ImGuiContext * )Engine::get()->getImguiHandler()->getCurrentContext());

		auto scene = Engine::get()->getContext()->getActiveScene();

		// store Default scene camera
		g_primaryCamera = scene->getActiveCamera();

		// set Editor camera as active camera
		auto editorCamera = scene->createEntity("Editor Camera");
		editorCamera.addComponent<CameraComponent>();
		editorCamera.addComponent<NativeScriptComponent>().bind<EditorCamera>();
		editorCamera.RemoveComponent<ObjectComponent>();
		scene->setPrimaryCamera(editorCamera);

		g_editorCamera = editorCamera;

		

		updateScene();

		auto gui = new GUI_Helper();
		Engine::get()->getImguiHandler()->addGUI(gui);
	}
};

Application* CreateApplication()
{
	return new EditorApp();
}