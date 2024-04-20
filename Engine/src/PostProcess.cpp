#include "PostProcess.h"

#include "Engine.h"

#include "Shader.h"

Shader* PostProcess::inversion()
{ 
	return Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/PostProcess/PostProcessShader_inversion.glsl");
}
Shader* PostProcess::grayscale()
{
	return Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/PostProcess/PostProcessShader_grayscale.glsl");
}
