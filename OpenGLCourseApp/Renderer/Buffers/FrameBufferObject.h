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

	void Bind();
	void Unbind();
	bool IsComplete();

	void AttachRenderBuffer(uint32_t renderBufferID, AttachmentType type);
	void AttachTexture(uint32_t textureID);

private:
	uint32_t m_id = 0;
};

