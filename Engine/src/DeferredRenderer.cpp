#include "DeferredRenderer.h"

#include "FrameBufferObject.h"
#include "TextureHandler.h"
#include "gl/glew.h"
#include "Engine.h"
#include "Window.h"
#include "Logger.h"

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

	if (!m_gBuffer.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_gBuffer.unbind();

    return true;
}

void DeferredRenderer::draw(const VertexArrayObject& vao, Shader& shader) const
{
}

void DeferredRenderer::clear() const
{
}

glm::mat4 DeferredRenderer::getProjection() const
{
    return glm::mat4();
}

void DeferredRenderer::render(const DrawQueueRenderParams& renderParams)
{
}

void DeferredRenderer::renderScene(DrawQueueRenderParams& renderParams)
{

}
