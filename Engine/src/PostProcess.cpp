#include "PostProcess.h"

#include "ShaderBuilder.h"
#include "Shader.h"

Shader* PostProcess::inversion()
{
	return ShaderBuilder::buildShader<Shader>("Resources\\Shaders\\PostProcess\\PostProcessShader_inversion.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_inversion.frag");
}
Shader* PostProcess::grayscale()
{
	return ShaderBuilder::buildShader<Shader>("Resources\\Shaders\\PostProcess\\PostProcessShader_grayscale.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_grayscale.frag");
}
