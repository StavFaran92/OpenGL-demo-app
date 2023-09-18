#include "ShadowSystem.h"

#include "FrameBufferObject.h"
#include "TextureHandler.h"
#include "gl/glew.h"
#include "Logger.h"
#include "Scene.h"
#include "IRenderer.h"

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

bool ShadowSystem::init(Scene* scene)
{
	m_scene = scene;

	scene->addRenderCallback(Scene::RenderPhase::PRE_RENDER_BEGIN, [=](const IRenderer::Params* params) {

		renderToDepthMap(params);
	});

	m_fbo.bind();

	// Generate 2D texture
	auto textureHandler = Texture::createEmptyTexture(SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

	// Attach texture to FBO
	m_fbo.attachTexture(textureHandler->getID(), GL_DEPTH_ATTACHMENT);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (!m_fbo.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_fbo.unbind();

	return true;
}

void ShadowSystem::renderToDepthMap(const IRenderer::Params* params)
{
	// Set shadow map viewport
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Bind FBO
	m_fbo.bind();

	// Clear buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Configure Shadow pass matrices

	// Render Scene
	for (auto&& [entity, mesh, transform, renderable] :
		params->registry.view<MeshComponent, Transformation, RenderableComponent>().each())
	{
		// draw model
		params->renderer->render(params);
	};

	// Unbind FBO
	m_fbo.unbind();

	// Set viewport back to default
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}
