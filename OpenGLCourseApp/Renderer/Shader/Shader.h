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

	ShaderProgramSource ParseShader(const std::string& vertexLocation, const std::string& fragmentLocation);

	void UseShader() const;
	void ClearShader();

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3 );
	int GetUniformLocation(const std::string& name);


	~Shader();

private:
	unsigned int mRendererID;
	const std::string mVertexFilePath;
	const std::string mFragmentFilePath;
	std::unordered_map<std::string, int> mUniformLocationCache;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void ValidateRenderer();
	void ValidateProgramLink();
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	
};

