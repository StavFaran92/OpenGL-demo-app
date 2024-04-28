#include "EntryPoint.h"
#include "sge.h"

#include "Window.h"
#include "commdlg.h"


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
	for (auto&& [entity, obj] : Engine::get()->getContext()->getActiveScene()->getRegistry().view<ObjectComponent>().each())
	{
		sceneObjects.emplace_back(SceneObject{ obj.name, obj.e });
	}
}

static bool ShowLightCreatorWindow = false;
static bool showModelCreatorWindow = false;
static bool showModelInspectorWindow = false;
static bool showPrimitiveCreatorWindow = false;

Entity  selectedEntity = Entity::EmptyEntity;

auto style = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse;

static const char* g_supportedFormats = "All formats (*.obj *.blend *.fbx *.dae)\0*.obj;*.blend;*.fbx;*.dae\0obj files (*.obj)\0*.obj\0Blender 3D (*.blend)\0*.blend\0Autodesk 3D (*.fbx)\0*.fbx\0Collada (*dae)\0*.dae\0";
static const char* g_supportedTextureFormats = "All formats (*.png *.jpg)\0";


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

enum class PrimitiveType
{
	Quad,
	Cube,
	Sphere

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
		static PrimitiveType shape = PrimitiveType::Quad;
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
			auto filePath = OpenFile(g_supportedFormats);
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

			//logInfo("Open file: " + path);

			//auto texture = Texture::loadTextureFromFile(texturePath.c_str(), flipTexture);

			Entity entity;
			if (shape == PrimitiveType::Quad)
			{
				//entity = ModelBuilder::builder<Quad>().build();
			}
			else if (shape == PrimitiveType::Cube)
			{
				entity = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			}

			assert(entity);

			if (entity.valid())
			{
				entity.getComponent<Transformation>().setLocalPosition(pos);
				entity.getComponent<Transformation>().setLocalScale(scale);

				//logInfo("Added Entity successfully.");
			}

			showPrimitiveCreatorWindow = false;

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
		ImGui::Begin("Model Creator", false, ImGuiWindowFlags_AlwaysAutoResize);

		static ImGuiTextBuffer modelPathBuffer;
		static ImGuiTextBuffer textureAlbedoPathBuffer;
		static ImGuiTextBuffer textureSpecularPathBuffer;
		static ImGuiTextBuffer textureNormalPathBuffer;
		static ImGuiTextBuffer textureMetallicPathBuffer;
		static ImGuiTextBuffer textureRoughnessPathBuffer;
		static ImGuiTextBuffer textureAmbientOcclusionPathBuffer;

		static glm::vec3 pos(0.f, 0.f, 0.f);
		static glm::vec3 rotation(0.f, 0.f, 0.f);
		static glm::vec3 scale(1.f, 1.f, 1.f);

		// Model
		addAssetLoadWidget("Model", modelPathBuffer, g_supportedFormats);

		ImGui::Separator();
		ImGui::LabelText("", "Texture");

		// Textures
		addAssetLoadWidget("Albedo", textureAlbedoPathBuffer, g_supportedTextureFormats);
		//addAssetLoadWidget("Specular", textureSpecularPathBuffer, g_supportedTextureFormats);
		addAssetLoadWidget("Normal", textureNormalPathBuffer, g_supportedTextureFormats);
		addAssetLoadWidget("Metallic", textureMetallicPathBuffer, g_supportedTextureFormats);
		addAssetLoadWidget("Roghuness", textureRoughnessPathBuffer, g_supportedTextureFormats);
		addAssetLoadWidget("Occlusion", textureAmbientOcclusionPathBuffer, g_supportedTextureFormats);

		ImGui::Separator();

		ImGui::LabelText("", "Transformation");
		ImGui::InputFloat3("Position", (float*)&pos);
		ImGui::InputFloat3("Rotation", (float*)&rotation);
		ImGui::InputFloat3("Scale", (float*)&scale);

		ImGui::Separator();

		if (ImGui::Button("Ok"))
		{
			//todo validate input

			//logInfo("Open file: " + path);

			auto entity = Engine::get()->getModelImporter()->loadModelFromFile(modelPathBuffer.c_str(), Engine::get()->getContext()->getActiveScene().get());

			auto& mat = entity.getComponent<Material>();

			auto albedoMap = Texture::loadTextureFromFile(textureAlbedoPathBuffer.c_str(), false);
			mat.setTexture(Texture::Type::Albedo, std::shared_ptr<TextureHandler>(albedoMap));
			auto roughnessMap = Texture::loadTextureFromFile(textureRoughnessPathBuffer.c_str(), false);
			mat.setTexture(Texture::Type::Roughness, std::shared_ptr<TextureHandler>(roughnessMap));
			auto normalMap = Texture::loadTextureFromFile(textureNormalPathBuffer.c_str(), false);
			mat.setTexture(Texture::Type::Normal, std::shared_ptr<TextureHandler>(normalMap));
			auto metallicMap = Texture::loadTextureFromFile(textureMetallicPathBuffer.c_str(), false);
			mat.setTexture(Texture::Type::Metallic, std::shared_ptr<TextureHandler>(metallicMap));
			auto aoMap = Texture::loadTextureFromFile(textureAmbientOcclusionPathBuffer.c_str(), false);
			mat.setTexture(Texture::Type::AmbientOcclusion, std::shared_ptr<TextureHandler>(aoMap));

			entity.getComponent<Transformation>().setLocalPosition(pos);
			entity.getComponent<Transformation>().setLocalScale(scale);

			showModelCreatorWindow = false;

			updateScene();

			//logInfo("Added Model successfully.");
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
				selectedEntity = obj.e;
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

static const char* rigidyBodyTypesStrList[]{ 
	"Static",
	"Dynamic",
	"Kinematic"
};


void RenderInspectorWindow(float width, float height) {
	float windowWidth = width * 0.2f - 5;
	float startX = width * 0.8f + 5; // Add a gap of 5 pixels
	ImGui::SetNextWindowPos(ImVec2(startX, 25)); // Adjust vertical position to make space for the menu bar
	ImGui::SetNextWindowSize(ImVec2(windowWidth - 5, height * 0.8f));
	ImGui::Begin("Inspector", nullptr, style);

	if (selectedEntity != Entity::EmptyEntity)
	{

		if (selectedEntity.HasComponent<Transformation>())
		{
			auto& transform = selectedEntity.getComponent<Transformation>();
			ImGui::LabelText("", "Transformation");

			glm::vec3 pos = transform.getLocalPosition();
			glm::vec3 rotation = transform.getLocalRotationVec3(); // todo fix
			glm::vec3 scale = transform.getLocalScale();

			ImGui::InputFloat3("Position", (float*)&pos);
			ImGui::InputFloat3("Rotation", (float*)&rotation);
			ImGui::InputFloat3("Scale", (float*)&scale);

			transform.setLocalPosition(pos);
			transform.setLocalRotation(rotation);
			transform.setLocalScale(scale);
		}

		ImGui::Separator();

		if (selectedEntity.HasComponent<RigidBodyComponent>())
		{
			auto& rBody = selectedEntity.getComponent<RigidBodyComponent>();
			ImGui::LabelText("", "RigidBody");

			ImGui::Combo("##Type", (int*)&rBody.type, rigidyBodyTypesStrList, IM_ARRAYSIZE(rigidyBodyTypesStrList));
			ImGui::InputFloat("Mass", &rBody.mass);
		}

		ImGui::Separator();

		if (selectedEntity.HasComponent<CollisionBoxComponent>())
		{
			auto& collisionBox = selectedEntity.getComponent<CollisionBoxComponent>();
			ImGui::LabelText("", "Collision Box");

			ImGui::InputFloat("Half Extent", &collisionBox.halfExtent);
		}

		ImGui::Separator();

		if (selectedEntity.HasComponent<CollisionSphereComponent>())
		{
			auto& collisionBox = selectedEntity.getComponent<CollisionSphereComponent>();
			ImGui::LabelText("", "Collision Sphere");

			ImGui::InputFloat("Radius", &collisionBox.radius);
		}

		ImGui::Separator();
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
		if (ImGui::BeginMainMenuBar()) 
		{
			if (ImGui::BeginMainMenuBar()) 
			{
				if (ImGui::BeginMenu("File")) 
				{ // Start of File dropdown
					if (ImGui::MenuItem("Open...", "Ctrl+O")) {
						// OpenFile();
					}
					if (ImGui::MenuItem("Save As..", "Ctrl+S")) {
						// SaveFile();
					}
					if (ImGui::MenuItem("Load Model")) {
						showModelCreatorWindow = true;
					}
					if (ImGui::MenuItem("Add Light")) {
						ShowLightCreatorWindow = true;
					}
					if (ImGui::MenuItem("Add Primitive")) {
						showPrimitiveCreatorWindow = true;
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
		RenderSceneHierarchyWindow(screenWidth, screenHeight);
		RenderViewWindow(screenWidth, screenHeight);
		RenderInspectorWindow(screenWidth, screenHeight);
		RenderAssetViewWindow(screenWidth, screenHeight); // Add the Asset View window

		ShowModelCreatorWindow();
		LightCreatorWindow();
		ShowPrimitiveCreatorWindow();
	}
};





class EditorApp : public Application
{
public:

	void start() override
	{
		
		ImGui::SetCurrentContext((ImGuiContext * )Engine::get()->getImguiHandler()->getCurrentContext());

		Skybox::CreateSkybox({ SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/right.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/left.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/top.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/bottom.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/front.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/back.jpg" });

		auto ground = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		auto& groundTransfrom = ground.getComponent<Transformation>();
		groundTransfrom.setLocalScale({ 50, .5f, 50 });
		auto& mat = ground.addComponent<Material>();
		auto tex = Texture::loadTextureFromFile(SGE_ROOT_DIR + "Resources/Engine/Textures/floor.jpg");
		mat.setTexture(Texture::Type::Albedo, std::shared_ptr<TextureHandler>(tex));
		auto& rb = ground.addComponent<RigidBodyComponent>(RigidbodyType::Static, 1.f);
		auto& collisionBox = ground.addComponent<CollisionBoxComponent>(.5f);

		auto camera = Engine::get()->getContext()->getActiveScene()->getActiveCamera();
		camera->lookAt(0, 5, 0);
		camera->setPosition(25, 225, 35);

		auto sphere = ShapeFactory::createSphere(Engine::get()->getContext()->getActiveScene().get());
		{
			auto random = Engine::get()->getRandomSystem();
			auto x = random->rand() * 10 - 5;
			auto z = random->rand() * 10 - 5;

			auto& sphereTransform = sphere.getComponent<Transformation>();
			sphereTransform.setLocalPosition({ x, 10, z });


			auto& mat = sphere.addComponent<Material>();
			auto tex = Texture::loadTextureFromFile("Resources/Content/Textures/checkers.jpg");
			mat.setTexture(Texture::Type::Diffuse, std::shared_ptr<TextureHandler>(tex));

			auto& rb = sphere.addComponent<RigidBodyComponent>(RigidbodyType::Dynamic, 1.f);
			auto& collisionBox = sphere.addComponent<CollisionSphereComponent>(1.f);
		}

		updateScene();

		auto gui = new GUI_Helper();
		Engine::get()->getImguiHandler()->addGUI(gui);
	}
};

Application* CreateApplication()
{
	return new EditorApp();
}