#include "Window.h"

#include <GL/glew.h>
#include "SDL2/SDL.h"


#include "Logger.h"

Window::Window(int width, int height) :
	m_width(width), m_height(height)
{}

int Window::init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		logError("SDL could not initialize! SDL Error: {}", SDL_GetError());
		return false;
	}

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	//Create window
	m_mainWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
	if (!m_mainWindow)
	{
		logError("Window could not be created! SDL Error: {}", SDL_GetError());
		return false;
	}
	
	//Create context
	m_glContext = SDL_GL_CreateContext(m_mainWindow);
	if (!m_glContext)
	{
		logError("OpenGL context could not be created! SDL Error: {}", SDL_GetError());
		return false;
	}

	if (SDL_GL_MakeCurrent(m_mainWindow, m_glContext) != 0)
	{
		logError("Make current context failed! SDL Error: {}", SDL_GetError());
		return false;
	}
	
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		logError("Error initializing GLEW! {}", glewGetErrorString(glewError));
		return false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		logError("Warning: Unable to set VSync! SDL Error: {}", SDL_GetError());
		return false;
	}
		
	logInfo("SDL has initialized successfully.");


	return true;
}

void Window::close()
{

	// Delete context
	SDL_GL_DeleteContext(m_glContext);

	//Destroy window
	SDL_DestroyWindow(m_mainWindow);

	//Quit SDL subsystems
	SDL_Quit();

	m_mainWindow = NULL;
}

void* Window::GetNativeWindow()
{
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(m_mainWindow, &wmInfo);
	HWND hwnd = wmInfo.info.win.window;
	return hwnd;
}

Window::~Window() {
	close();
}