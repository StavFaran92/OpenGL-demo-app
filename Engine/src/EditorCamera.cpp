#include "EditorCamera.h"

#include <gl/glew.h>

#include "ApplicationConstants.h"
#include "Engine.h"
#include "Context.h"
#include "ObjectSelection.h"
#include "Scene.h"
#include "EventSystem.h"
#include "Transformation.h"
#include "CameraControllerOrbit.h"
#include "CameraControllerFreeLook.h"

#include "Logger.h"

EditorCamera::EditorCamera()
{
	m_cameraController = std::make_shared<CameraControllerOrbit>();
}

glm::mat4 EditorCamera::getView()
{
	//auto pos = entity.getComponent<Transformation>().getWorldPosition();
	//auto rot = entity.getComponent<Transformation>().getWorldRotation();

	//// Calculate the target point based on the forward direction of the quaternion
	//glm::vec3 forward = glm::normalize(rot * glm::vec3(0, 0, 1));
	//glm::vec3 target = pos + forward;

	//// Use glm::lookAt() to create the view matrix
	//return glm::lookAt(pos, target, glm::vec3(0, 1, 0));

	return m_cameraComponent->getView();
}

glm::vec3 EditorCamera::getPosition()
{
	//return entity.getComponent<Transformation>().getWorldPosition();

	return m_cameraComponent->getPosition();
}

void EditorCamera::lookAt(float x, float y, float z)
{
	
	//entity.getComponent<Transformation>().setLocalRotation(glm::quat(getPosition(), { x,y,z }));

	//m_cameraComponent->center = { x, y, z };

	//calculateOrientation();
}

void EditorCamera::setPosition(float x, float y, float z)
{
	//entity.getComponent<Transformation>().setWorldPosition({ x, y, z } );

	//m_distance = distance;
	//m_angleX = angleX;
	//m_angleY = angleY;

	//calculateOrientation();
}

void EditorCamera::onCreate()
{
	m_cameraComponent = &entity.getComponent<CameraComponent>();

	m_cameraController->onCreate(m_cameraComponent);

	m_cameraController->calculateOrientation();


}