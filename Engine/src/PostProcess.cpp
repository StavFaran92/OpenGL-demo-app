#include "PostProcess.h"

#include "Shader.h"

Shader* PostProcess::inversion()
{
	return new Shader("Resources\\Shaders\\PostProcess\\PostProcessShader_inversion.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_inversion.frag");
}
Shader* PostProcess::grayscale()
{
	return new Shader("Resources\\Shaders\\PostProcess\\PostProcessShader_grayscale.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_grayscale.frag");
}
