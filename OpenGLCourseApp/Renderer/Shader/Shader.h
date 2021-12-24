#pragma once

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Utils/IO/Utils.h"

#include <GL/glew.h>

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(const std::string& vertexfilePath, const std::string& fragmentFilePath);

	void Init(const std::string& vertexFilePath, const std::string& fragmentFilePath);

	void UseShader() const;
	void ClearShader();

	inline GLuint getID() const { return m_id; }

	void SetFloat(const std::string& name, float v);
	void SetFloat(const std::string& name, glm::vec2 v);
	void SetFloat(const std::string& name, glm::vec3 v);
	void SetFloat(const std::string& name, glm::vec4 v);
	void SetInt(const std::string& name, int v);
	void SetMat3(const std::string& name, const glm::mat3& v);
	void SetMat4(const std::string& name, const glm::mat4& v);

	int GetUniformLocation(const std::string& name);


	~Shader();


private:
	void BuildShaders(const std::string& vertexCode, const std::string& fragmentCode);
	GLuint AddShader(const std::string& shaderCode, GLenum shaderType);

	bool ValidateRenderer();
	bool ValidateProgramLink();
	bool validateCompilation(const GLuint& theShader, const GLenum& shaderType);
private:
	GLuint m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;

	
};

