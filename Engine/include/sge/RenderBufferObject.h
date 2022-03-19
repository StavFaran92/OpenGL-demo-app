#pragma once
#include <cstdint>
#include "GL/glew.h"
class RenderBufferObject
{
public:
	RenderBufferObject();

	uint32_t GetID() { return m_id; }

	void Bind();
	void Unbind();
private:
	uint32_t m_id;
};
