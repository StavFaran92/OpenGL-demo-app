#include "ImguiHandler.h"

bool ImguiHandler::Init(SDL_Window* window, const SDL_GLContext& context)
{
	const char* glsl_version = "#version 140";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

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
}

void ImguiHandler::Render()
{
	bool show_demo_window = true;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	ImGui::ShowDemoWindow(&show_demo_window);
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
