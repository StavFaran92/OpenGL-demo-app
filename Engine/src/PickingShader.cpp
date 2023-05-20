#include "PickingShader.h"

PickingShader::PickingShader() : Shader("Resources\\Shaders\\PickingShader.vert", "Resources\\Shaders\\PickingShader.frag")
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
