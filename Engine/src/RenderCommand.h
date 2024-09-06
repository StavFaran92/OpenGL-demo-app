#pragma once

#include <memory>
#include "VertexArrayObject.h"
#include "Core.h"

#include "gl/glew.h"

class EngineAPI RenderCommand
{
public:
	static void clear();

	static void draw(const VertexArrayObject* vao);

	static void drawPatches(const VertexArrayObject* vao);

	static void drawInstanced(const VertexArrayObject* vao, int count);
};