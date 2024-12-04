#include "Material.h"

#include "Logger.h"
#include <GL\glew.h>

#include "Engine.h"
#include "Context.h"
#include "CommonTextures.h"

Material::Material()
{
	m_samplers[Texture::Type::Albedo] = std::make_shared<TextureSampler>(3);
	m_samplers[Texture::Type::Normal] = std::make_shared<TextureSampler>(3);
	m_samplers[Texture::Type::Metallic] = std::make_shared<TextureSampler>(1);
	m_samplers[Texture::Type::Roughness] = std::make_shared<TextureSampler>(1);
	m_samplers[Texture::Type::AmbientOcclusion] = std::make_shared<TextureSampler>(1);
}

void Material::use(Shader& shader)
{
	setTexturesInShader(shader);
}

void Material::release()
{
	for (auto i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

std::shared_ptr<TextureSampler> Material::getSampler(Texture::Type textureType) const
{
	return m_samplers.at(textureType);
}

void Material::setSampler(Texture::Type textureType, std::shared_ptr<TextureSampler> sampler)
{
	m_samplers[textureType] = sampler;
}

bool Material::hasTexture(Texture::Type textureType) const
{
	auto iter = m_samplers.find(textureType);
	return iter != m_samplers.end() && iter->second->texture.get();
}

void Material::setTextureInShader(Shader& shader, Texture::Type ttype, int slot)
{
	auto sampler = getSampler(ttype);

	// Activate texture unit i
	glActiveTexture(GL_TEXTURE0 + slot);

	// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
	glBindTexture(GL_TEXTURE_2D, sampler->texture.get()->getID());

	// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".texture", slot);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".xOffset", sampler->xOffset);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".yOffset", sampler->yOffset);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".xScale", sampler->xScale);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".yScale", sampler->yScale);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".channelMaskR", sampler->channelMaskR);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".channelMaskG", sampler->channelCount > 1 ? sampler->channelMaskG : 0);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".channelMaskB", sampler->channelCount > 2 ? sampler->channelMaskB : 0);
	shader.setUniformValue("material." + Texture::textureTypeToString(ttype) + ".channelMaskA", sampler->channelCount > 3 ? sampler->channelMaskA : 0);
}

void Material::setTexturesInShader(Shader& shader)
{
	// It either has diffuse or albedo
	//setTextureInShader(shader, Texture::Type::Diffuse, 0);
	setTextureInShader(shader, Texture::Type::Albedo, 0);
	setTextureInShader(shader, Texture::Type::Normal, 1);
	setTextureInShader(shader, Texture::Type::Metallic, 2);
	setTextureInShader(shader, Texture::Type::Roughness, 3);
	setTextureInShader(shader, Texture::Type::AmbientOcclusion, 4);
}

void Material::setTexture(Texture::Type textureType, Resource<Texture> textureHandler)
{
	auto iter = m_samplers.find(textureType);
	if (iter == m_samplers.end())
	{
		m_samplers[textureType] = std::make_shared<TextureSampler>();
	}
	m_samplers[textureType]->texture = textureHandler;
}

std::vector<Resource<Texture>> Material::getAllTextures() const
{
	auto& res = std::vector<Resource<Texture>>();
	for (auto& [_, sampler] : m_samplers)
	{
		res.push_back(sampler->texture);
	}
	return res;
}
