#pragma once

#include <iostream>
#include <GL/glew.h>
#include <string>

#include "Utils/Logger/Logger.h"

class Texture
{
public:
	// This should not be used.
	Texture();

	static std::shared_ptr<Texture> CreateEmptyTexture();
	static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& fileLocation, bool isFlipped = false);

	void Bind();

	inline void SetType(const std::string type) { m_type = type; }
	inline const GLuint GetID() const { return m_id; }
	inline const std::string GetType() const { return m_type; }
	inline const std::string GetPath() const { return m_fileLocation; }
	~Texture();
private:
	void ClearTexture();

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot;
	std::string m_type = "";

	std::string m_fileLocation;
};
