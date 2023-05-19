#pragma once

#include <memory>
#include "Core.h"
#include "Material.h"

class Shader;

class EngineAPI DefaultMaterial : public Material
{
public:
	DefaultMaterial();
	DefaultMaterial(float shine);

	void UseMaterial(Shader& shader);

	~DefaultMaterial();


};
