#include "RubiksCubeController.h"

#include "Engine.h"
#include "Input.h"
#include "EventSystem.h"

#include "RubiksCube.h"
#include "RubiksCubeFace.h"
#include "RubiksCubeEnt.h"

void RubiksCubeController::init(RubiksCube* rubiksCube)
{
	m_rubiksCube = rubiksCube;

	m_rubiksCube->setController(this);

	Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_X, [&](SDL_Event e)
	{
		if(!m_pickedEnt)
		{
			std::cout << "Warning: No Entity selected." << std::endl;
			return;
		}


		m_face = m_pickedEnt->getFaceData(Axis::X).face;

		Engine::get()->getContext()->getActiveScene()->clearObjectSelection();

		m_face->select();
	});

	Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_Y, [&](SDL_Event e)
	{
		if (!m_pickedEnt)
		{
			std::cout << "Warning: No Entity selected." << std::endl;
			return;
		}

		m_face = m_pickedEnt->getFaceData(Axis::Y).face;

		Engine::get()->getContext()->getActiveScene()->clearObjectSelection();

		m_face->select();
	});

	Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_Z, [&](SDL_Event e)
	{
		if (!m_pickedEnt)
		{
			std::cout << "Warning: No Entity selected." << std::endl;
			return;
		}

		m_face = m_pickedEnt->getFaceData(Axis::Z).face;

		Engine::get()->getContext()->getActiveScene()->clearObjectSelection();

		m_face->select();
	});

	Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_RIGHT, [&](SDL_Event e)
	{
		if (!m_face)
		{
			std::cout << "Warning: No face selected." << std::endl;
			return;
		}

		m_rubiksCube->rotateFace(m_face->getAxis(), m_face->getIndex(), Shift::CW);
	});

	Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_LEFT, [&](SDL_Event e)
	{
		if (!m_face)
		{
			std::cout << "Warning: No face selected." << std::endl;
			return;
		}

		m_rubiksCube->rotateFace(m_face->getAxis(), m_face->getIndex(), Shift::CCW);
	});

	Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_I, [&](SDL_Event e)
	{
		m_rubiksCube->print();
	});
}

void RubiksCubeController::setFace(RubiksCubeFace* face)
{
	m_face = face;
}

void RubiksCubeController::setPickedEnt(RubiksCubeEnt* ent)
{
	m_pickedEnt = ent;
}
