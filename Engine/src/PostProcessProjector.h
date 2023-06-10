#pragma once

#include <memory>

#include "IProjector.h"
#include "ObjectHandler.h"

class FrameBufferObject;
class RenderBufferObject;
class Shader;
class ScreenQuad;
class IRenderer;
class TextureHandler;
class Scene;
class Entity;

class PostProcessProjector : public IProjector
{
public:
	PostProcessProjector(Scene* scene);
	bool init(int windowWidth, int windowHeight) override;
	void enableWriting() override;
	void disableWriting() override;
	void draw();
	void setPostProcessShader(std::shared_ptr<Shader> shader);
	bool isEnabled() const;
	void setEnabled(bool enable);

private:
	std::shared_ptr<FrameBufferObject> m_frameBuffer = nullptr;
	std::shared_ptr<RenderBufferObject> m_renderBuffer = nullptr;
	std::shared_ptr<Shader> m_screenShader = nullptr;
	std::shared_ptr<Entity> m_quad;
	std::shared_ptr<IRenderer> m_renderer = nullptr;
	std::shared_ptr<TextureHandler> m_textureHandler = nullptr;
	Scene* m_scene = nullptr;
	bool m_isEnabled = false;
};

