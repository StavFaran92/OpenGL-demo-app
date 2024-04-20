#include "PickingShader.h"

#include "Engine.h"

PickingShader::PickingShader() : Shader(SGE_ROOT_DIR + "Resources/Engine/Shaders/PickingShader.glsl")
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
