#include "TerrainSystem.h"

#include "Shader.h"
#include "Engine.h"

#include <GL/glew.h>

void TerrainSystem::init()
{
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	auto terrainShader = Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/terrainShader.glsl");


}
