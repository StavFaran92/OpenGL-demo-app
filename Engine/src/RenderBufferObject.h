#pragma once
#include <cstdint>

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
