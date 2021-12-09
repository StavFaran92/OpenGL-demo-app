#pragma once

#include <iostream>
#include <GL/glew.h>


class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);

	void LoadTexture();
	void Bind(unsigned int slot = 0);
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

