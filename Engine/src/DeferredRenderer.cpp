#include "DeferredRenderer.h"

#include "TextureHandler.h"
#include "gl/glew.h"
#include "Engine.h"
#include "Window.h"
#include "Logger.h"
#include "Component.h"
#include "Transformation.h"
#include "ScreenQuad.h"
#include "Scene.h"
#include "Renderer2D.h"

DeferredRenderer::DeferredRenderer(Scene* scene)
	: m_scene(scene)
{
}

bool DeferredRenderer::init()
{
	m_gBuffer.bind();

	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	// Generate Texture for Position data
	m_positionTexture = Texture::createEmptyTexture(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_gBuffer.attachTexture(m_positionTexture->getID(), GL_COLOR_ATTACHMENT0);

	// Generate Texture for Normal data
	m_normalTexture = Texture::createEmptyTexture(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_gBuffer.attachTexture(m_normalTexture->getID(), GL_COLOR_ATTACHMENT1);

	// Generate Texture for Albedo + Specular
	m_albedoSpecularTexture = Texture::createEmptyTexture(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	m_gBuffer.attachTexture(m_albedoSpecularTexture->getID(), GL_COLOR_ATTACHMENT2);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Create RBO and attach to FBO
	m_gBuffer.attachRenderBuffer(m_renderBuffer.GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	if (!m_gBuffer.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_gBuffer.unbind();

	// Generate screen quad
	m_quad = ScreenQuad::GenerateScreenQuad(m_scene);

	// Generate screen shader
	m_screenShader = Shader::createShared<Shader>("Resources/Engine/Shaders/PostProcess/PostProcessShader_default.vert", "Resources/Engine/Shaders/PostProcess/PostProcessShader_default.frag");

	// Generate screen renderer
	m_2DRenderer = std::make_shared<Renderer2D>();

    return true;
}

void DeferredRenderer::draw(const VertexArrayObject& vao) const
{
}

void DeferredRenderer::clear() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void DeferredRenderer::render(const DrawQueueRenderParams& renderParams)
{
}

void DeferredRenderer::renderScene(DrawQueueRenderParams& renderParams)
{
	// clear color and buffers
	clear();

	// bind gBuffer
	m_gBuffer.bind();

	// bind vShader 
	m_gBufferShader.use();

	// Render all objects
	for (auto&& [entity, mesh, transform, renderable] :
		renderParams.registry->view<MeshComponent, Transformation, RenderableComponent>().each())
	{
		Entity entityhandler{ entity, renderParams.scene };
		renderParams.entity = &entityhandler;
		renderParams.mesh = mesh.mesh.get();
		auto tempModel = transform.getWorldTransformation();
		renderParams.model = &tempModel;

		// draw model
		render(renderParams);

		renderParams.entity = nullptr;
		renderParams.mesh = nullptr;
		renderParams.model = nullptr;
	};

	// unbind gBuffer
	m_gBuffer.unbind();

	// bind textures
	// Todo solve slots issue
	m_positionTexture->bind();
	m_normalTexture->bind();
	m_albedoSpecularTexture->bind();

	// bind fShader
	m_lightPassShader.use();

	// render to quad
	auto& mesh = m_quad.getComponent<MeshComponent>();

	DrawQueueRenderParams renderParams2D;
	renderParams2D.mesh = mesh.mesh.get();
	renderParams2D.shader = m_screenShader.get();
	m_2DRenderer->render(renderParams2D);

	m_positionTexture->unbind();
	m_normalTexture->unbind();
	m_albedoSpecularTexture->unbind();
}
