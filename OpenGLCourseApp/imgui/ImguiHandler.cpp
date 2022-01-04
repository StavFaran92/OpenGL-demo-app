#include "ImguiHandler.h"

#include "GUI/Menu.h"

bool ImguiHandler::Init(SDL_Window* window, const SDL_GLContext& context)
{
	const char* glsl_version = "#version 140";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsLight();

	if (!ImGui_ImplSDL2_InitForOpenGL(window, context))
	{
		logError("Init Imgui_SDL failed.");
		return false;
	}
	if(!ImGui_ImplOpenGL3_Init(glsl_version))
	{
		logError("Init Imgui_OpenGL failed.");
		return false;
	}

	logInfo("Imgui has initialized successfully.");

    return true;
}

void ImguiHandler::ProccessEvents(SDL_Event& e)
{
	ImGui_ImplSDL2_ProcessEvent(&e);
}

void ImguiHandler::Render()
{
	bool show_demo_window = true;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	DisplayMenu();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

bool ImguiHandler::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
    return true;
}
