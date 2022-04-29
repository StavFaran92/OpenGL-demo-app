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

	/** Constructor */
	Shader(const std::string& vertexfilePath, const std::string& fragmentFilePath);

	/** Copy Constructor */
	Shader(const Shader& other);

	/** Copy Assignemnt operator */
	Shader& operator=(const Shader& other);

	void use() const;
	void release() const;
	

	inline unsigned int getID() const;

	void setFloat(const std::string& name, float v);
	void setFloat(const std::string& name, glm::vec2 v);
	void setFloat(const std::string& name, glm::vec3 v);
	void setFloat(const std::string& name, glm::vec4 v);
	void setInt(const std::string& name, int v);
	void setUInt(const std::string& name, unsigned int v);
	void setMat3(const std::string& name, const glm::mat3& v);
	void setMat4(const std::string& name, const glm::mat4& v);

	int getUniformLocation(const std::string& name);

	inline bool IsLightsEnabled() { return m_enableLight; }
	inline void SetEnableLights(bool value) { m_enableLight = value; }
	inline bool IsTexturesEnabled() { return m_enableTexture; }
	inline void SetEnableTextures(bool value) { m_enableTexture = value; }
	inline bool IsMaterialsEnabled() { return m_enableMaterial; }
	inline void SetEnableMaterials(bool value) { m_enableMaterial = value; }
	inline bool isSupportColors() { return m_enableColors; }
	inline void setEnableColors(bool value) { m_enableColors = value; }

	static Shader* PhongShader();
	static Shader* SolidColorShader();

	~Shader();

protected:
	void clear();
private:
	friend class Context;
	inline void SetID(uint32_t id) { m_id = id; }
	void init();
	void BuildShaders(const std::string& vertexCode, const std::string& fragmentCode);
	uint32_t AddShader(const std::string& shaderCode, unsigned int shaderType);

	bool ValidateRenderer();
	bool ValidateProgramLink();
	bool validateCompilation(const unsigned int& theShader, const unsigned int& shaderType);
protected:
	unsigned int m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;
	bool m_enableLight = false;
	bool m_enableMaterial = false;
	bool m_enableTexture = false;
	bool m_enableColors = false;
	std::string m_vertexShaderFilepath;
	std::string m_FragmentShaderFilepath;

	static uint32_t s_activateShader;
};
