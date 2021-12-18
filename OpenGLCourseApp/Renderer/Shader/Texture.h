#pragma once

#include <iostream>
#include <GL/glew.h>


class Texture
{
public:
	Texture(const std::string& fileLocation, int slot = 0);

	void LoadTexture();
	void Bind();
	~Texture();

private:
	void ClearTexture();


private:
	GLuint m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot;

	std::string m_fileLocation;
};

