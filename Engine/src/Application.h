#pragma once
#include "Core.h"
#include "SDL_scancode.h"

class Model;
class Shader;
class Skybox;

class EngineAPI Application
{
public:
	virtual void start() = 0;
	virtual void draw() = 0;
	virtual void close() {};

protected:
	/** API */


	void draw(Model* model);
	Model* createBox();
	Model* createQuad();
	void postProcess(Shader*);
	void skybox(Skybox*);

	// Input
	int keyboard_getKeyState(SDL_Scancode code);
	bool mouse_leftButtonPressed();
	bool mouse_rightButtonPressed();
	bool mouse_middleButtonPressed();

	Application() = default;
	~Application() = default;

private:


};

Application* CreateApplication();



