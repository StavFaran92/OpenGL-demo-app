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
#include "Material.h"
#include "Random.h"

static float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

DeferredRenderer::DeferredRenderer(std::shared_ptr<FrameBufferObject> renderTarget, Scene* scene) 
	: m_renderTargetFBO(renderTarget), m_scene(scene)
{
}

bool DeferredRenderer::setupGBuffer()
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

	m_gBufferShader = Shader::createShared<Shader>(
		"Resources/Engine/Shaders/GBufferShader.vert",
		"Resources/Engine/Shaders/GBufferShader.frag");

	m_lightPassShader = Shader::createShared<Shader>(
		"Resources/Engine/Shaders/LightPassShader.vert",
		"Resources/Engine/Shaders/LightPassShader.frag");

	return true;
}

bool DeferredRenderer::setupSSAO()
{
	// Generate SSAO kernel
	m_ssaoKernel.reserve(64);
	auto rand = Engine::get()->getRandomSystem();

	for (int i = 0; i < 64; i++)
	{
		glm::vec3 sample(rand->rand() * 2, rand->rand() * 2, rand->rand());
		sample = glm::normalize(sample);
		sample *= rand->rand();
		float scale = (float)i / 64;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		m_ssaoKernel.push_back(sample);
	}

	// Generate SSAO Noise
	std::vector<glm::vec3> ssaoNoise;
	ssaoNoise.reserve(16);
	for (int i = 0; i < 16; i++)
	{
		ssaoNoise.push_back({
				rand->rand() * 2 - 1,
				rand->rand() * 2 - 1,
				0.f
			});
	}

	m_ssaoNoiseTexture = Texture::createTexture(4, 4,
		GL_RGBA16F,
		GL_RGB,
		GL_FLOAT, {
		{ GL_TEXTURE_MIN_FILTER,	GL_NEAREST	},
		{ GL_TEXTURE_MAG_FILTER,	GL_NEAREST	},
		{ GL_TEXTURE_WRAP_S,		GL_REPEAT	},
		{ GL_TEXTURE_WRAP_T,		GL_REPEAT   } },
		&ssaoNoise[0]
		);

	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	// Initialize SSAO FBO
	m_ssaoFBO.bind();

	m_ssaoColorBuffer = Texture::createEmptyTexture(width, height, GL_RED, GL_RED, GL_FLOAT);
	m_ssaoFBO.attachTexture(m_ssaoColorBuffer->getID(), GL_COLOR_ATTACHMENT0);

	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	// Create RBO and attach to FBO
	m_ssaoFBO.attachRenderBuffer(m_ssaoRenderBuffer.GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	if (!m_ssaoFBO.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_ssaoFBO.unbind();

	m_ssaoPassShader = Shader::createShared<Shader>(
		"Resources/Engine/Shaders/SSAOPassShader.vert",
		"Resources/Engine/Shaders/SSAOPassShader.frag");

	return true;
}

bool DeferredRenderer::init()
{
	setupGBuffer();

	setupSSAO();

	// Generate screen quad
	m_quad = ScreenQuad::GenerateScreenQuad(m_scene);

	// Generate screen renderer
	m_2DRenderer = std::make_shared<Renderer2D>();

	return true;
}

void DeferredRenderer::render(const DrawQueueRenderParams& renderParams)
{
	// Model
    if (renderParams.model)
    {
        renderParams.shader->setModelMatrix(*renderParams.model);
    }

    // View
    if (renderParams.view)
    {
        renderParams.shader->setViewMatrix(*renderParams.view);
    }

    // Projection
    if (renderParams.projection)
    {
        renderParams.shader->setProjectionMatrix(*renderParams.projection);
    }

    if (renderParams.material)
    {
        renderParams.material->use(*renderParams.shader);
    }

    renderParams.shader->bindUniformBlockToBindPoint("Time", 0);
    renderParams.shader->bindUniformBlockToBindPoint("Lights", 1);

	// Draw
	draw(*renderParams.mesh->getVAO());

	// Release
	if (renderParams.material)
	{
		renderParams.material->release();
	}

	//renderParams.shader->release();
}

void DeferredRenderer::renderScene(DrawQueueRenderParams& renderParams)
{
	glEnable(GL_DEPTH_TEST);

	// bind gBuffer
	m_gBuffer.bind();

	// clear color and buffers
	clear();

	// bind vShader 
	m_gBufferShader->use();

	// Render all objects
	for (auto& entityHandler : *renderParams.entityGroup)
	{
		renderParams.entity = &entityHandler;
		renderParams.mesh = entityHandler.getComponent<MeshComponent>().mesh.get();
		auto tempModel = entityHandler.getComponent<Transformation>().getWorldTransformation();
		renderParams.model = &tempModel;
		renderParams.shader = m_gBufferShader.get();
		Material* mat = renderParams.entity->tryGetComponentInParent<Material>();

		if (mat)
		{
			renderParams.material = mat;
		}

		// draw model
		render(renderParams);

		renderParams.entity = nullptr;
		renderParams.mesh = nullptr;
		renderParams.model = nullptr;
	};

	// unbind gBuffer
	m_gBuffer.unbind();

	m_ssaoFBO.bind();

	glDisable(GL_DEPTH_TEST);

	// SSAO
	m_positionTexture->setSlot(0);
	m_positionTexture->bind();
	m_ssaoPassShader->setValue("gPosition", 0);

	m_normalTexture->setSlot(1);
	m_normalTexture->bind();
	m_ssaoPassShader->setValue("gNormal", 1);

	m_ssaoNoiseTexture->setSlot(2);
	m_ssaoNoiseTexture->bind();
	m_ssaoPassShader->setValue("gSSAONoise", 2);

	// TODO remove
	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	m_ssaoPassShader->setValue("screenWidth", width);
	m_ssaoPassShader->setValue("screenHeight", height);

	for (unsigned int i = 0; i < 64; ++i)
	{
		m_ssaoPassShader->setValue("ssaoKernel[" + std::to_string(i) + "]", m_ssaoKernel[i]);
	}

	m_ssaoPassShader->setValue("projection", *renderParams.projection);

	m_ssaoPassShader->use();

	{
		// render to quad
		auto& mesh = m_quad.getComponent<MeshComponent>();

		DrawQueueRenderParams renderParams2D;
		renderParams2D.mesh = mesh.mesh.get();

		m_2DRenderer->render(renderParams2D);
	}

	m_ssaoFBO.unbind();

	// bind textures
	// Todo solve slots issue
	m_positionTexture->setSlot(0);
	m_positionTexture->bind();
	m_lightPassShader->setValue("gPosition", 0);

	m_normalTexture->setSlot(1);
	m_normalTexture->bind();
	m_lightPassShader->setValue("gNormal", 1);

	m_albedoSpecularTexture->setSlot(2);
	m_albedoSpecularTexture->bind();
	m_lightPassShader->setValue("gAlbedoSpec", 2);

	m_ssaoColorBuffer->setSlot(3);
	m_ssaoColorBuffer->bind();
	m_lightPassShader->setValue("gSSAOColorBuffer", 3);

	m_renderTargetFBO->bind();

	// bind fShader
	m_lightPassShader->use();

	m_lightPassShader->bindUniformBlockToBindPoint("Time", 0);
	m_lightPassShader->bindUniformBlockToBindPoint("Lights", 1);

	{
		// render to quad
		auto& mesh = m_quad.getComponent<MeshComponent>();

		DrawQueueRenderParams renderParams2D;
		renderParams2D.mesh = mesh.mesh.get();

		m_2DRenderer->render(renderParams2D);
	}

	m_renderTargetFBO->unbind();
}

uint32_t DeferredRenderer::getRenderTarget() const
{
	return m_renderTargetFBO->getID();
}

const FrameBufferObject& DeferredRenderer::getGBuffer() const
{
	return m_gBuffer;
}
