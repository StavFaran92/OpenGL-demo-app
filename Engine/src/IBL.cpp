#include "IBL.h"

#include "Texture.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"
#include "Shader.h"
#include "ShapeFactory.h"

#include "GL/glew.h"
#include "glm/ext.hpp"
#include "Logger.h"

#include "RenderCommand.h"
#include "Entity.h"
#include "Component.h"
#include "Mesh.h"

#include "Engine.h"

TextureHandler* IBL::generateIrradianceMap(TextureHandler* environmentMap, Scene* scene)
{
	auto irradianceShader = Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/IrradianceShader.glsl");

	// Generate FBO 
	FrameBufferObject fbo;

	fbo.bind();

	// Generate cubemap
	auto irradianceMap = Texture::createCubemapTexture(32, 32, GL_RGB16F, GL_RGB, GL_FLOAT);

	RenderBufferObject rbo{ 32, 32 };
	fbo.attachRenderBuffer(rbo.GetID(), FrameBufferObject::AttachmentType::Depth);

	if (!fbo.isComplete())
	{
		logError("FBO is not complete!");
		return nullptr;
	}

	// Generate views and projection
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// set viewport
	glViewport(0, 0, 32, 32);

	irradianceShader->use();
	irradianceShader->setProjectionMatrix(captureProjection);
	irradianceShader->setValue("environmentMap", 0);

	environmentMap->setSlot(0);
	environmentMap->bind();


	auto box = ShapeFactory::createBox(scene);
	box.RemoveComponent<RenderableComponent>();
	auto vao = box.getComponent<MeshComponent>().mesh->getVAO();

	// render to cube
	// Attach cube map to frame buffer
	for (int i = 0; i < 6; i++)
	{
		// set view
		irradianceShader->setViewMatrix(captureViews[i]);

		// attach cubemap face to fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap->getID(), 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render cube
		RenderCommand::drawIndexed(vao);
	}

	environmentMap->unbind();
	fbo.unbind();

	return irradianceMap;
}

TextureHandler* IBL::generatePrefilterEnvMap(TextureHandler* environmentMap, Scene* scene)
{
	auto prefilterShader = Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/IBLPrefilterShader.glsl");

	// Generate FBO 
	FrameBufferObject fbo;

	fbo.bind();

	// Generate cubemap
	auto prefilterEnvMap = Texture::createCubemapTexture(128, 128, GL_RGB16F, GL_RGB, GL_FLOAT, {
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR },
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR },
		{ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE },
		{ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE },
		{ GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE }
	}, true);

	RenderBufferObject rbo{ 128, 128 };
	fbo.attachRenderBuffer(rbo.GetID(), FrameBufferObject::AttachmentType::Depth);

	if (!fbo.isComplete())
	{
		logError("FBO is not complete!");
		return nullptr;
	}

	// Generate views and projection
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	prefilterShader->use();
	prefilterShader->setProjectionMatrix(captureProjection);
	prefilterShader->setValue("environmentMap", 0);

	environmentMap->setSlot(0);
	environmentMap->bind();


	auto box = ShapeFactory::createBox(scene);
	box.RemoveComponent<RenderableComponent>();
	auto vao = box.getComponent<MeshComponent>().mesh->getVAO();

	// render to cube
	// Attach cube map to frame buffer
	int maxMipLevel = 5;
	for (int mip = 0; mip < maxMipLevel; mip++)
	{
		int mipWidth = 128 * std::pow(.5, mip);
		int mipHeight = 128 * std::pow(.5, mip);

		//rbo.Bind();
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mipWidth, mipHeight);

		// set viewport
		glViewport(0, 0, mipWidth, mipHeight);
		
		float roughness = (float)mip / (maxMipLevel - 1);
		prefilterShader->setValue("roughness", roughness);

		for (int i = 0; i < 6; i++)
		{
			// set view
			prefilterShader->setViewMatrix(captureViews[i]);

			// attach cubemap face to fbo
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterEnvMap->getID(), mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// render cube
			RenderCommand::drawIndexed(vao);
		}
	}

	environmentMap->unbind();
	fbo.unbind();

	return prefilterEnvMap;
}

TextureHandler* IBL::generateBRDFIntegrationLUT(Scene* scene)
{
	auto BRDFIntegrationShader = Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/BRDFIntegrationShader.glsl");

	// Generate FBO 
	FrameBufferObject fbo;

	fbo.bind();

	// Generate 2D LUT
	auto lut = Texture::createTexture(512, 512, GL_RG16, GL_RG, GL_FLOAT, 
		{	
			{ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE },
			{ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE },
			{ GL_TEXTURE_MIN_FILTER, GL_LINEAR },
			{ GL_TEXTURE_MAG_FILTER, GL_LINEAR } 
		}
	, nullptr);

	RenderBufferObject rbo{ 512, 512 };
	fbo.attachRenderBuffer(rbo.GetID(), FrameBufferObject::AttachmentType::Depth);

	if (!fbo.isComplete())
	{
		logError("FBO is not complete!");
		return nullptr;
	}

	fbo.attachTexture(lut->getID());

	// set viewport
	glViewport(0, 0, 512, 512);

	BRDFIntegrationShader->use();

	auto quad = ShapeFactory::createQuad(scene);
	quad.RemoveComponent<RenderableComponent>();
	auto vao = quad.getComponent<MeshComponent>().mesh->getVAO();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render to quad
	RenderCommand::drawIndexed(vao);

	fbo.unbind();

	return lut;

	
}
