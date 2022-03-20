#pragma once

#include <iostream>
#include <string>

#include "Logger.h"

class Texture
{
public:
	// This should not be used.
	Texture();

	static std::shared_ptr<Texture> CreateEmptyTexture(int width, int height);
	static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& fileLocation, bool isFlipped = false);
	static std::shared_ptr<Texture> LoadCubemap(std::vector<std::string> faces);

	void Bind();

	inline void SetType(const std::string type) { m_type = type; }
	inline const unsigned int GetID() const { return m_id; }
	inline const std::string GetType() const { return m_type; }
	inline const std::string GetPath() const { return m_fileLocation; }
	inline const uint32_t GetCategory() const { return m_textureCategory; }
	~Texture();
private:
	inline void SetCategory(uint32_t category) { m_textureCategory = category ; }
	void ClearTexture();

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot;
	std::string m_type = "";
	uint32_t m_textureCategory = 0;

	std::string m_fileLocation;
};
