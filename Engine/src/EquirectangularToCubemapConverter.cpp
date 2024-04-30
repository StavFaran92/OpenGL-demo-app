#include "EquirectangularToCubemapConverter.h"

#include "Texture.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "Resource.h"
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


Resource<Texture> EquirectangularToCubemapConverter::convert(Resource<Texture> equirectangularTexture, Scene* scene)
{
	auto equirectangularShader = Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/EquirectangularToCubemap.glsl");

	// Generate FBO 
	FrameBufferObject fbo;

	fbo.bind();

	// Generate cubemap
	auto cubemap = Texture::createCubemapTexture(512, 512, GL_RGB16F, GL_RGB, GL_FLOAT);

	RenderBufferObject rbo{ 512, 512 };
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
	glViewport(0, 0, 512, 512);

	equirectangularShader->use();
	equirectangularShader->setProjectionMatrix(captureProjection);
	equirectangularShader->setValue("equirectangularMap", 0);

	equirectangularTexture.get()->setSlot(0);
	equirectangularTexture.get()->bind();
	

	auto box = ShapeFactory::createBox(scene);
	auto vao = box.getComponent<MeshComponent>().mesh.get()->getVAO();

	// render to cube
	// Attach cube map to frame buffer
	for (int i = 0; i < 6; i++)
	{
		// set view
		equirectangularShader->setViewMatrix(captureViews[i]);

		// attach cubemap face to fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap.get()->getID(), 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render cube
		RenderCommand::drawIndexed(vao);
	}

	equirectangularTexture.get()->unbind();
	fbo.unbind();

	return cubemap;
}
