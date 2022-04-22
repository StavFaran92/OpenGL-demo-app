#include "TextureHandler.h"

TextureHandler::TextureHandler(Texture* texture)
{
	m_texture = std::shared_ptr<Texture>(texture);
}

TextureHandler::TextureHandler(std::shared_ptr<Texture> texture)
{
	m_texture = texture;
}

TextureHandler::TextureHandler(const TextureHandler& textureHandler)
{
	m_texture = textureHandler.m_texture;
	m_type = textureHandler.m_type;
}

void TextureHandler::bind() const
{
	m_texture->bind();
}

void TextureHandler::unbind() const
{
	m_texture->unbind();
}

void TextureHandler::setType(Texture::Type type)
{
	m_type = type;
}

Texture::Type TextureHandler::getType() const
{
	return m_type;
}

unsigned int TextureHandler::getID() const
{
	return m_texture->getID();
}
