#include "TextureHandler.h"

TextureHandler::TextureHandler(Texture* texture)
{
	m_texture = std::shared_ptr<Texture>(texture);
}

TextureHandler::TextureHandler(std::shared_ptr<Texture> texture)
{
	m_texture = texture;
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
