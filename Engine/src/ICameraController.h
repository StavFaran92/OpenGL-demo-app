#pragma once

class CameraComponent;

class ICameraController
{
public:
	virtual void onCreate(CameraComponent* cameraComponent) = 0;
	virtual void calculateOrientation() = 0;
	virtual ~ICameraController() = default;
};