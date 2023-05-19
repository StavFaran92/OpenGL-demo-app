#pragma once

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "Core.h"
#include "ShaderBuilder.h"

#include "glm/glm.hpp"

class EngineAPI Shader
{
public:
	void use() const;
	void release() const;
	Shader();

	/** Constructor */
	Shader(const std::string& vertexfilePath, const std::string& fragmentFilePath, const std::string& geometryShader = "");

	/** Copy Constructor */
	Shader(const Shader& other);

	/** Copy Assignemnt operator */
	Shader& operator=(const Shader& other);

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

	void setModelMatrix(glm::mat4 model);
	void setViewMatrix(glm::mat4 view);
	void setProjectionMatrix(glm::mat4 projection);
	void setTime(float time);
	void init();

	static Shader* PhongShader();
	static Shader* SolidColorShader();

	~Shader();

protected:


	void clear();
	virtual void BuildShaders(const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryShader);
	uint32_t AddShader(const std::string& shaderCode, unsigned int shaderType);
	bool ValidateRenderer();
	bool ValidateProgramLink();
	bool validateCompilation(const unsigned int& theShader, const unsigned int& shaderType);
private:
	friend class Context;
	inline void SetID(uint32_t id) { m_id = id; }



	friend class ShaderBuilder;

protected:
	unsigned int m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;
	bool m_enableLight = false;
	bool m_enableMaterial = false;
	bool m_enableTexture = false;
	bool m_enableColors = false;
	std::string m_vertexShaderFilepath;
	std::string m_FragmentShaderFilepath;
	std::string m_geometryShaderFilepath;

	static uint32_t s_activateShader;
};
