#include "TextureTransformer.h"

#include "Resource.h"
#include "Texture.h"
#include "Shader.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "ShapeFactory.h"
#include "Engine.h"
#include "Context.h"
#include "Entity.h"
#include "Component.h"
#include "RenderCommand.h"
#include <GL/glew.h>
#include "MeshCollection.h"

Resource<Texture> TextureTransformer::flipVertical(Resource<Texture> srcTexture)
{
	auto dstTexture = Texture::createEmptyTexture(srcTexture.get()->getWidth(), srcTexture.get()->getHeight(), GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	flipVertical(srcTexture, dstTexture);
	return dstTexture;
}

void TextureTransformer::flipVertical(Resource<Texture> srcTexture, Resource<Texture>& dstTexture)
{
	auto shader = Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/TextureTransformShader.glsl");

	// Generate FBO 
	FrameBufferObject fbo;

	fbo.bind();

	RenderBufferObject rbo{ srcTexture.get()->getWidth(), srcTexture.get()->getHeight() };
	fbo.attachRenderBuffer(rbo.GetID(), FrameBufferObject::AttachmentType::Depth);

	if (!fbo.isComplete())
	{
		logError("FBO is not complete!");
		return;
	}

	// set viewport
	glViewport(0, 0, srcTexture.get()->getWidth(), srcTexture.get()->getHeight());

	shader->use();
	shader->setUniformValue("source", 0);

	srcTexture.get()->setSlot(0);
	srcTexture.get()->bind();

	fbo.attachTexture(dstTexture.get()->getID());

	auto quad = ShapeFactory::createQuad(&Engine::get()->getContext()->getRegistry());
	quad.RemoveComponent<RenderableComponent>();
	quad.RemoveComponent<ObjectComponent>();
	auto vao = quad.getComponent<MeshComponent>().mesh.get()->getPrimaryMesh()->getVAO();

	RenderCommand::clear();

	// render to quad
	RenderCommand::draw(vao);
}