#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Constants {
	static const float PI = 3.14159265358979323846f;
	static const float toRadians = PI / 180;

	static const std::string g_textureDiffuse = "texture_diffuse";
	static const std::string g_textureSpecular = "texture_specular";
	static const std::string g_textureNormal = "texture_normal";

	static const glm::vec3 VEC3_ONE = glm::vec3(1.0f, 1.0f, 1.0f);
	static const glm::vec3 VEC3_ZERO = glm::vec3(0.0f, 0.0f, 0.0f);

	static const char* g_supportedFormats = "All formats (*.obj *.blend *.fbx *.dae)\0*.obj;*.blend;*.fbx;*.dae\0obj files (*.obj)\0*.obj\0Blender 3D (*.blend)\0*.blend\0Autodesk 3D (*.fbx)\0*.fbx\0Collada (*dae)\0*.dae\0";

	static const std::string N_A = "N/A";
}