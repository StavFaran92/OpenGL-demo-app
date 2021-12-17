#include "Shader.h"
#include <stdexcept>


Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath):
	m_id(0)
{
	Init(vertexFilePath, fragmentFilePath);
}

void Shader::Init(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	// Parse shaders
	std::string vertexCode = Utils::ReadFile(vertexFilePath);
	std::string fragmentCode = Utils::ReadFile(fragmentFilePath);

	// Build shaders
	BuildShaders(vertexCode, fragmentCode);
}

void Shader::BuildShaders(const std::string& vertexCode, const std::string& fragmentCode)
{
	//Create a new shader program
	m_id = glCreateProgram();

	// Validate shader program creation
	if (!ValidateRenderer())
		return;

	// Create and attach vertex shader to program
	GLuint vertexShader = AddShader(vertexCode, GL_VERTEX_SHADER);
	glAttachShader(m_id, vertexShader);

	// Create and attach fragment shader to program
	GLuint fragShader = AddShader(fragmentCode, GL_FRAGMENT_SHADER);
	glAttachShader(m_id, fragShader);

	// Link shader program
	glLinkProgram(m_id);

	// Validate shader program link
	ValidateProgramLink();

	// Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

bool Shader::ValidateRenderer()
{
	if (!m_id)
	{
		fprintf(stderr, "Error creating program\n");
		return false;
	}
}

bool Shader::ValidateProgramLink()
{
	GLint result = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	if (!result)
	{
		GLchar eLog[1024] = { 0 };
		glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
		fprintf(stderr, "Error linking program: '%s'\n", eLog);
		return false;
	}

	glValidateProgram(m_id);

	glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		GLchar eLog[1024] = { 0 };
		glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
		fprintf(stderr, "Error validating program: '%s'\n", eLog);
		return false;
	}
	return true;
}

void Shader::UseShader() const
{
	glUseProgram(m_id);
}

void Shader::ClearShader()
{
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

GLuint Shader::AddShader(const std::string& shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[] = { shaderCode.c_str() };
	const GLint length[] = { shaderCode.size() };

	glShaderSource(shader, 1, code, length);
	glCompileShader(shader);

	if (!validateCompilation(shader, shaderType))
		return -1;

	return shader;
}

bool Shader::validateCompilation(const GLuint& shader, const GLenum& shaderType)
{
	GLint result = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar eLog[1024] = { 0 };
		glGetShaderInfoLog(shader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return false;
	}
	return true;
}

int Shader::GetUniformLocation(const std::string & name)
{
	if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
		return mUniformLocationCache[name];

	int location = glGetUniformLocation(m_id, name.c_str());

	if (location == -1) {
		printf("Warning: uniform %s doesn't exists!", name.c_str());
	}

	mUniformLocationCache[name] = location;

	return location;
}

Shader::~Shader() {
	ClearShader();
}