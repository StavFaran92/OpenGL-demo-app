#pragma once

#include <stdio.h>

#include "SDL_syswm.h"


#include "Logger.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

class Window
{
public:

	Window(int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT);

	int init();

	inline int getWidth() { return m_width; }
	inline int getHeight() { return m_height; }

	void close();

	void SwapBuffer() { SDL_GL_SwapWindow(m_mainWindow); }

	SDL_Window* GetWindow() { return m_mainWindow; }
	SDL_GLContext GetContext() { return m_glContext; }

	void* GetNativeWindow();

	~Window();
private:
	SDL_Window* m_mainWindow = nullptr;
	SDL_GLContext m_glContext = nullptr;

	int m_width = 0;
	int m_height = 0;

	int m_mousePreviousX = 0;
	int m_mousePreviousY = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

	bool m_mouseFirstMoved = false;
};
