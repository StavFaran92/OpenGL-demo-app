#pragma once
#include <memory>

#include "Core.h"
#include "SDL_scancode.h"

class Model;
class Shader;
class Skybox;
class Box;
class Quad;
class Sphere;

class EngineAPI Application
{
public:
	virtual void start() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;
	virtual void close() {};

protected:
	/** API */


	void update(Model* model);
	void draw(Model* model);
	Box* createBox();
	Quad* createQuad();
	void postProcess(Shader*);
	void skybox(std::shared_ptr<Skybox> skybox);

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



