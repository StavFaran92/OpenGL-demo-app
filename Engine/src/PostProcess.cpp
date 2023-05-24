#include "PostProcess.h"

#include "Shader.h"

Shader* PostProcess::inversion()
{
	return Shader::create<Shader>("Resources/Engine/Shaders/PostProcess/PostProcessShader_inversion.vert", "Resources/Engine/Shaders/PostProcess/PostProcessShader_inversion.frag");
}
Shader* PostProcess::grayscale()
{
	return Shader::create<Shader>("Resources/Engine/Shaders/PostProcess/PostProcessShader_grayscale.vert", "Resources/Engine/Shaders/PostProcess/PostProcessShader_grayscale.frag");
}
