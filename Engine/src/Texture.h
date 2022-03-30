#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Core.h"


class EngineAPI Texture
{
public:
	enum class Type
	{
		Diffuse,
		Specular,

		None
	};
	// This should not be used.
	Texture();

	static std::shared_ptr<Texture> createEmptyTexture(int width, int height);
	static std::shared_ptr<Texture> loadTextureFromFile(const std::string& fileLocation, bool isFlipped = false);
	static std::shared_ptr<Texture> loadCubemapTexture(std::vector<std::string> faces);

	static std::string textureTypeToString(Type type);

	void bind();
	void unbind();

	void setType(Type type);
	unsigned int getID() const;
	Type getType() const;
	std::string getFilepath() const;
	uint32_t getTarget() const;
	~Texture();
private:
	inline void setTarget(uint32_t target) { m_target = target; }
	void ClearTexture();

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot;
	Type m_type;
	uint32_t m_target = 0;

	std::string m_fileLocation;
};
