#pragma once

#include <memory>
#include "Core.h"
#include "Material.h"

class EngineAPI DefaultMaterial : public Material
{
public:
	DefaultMaterial(float shine);

	void use(Shader& shader) override;
	void release() override;

	~DefaultMaterial();

private:

};
