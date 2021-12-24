#pragma once

#include <iostream>
#include <GL/glew.h>
#include <string>


class Texture
{
public:
	Texture() = delete;
	Texture(const std::string& fileLocation, std::string typeName = "", int slot = 0);

	void LoadTexture(bool isFlipped = false);
	void Bind();
	inline GLuint GetID() const { return m_id; }
	inline std::string GetType() const { return m_type; }
	inline const std::string GetPath() const { return m_fileLocation; }
	~Texture();
private:
	void ClearTexture();


private:
	GLuint m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot;
	std::string m_type = "";

	std::string m_fileLocation;
};

