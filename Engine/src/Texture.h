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
		Specular
	};
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
	inline const uint32_t getTarget() const { return m_target; }
	~Texture();
private:
	inline void setTarget(uint32_t target) { m_target = target; }
	void ClearTexture();

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot;
	std::string m_type = "";
	uint32_t m_target = 0;

	std::string m_fileLocation;
};
