#include "Window.h"

Window::Window(GLint width, GLint height):
	m_width(width), m_height(height)
{}

int Window::initialize() 
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		m_mainWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (!m_mainWindow)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			m_glContext = SDL_GL_CreateContext(m_mainWindow);
			if (m_glContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				////Initialize OpenGL
				//if (!initGL())
				//{
				//	printf("Unable to initialize OpenGL!\n");
				//	success = false;
				//}
			}
		}
	}

	return success;
}

//void Window::SetWindowCallbacks()
//{
//	glfwSetKeyCallback(m_mainWindow, handleKeys);
//	glfwSetCursorPosCallback(m_mainWindow, handleMouse);
//}





//void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
//{
//	Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
//
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, GL_TRUE);
//	}
//
//	if (key >= 0 && key < 1024) {
//		if (action == GLFW_PRESS)
//		{
//			myWindow->keys[key] = true;
//			printf("Pressed %d\n", key);
//		}
//		else if (action == GLFW_RELEASE)
//		{
//			myWindow->keys[key] = false;
//			printf("Released %d\n", key);
//		}
//	}
//}
//
//void Window::handleMouse(double xPos, double yPos)
//{
//
//	if (m_mouseFirstMoved)
//	{
//		m_mousePreviousX = xPos;
//		m_mousePreviousY = yPos;
//		m_mouseFirstMoved = false;
//	}
//
//	int xChange = xPos - m_mousePreviousX;
//	int yChange = m_mousePreviousY - yPos;
//
//	m_mousePreviousX = xPos;
//	m_mousePreviousY = yPos;
//}



Window::~Window() {
	Close();
}