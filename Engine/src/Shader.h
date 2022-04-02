#pragma once

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "Core.h"

#include "glm/glm.hpp"

class EngineAPI Shader
{
public:
	Shader();
	Shader(const std::string& vertexfilePath, const std::string& fragmentFilePath);
	Shader(const Shader& other);

	void use() const;
	void release() const;
	void ClearShader();

	inline unsigned int getID() const { return m_id; }

	void SetFloat(const std::string& name, float v);
	void SetFloat(const std::string& name, glm::vec2 v);
	void SetFloat(const std::string& name, glm::vec3 v);
	void SetFloat(const std::string& name, glm::vec4 v);
	void SetInt(const std::string& name, int v);
	void SetMat3(const std::string& name, const glm::mat3& v);
	void SetMat4(const std::string& name, const glm::mat4& v);

	int GetUniformLocation(const std::string& name);

	inline bool IsLightsEnabled() { return m_enableLight; }
	inline void SetEnableLights(bool value) { m_enableLight = value; }
	inline bool IsTexturesEnabled() { return m_enableTexture; }
	inline void SetEnableTextures(bool value) { m_enableTexture = value; }
	inline bool IsMaterialsEnabled() { return m_enableMaterial; }
	inline void SetEnableMaterials(bool value) { m_enableMaterial = value; }

	static Shader* PhongShader();
	static Shader* SolidColorShader();

	~Shader();

private:
	friend class Context;
	inline void SetID(uint32_t id) { m_id = id; }
	void init(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	void BuildShaders(const std::string& vertexCode, const std::string& fragmentCode);
	uint32_t AddShader(const std::string& shaderCode, unsigned int shaderType);

	bool ValidateRenderer();
	bool ValidateProgramLink();
	bool validateCompilation(const unsigned int& theShader, const unsigned int& shaderType);
private:
	unsigned int m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;
	bool m_enableLight = false;
	bool m_enableMaterial = false;
	bool m_enableTexture = false;

	std::string m_vertexFilePath;
	std::string m_fragmentFilePath;

	static uint32_t s_activateShader;
};
