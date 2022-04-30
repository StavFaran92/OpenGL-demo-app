#pragma once

#include <memory>

#include "IProjector.h"

class FrameBufferObject;
class RenderBufferObject;
class Shader;
class ScreenQuad;
class IRenderer;
class TextureHandler;

class PostProcessProjector : public IProjector
{
public:
	bool init(int windowWidth, int windowHeight) override;
	void enableWriting() override;
	void disableWriting() override;
	void draw();
	void setPostProcessShader(std::shared_ptr<Shader> shader);

private:
	std::shared_ptr<FrameBufferObject> m_frameBuffer = nullptr;
	std::shared_ptr<RenderBufferObject> m_renderBuffer = nullptr;
	std::shared_ptr<Shader> m_screenShader = nullptr;
	std::shared_ptr<ScreenQuad> m_quad = nullptr;
	std::shared_ptr<IRenderer> m_renderer = nullptr;
	std::shared_ptr<TextureHandler> m_textureHandler = nullptr;
};

