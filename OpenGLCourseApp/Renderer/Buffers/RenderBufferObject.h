#pragma once
#include <cstdint>
#include "GL/glew.h"
class RenderBufferObject
{
public:
	RenderBufferObject();

	void Bind();
	void Unbind();
private:
	uint32_t m_id;
};
