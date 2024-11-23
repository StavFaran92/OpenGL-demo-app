#include "DeferredRenderer.h"

#include "Resource.h"
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
#include "RenderCommand.h"
#include "Context.h"
#include "Animator.h"
#include "MeshCollection.h"
#include "Graphics.h"
#include "System.h"

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
	m_gBuffer.attachTexture(m_positionTexture.get()->getID(), GL_COLOR_ATTACHMENT0);

	// Generate Texture for Normal data
	m_normalTexture = Texture::createEmptyTexture(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_gBuffer.attachTexture(m_normalTexture.get()->getID(), GL_COLOR_ATTACHMENT1);

	// Generate Texture for Albedo
	m_albedoTexture = Texture::createEmptyTexture(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	m_gBuffer.attachTexture(m_albedoTexture.get()->getID(), GL_COLOR_ATTACHMENT2);

	// Generate Texture for MRA
	m_MRATexture = Texture::createEmptyTexture(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	m_gBuffer.attachTexture(m_MRATexture.get()->getID(), GL_COLOR_ATTACHMENT3);

	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	// Create RBO and attach to FBO
	m_gBuffer.attachRenderBuffer(m_renderBuffer.GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	if (!m_gBuffer.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_gBuffer.unbind();

	m_gBufferShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/PBR_GeomPassShader.glsl");

	m_lightPassShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/PBR_LightPassShader.glsl");

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

	m_ssaoNoiseTexture = Texture::create2DTextureFromBuffer(4, 4,
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
	m_ssaoFBO.attachTexture(m_ssaoColorBuffer.get()->getID(), GL_COLOR_ATTACHMENT0);

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

	m_ssaoPassShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/SSAOPassShader.glsl");

	// Initialize SSAO Blur
	m_ssaoBlurFBO.bind();

	m_ssaoBlurColorBuffer = Texture::createEmptyTexture(width, height, GL_RED, GL_RED, GL_FLOAT);
	m_ssaoBlurFBO.attachTexture(m_ssaoBlurColorBuffer.get()->getID(), GL_COLOR_ATTACHMENT0);

	// Create RBO and attach to FBO
	m_ssaoBlurFBO.attachRenderBuffer(m_ssaoBlurRenderBuffer.GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	if (!m_ssaoBlurFBO.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_ssaoBlurFBO.unbind();

	m_ssaoBlurPassShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/SSAOBlurPassShader.glsl");

	return true;
}

bool DeferredRenderer::init()
{
	setupGBuffer();

	//setupSSAO();

	// Generate screen quad
	m_quad = ScreenQuad::GenerateScreenQuad(&Engine::get()->getContext()->getRegistry());
	m_quad.RemoveComponent<RenderableComponent>();
	m_quad.RemoveComponent<ObjectComponent>();

	return true;
}

void DeferredRenderer::render()
{
	auto graphics = Engine::get()->getSubSystem<Graphics>();

    graphics->shader->setModelMatrix(*graphics->model);
    graphics->shader->setViewMatrix(*graphics->view);
    graphics->shader->setProjectionMatrix(*graphics->projection);
	graphics->shader->bindUniformBlockToBindPoint("Time", 0);
	graphics->shader->bindUniformBlockToBindPoint("Lights", 1);

    if (graphics->material)
    {
        graphics->material->use(*graphics->shader);
    }

	// Draw
	auto instanceBatch = graphics->entity->tryGetComponent<InstanceBatch>();
	if (!instanceBatch)
	{
		graphics->shader->setUniformValue("isGpuInstanced", false);
		RenderCommand::draw(graphics->mesh->getVAO());
	}
	else
	{
		graphics->shader->setUniformValue("isGpuInstanced", true);
		RenderCommand::drawInstanced(graphics->mesh->getVAO(), instanceBatch->getCount());
	}
}

void DeferredRenderer::renderSceneUsingCustomShader()
{
	auto graphics = Engine::get()->getSubSystem<Graphics>();

	m_gBuffer.bind();

	glEnable(GL_DEPTH_TEST);

	
	
	
	// TODO fix

	graphics->entity = graphics->entity;
	graphics->mesh = 0;// graphics->entity->getComponent<MeshComponent>().mesh.get();
	auto tempModel = graphics->entity->getComponent<Transformation>().getWorldTransformation();
	auto& shaderComponent = graphics->entity->getComponent<ShaderComponent>();
	Shader* vertexShader = shaderComponent.m_vertexShader ? shaderComponent.m_vertexShader : m_gBufferShader.get();
	Shader* fragmentShader = shaderComponent.m_fragmentShader ? shaderComponent.m_fragmentShader : m_lightPassShader.get();
	vertexShader->use();
	graphics->shader = vertexShader;
	graphics->model = &tempModel;

	MaterialComponent& mat = graphics->entity->getRoot().getComponent<MaterialComponent>();

	graphics->material = mat.begin()->second.get();

	{
		int currentSlot = 8;
		for (const auto& [texName, texture] : shaderComponent.customTextures)
		{
			texture.get()->setSlot(currentSlot);
			texture.get()->bind();
			vertexShader->setUniformValue(texName, currentSlot);
			currentSlot++;
		}
	}

	// draw model
	render();

	// unbind gBuffer
	m_gBuffer.unbind();


	glDisable(GL_DEPTH_TEST);

	// bind textures
	// Todo solve slots issue
	fragmentShader->setTextureInShader(m_positionTexture, "gPosition", 0);
	fragmentShader->setTextureInShader(m_normalTexture, "gNormal", 1);
	fragmentShader->setTextureInShader(m_albedoTexture, "gAlbedo", 2);
	fragmentShader->setTextureInShader(m_MRATexture, "gMRA", 3);
	fragmentShader->setTextureInShader(graphics->irradianceMap, "gIrradianceMap", 4);
	fragmentShader->setTextureInShader(graphics->prefilterEnvMap, "gPrefilterEnvMap", 5);
	fragmentShader->setTextureInShader(graphics->brdfLUT, "gBRDFIntegrationLUT", 6);
	fragmentShader->setTextureInShader(graphics->shadowMap, "gShadowMap", 7);

	//{
	//	// This needs to be fixed since the texture limit will might eventually reached.
	//	int currentSlot = 8;
	//	for (const auto& [texName, texture] : shaderComponent.customTextures)
	//	{
	//		texture.get()->setSlot(currentSlot);
	//		texture.get()->bind();
	//		fragmentShader->setUniformValue(texName, currentSlot);
	//		currentSlot++;
	//	}
	//}
	

#if 0
	m_ssaoBlurColorBuffer->setSlot(3);
	m_ssaoBlurColorBuffer->bind();
	m_lightPassShader->setValue("gSSAOColorBuffer", 3);
#endif

	m_renderTargetFBO->bind();

	// bind fShader
	fragmentShader->use();

	fragmentShader->bindUniformBlockToBindPoint("Time", 0);
	fragmentShader->bindUniformBlockToBindPoint("Lights", 1);

	fragmentShader->setUniformValue("cameraPos", graphics->cameraPos);
	fragmentShader->setUniformValue("lightSpaceMatrix", graphics->lightSpaceMatrix);

	{
		// render to quad
		auto& mesh = m_quad.getComponent<MeshComponent>().mesh.get()->getPrimaryMesh();
		RenderCommand::draw(mesh->getVAO());
	}

	m_renderTargetFBO->unbind();
}

void DeferredRenderer::renderScene(Scene* scene)
{
	auto graphics = Engine::get()->getSubSystem<Graphics>();

	glEnable(GL_DEPTH_TEST);

	graphics->shader = m_gBufferShader.get();
	graphics->shader->use();

	// Render all objects
	for (auto& entityHandler : *graphics->entityGroup)
	{
		Resource<MeshCollection> meshCollecton = entityHandler.getComponent<MeshComponent>().mesh;

		auto animator = entityHandler.tryGetComponent<Animator>();
		if (!animator || animator->m_currentAnimation.isEmpty())
		{
			graphics->shader->setUniformValue("isAnimated", false);
		}
		else
		{
			std::vector<glm::mat4> finalBoneMatrices;
			animator->getFinalBoneMatrices(meshCollecton.get(), finalBoneMatrices);
			for (int i = 0; i < finalBoneMatrices.size(); ++i)
			{
				graphics->shader->setUniformValue("finalBonesMatrices[" + std::to_string(i) + "]", finalBoneMatrices[i]);
			}

			graphics->shader->setUniformValue("isAnimated", true);
		}

		for (auto mesh : meshCollecton.get()->getMeshes())
		{
			graphics->entity = &entityHandler;
			graphics->mesh = mesh.get();
			auto& transform = entityHandler.getComponent<Transformation>();
			graphics->model = &transform.getWorldTransformation();

			// TODO get this to work
			AABB& aabb = mesh.get()->getAABB();
			aabb.adjustToTransform(transform);

			if (!aabb.isOnFrustum(*graphics->frustum))
			{
				continue;
			}

			auto matIndex = mesh->getMaterialIndex();
			auto& materialComponent = graphics->entity->getComponent<MaterialComponent>();
			graphics->material = materialComponent.at(matIndex).get();
			if (!graphics->material)
			{
				graphics->material = Engine::get()->getDefaultMaterial().get();
			}

			// draw model
			render();
		}
		
	};

	// unbind gBuffer
	m_gBuffer.unbind();


	glDisable(GL_DEPTH_TEST);
	
#if 0
	m_ssaoFBO.bind();

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

	m_ssaoPassShader->setValue("projection", *graphics->projection);

	m_ssaoPassShader->use();

	{
		// render to quad
		auto& mesh = m_quad.getComponent<MeshComponent>();

		DrawQueueRenderParams renderParams2D;
		renderParams2D.mesh = mesh.mesh.get();

		m_2DRenderer->render(renderParams2D);
	}

	m_ssaoFBO.unbind();

	glClear(GL_COLOR_BUFFER_BIT);

	m_ssaoBlurFBO.bind();
	
	m_ssaoColorBuffer->setSlot(0);
	m_ssaoColorBuffer->bind();
	m_ssaoBlurPassShader->setValue("gSSAOColorBuffer", 0);

	m_ssaoBlurPassShader->use();

	{
		// render to quad
		auto& mesh = m_quad.getComponent<MeshComponent>();

		DrawQueueRenderParams renderParams2D;
		renderParams2D.mesh = mesh.mesh.get();

		m_2DRenderer->render(renderParams2D);
	}

	m_ssaoBlurFBO.unbind();
#endif

	// bind textures
	// Todo solve slots issue
	m_lightPassShader->setTextureInShader(m_positionTexture, "gPosition", 0);
	m_lightPassShader->setTextureInShader(m_normalTexture, "gNormal", 1);
	m_lightPassShader->setTextureInShader(m_albedoTexture, "gAlbedo", 2);
	m_lightPassShader->setTextureInShader(m_MRATexture, "gMRA", 3);
	m_lightPassShader->setTextureInShader(graphics->irradianceMap, "gIrradianceMap", 4);
	m_lightPassShader->setTextureInShader(graphics->prefilterEnvMap, "gPrefilterEnvMap", 5);
	m_lightPassShader->setTextureInShader(graphics->brdfLUT, "gBRDFIntegrationLUT", 6);
	m_lightPassShader->setTextureInShader(graphics->shadowMap, "gShadowMap", 7);

#if 0
	m_ssaoBlurColorBuffer->setSlot(3);
	m_ssaoBlurColorBuffer->bind();
	m_lightPassShader->setValue("gSSAOColorBuffer", 3);
#endif

	m_renderTargetFBO->bind();

	// bind fShader
	m_lightPassShader->use();

	m_lightPassShader->bindUniformBlockToBindPoint("Time", 0);
	m_lightPassShader->bindUniformBlockToBindPoint("Lights", 1);

	m_lightPassShader->setUniformValue("cameraPos", graphics->cameraPos);
	m_lightPassShader->setUniformValue("lightSpaceMatrix", graphics->lightSpaceMatrix);

	{
		// render to quad
		auto& mesh = m_quad.getComponent<MeshComponent>().mesh.get()->getPrimaryMesh();
		RenderCommand::draw(mesh->getVAO());
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
