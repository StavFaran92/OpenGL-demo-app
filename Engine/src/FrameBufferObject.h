#pragma once
#include <cstdint>

class FrameBufferObject
{
public:
	enum class AttachmentType
	{
		Color,
		Stencil,
		Depth,
		Depth_Stencil
	};
public:
	FrameBufferObject();
	~FrameBufferObject();

	void bind();
	void unbind();
	bool isComplete();

	void attachRenderBuffer(uint32_t renderBufferID, AttachmentType type);
	void attachTexture(uint32_t textureID);

private:
	uint32_t m_id = 0;

	static uint32_t s_boundFBO;
};

