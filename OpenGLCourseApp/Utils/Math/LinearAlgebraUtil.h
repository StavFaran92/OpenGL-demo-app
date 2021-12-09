#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void calcAverageNormal(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices,
	unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset);