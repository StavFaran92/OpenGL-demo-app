#include "PostProcess.h"

#include "Shader.h"

Shader* PostProcess::inversion()
{
	return Shader::create<Shader>("Resources\\Shaders\\PostProcess\\PostProcessShader_inversion.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_inversion.frag");
}
Shader* PostProcess::grayscale()
{
	return Shader::create<Shader>("Resources\\Shaders\\PostProcess\\PostProcessShader_grayscale.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_grayscale.frag");
}
