#include "Window.h"

Window::Window(GLint width, GLint height) :
	m_width(width), m_height(height)
{}

int Window::initialize()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		logError("SDL could not initialize! SDL Error: {}", SDL_GetError());
		return false;
	}

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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

Window::~Window() {
	Close();
}