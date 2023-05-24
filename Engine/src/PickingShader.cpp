#include "PickingShader.h"

PickingShader::PickingShader() : Shader("Resources/Engine/Shaders/PickingShader.vert", "Resources/Engine/Shaders/PickingShader.frag")
{
}

void PickingShader::setObjectIndex(unsigned int index)
{
	setValue("objectIndex", index);
}

void PickingShader::setDrawIndex(unsigned int index)
{
	setValue("drawIndex", index);
}
