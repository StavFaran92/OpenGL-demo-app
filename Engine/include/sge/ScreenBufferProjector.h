#pragma once

#include <memory>

class FrameBufferObject;
class RenderBufferObject;
class Shader;
class ScreenQuad;
class IRenderer;
class Texture;

class ScreenBufferProjector
{
public:
	bool init();
	void RedirectToFrameBuffer();
	void RedirectToDefault();
	void setPostProcessShader(std::shared_ptr<Shader> shader);
private:
	std::shared_ptr<FrameBufferObject> m_frameBuffer = nullptr;
	std::shared_ptr<RenderBufferObject> m_renderBuffer = nullptr;
	std::shared_ptr<Shader> m_screenShader = nullptr;
	std::shared_ptr<ScreenQuad> m_quad = nullptr;
	std::shared_ptr<IRenderer> m_renderer = nullptr;
	std::shared_ptr<Texture> m_texture = nullptr;
};

