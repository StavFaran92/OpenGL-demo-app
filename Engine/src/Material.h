#pragma once

#include <memory>
#include "Core.h"

class Shader;

class EngineAPI Material
{
public:
	Material();
	Material(float shine);

	void UseMaterial(Shader& shader);

	~Material();

private:
	float m_specularIntensity;
	float m_shininess;
};
