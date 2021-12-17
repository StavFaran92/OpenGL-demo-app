#pragma once

#include <stdio.h>
#include <string>
#include "Utils/IO/Utils.h"
#include <unordered_map>


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

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3 );
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
	std::unordered_map<std::string, int> mUniformLocationCache;

	
};

