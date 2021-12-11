#pragma once

#include <stdio.h>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>


static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;



class Window
{
public:
	

	Window(GLint width = SCREEN_WIDTH, GLint height = SCREEN_HEIGHT);

	int initialize();

	GLint getBufferWidth() { return m_bufferWidth; }
	GLint getBufferHeight() { return m_bufferHeight; }

	void Close() { 

		//Destroy window	
		SDL_DestroyWindow(m_mainWindow);
		m_mainWindow = NULL;
	}

	void SwapBuffer() { SDL_GL_SwapWindow(m_mainWindow); }

	SDL_Window* GetWindow() { return m_mainWindow; }

	~Window();
private:
	void SetWindowCallbacks();

	static void handleKeys(SDL_Window* window, int key, int code, int action, int mode);
	static void handleMouse(SDL_Window* window, double xPos, double yPos);
private:
	SDL_Window* m_mainWindow = nullptr;
	SDL_GLContext m_glContext = nullptr;

	GLint m_bufferWidth = 0;
	GLint m_bufferHeight = 0;
	GLint m_width = 0;
	GLint m_height = 0;

	int m_mousePreviousX = 0;
	int m_mousePreviousY = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

	bool m_mouseFirstMoved = false;



};

