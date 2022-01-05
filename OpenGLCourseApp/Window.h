#pragma once

#include <stdio.h>

#include "SDL_syswm.h"
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include "Utils/Logger/Logger.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

class Window
{
public:

	Window(GLint width = SCREEN_WIDTH, GLint height = SCREEN_HEIGHT);

	int initialize();

	inline GLint getWidth() { return m_width; }
	inline GLint getHeight() { return m_height; }

	void Close() {

		// Delete context
		SDL_GL_DeleteContext(m_glContext);

		//Destroy window
		SDL_DestroyWindow(m_mainWindow);

		//Quit SDL subsystems
		SDL_Quit();

		m_mainWindow = NULL;
	}

	void SwapBuffer() { SDL_GL_SwapWindow(m_mainWindow); }

	SDL_Window* GetWindow() { return m_mainWindow; }
	SDL_GLContext GetContext() { return m_glContext; }

	void* GetNativeWindow();

	~Window();
private:
	SDL_Window* m_mainWindow = nullptr;
	SDL_GLContext m_glContext = nullptr;

	GLint m_width = 0;
	GLint m_height = 0;

	int m_mousePreviousX = 0;
	int m_mousePreviousY = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

	bool m_mouseFirstMoved = false;
};
