#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Constants {
	const float PI = 3.1315265f;
	const float toRadians = PI / 180;

	const std::string g_textureDiffuse = "texture_diffuse";
	const std::string g_textureSpecular = "texture_specular";

	const glm::vec3 VEC3_ONE = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 VEC3_ZERO = glm::vec3(0.0f, 0.0f, 0.0f);
}