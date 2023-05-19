#include "Shader.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "Utils.h"
#include "Logger.h"
#include "ShaderExtender.h"

Shader PhongShader;
Shader SolidColorShader;

uint32_t Shader::s_activateShader = 0;

Shader::Shader()
{
}

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath, const std::string& geometryShader /*= ""*/) :
	m_id(0),
	m_vertexShaderFilepath(vertexFilePath), 
	m_FragmentShaderFilepath(fragmentFilePath),
	m_geometryShaderFilepath(geometryShader)
{}

Shader::Shader(const Shader& other) :
	m_id(0),
	m_vertexShaderFilepath(other.m_vertexShaderFilepath),
	m_FragmentShaderFilepath(other.m_FragmentShaderFilepath),
	m_geometryShaderFilepath(other.m_geometryShaderFilepath)
{
	m_enableLight = other.m_enableLight;
	m_enableMaterial = other.m_enableMaterial;
	m_enableTexture = other.m_enableTexture;
	m_enableColors = other.m_enableColors;
}

Shader& Shader::operator=(const Shader& other)
{
	m_vertexShaderFilepath = other.m_vertexShaderFilepath;
	m_FragmentShaderFilepath = other.m_FragmentShaderFilepath;
	m_geometryShaderFilepath = other.m_geometryShaderFilepath;

	m_enableLight = other.m_enableLight;
	m_enableMaterial = other.m_enableMaterial;
	m_enableTexture = other.m_enableTexture;
	m_enableColors = other.m_enableColors;

	init();

	return *this;
}


void Shader::init()
{
	// Parse shaders
	std::string vertexCode = Utils::ReadFile(m_vertexShaderFilepath);
	std::string fragmentCode = Utils::ReadFile(m_FragmentShaderFilepath);

	std::string geometryCode;

	if (!m_geometryShaderFilepath.empty())
	{
		geometryCode = Utils::ReadFile(m_geometryShaderFilepath);
	}

	// Build shaders
	BuildShaders(vertexCode, fragmentCode, geometryCode);
}

void Shader::BuildShaders(const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryCode) 
{
	//Create a new shader program
	m_id = glCreateProgram();

	// Validate shader program creation
	if (!ValidateRenderer())
		return;

	// Create and attach vertex shader to program
	GLuint vertexShader = AddShader(vertexCode, GL_VERTEX_SHADER);

	//ShaderExtender::ExtensionParams eParams{};
	//std::string extendedVertexCode = ShaderExtender::ExtendVertexShader(vertexCode, eParams);
	//GLuint extendedVertexShader = AddShader(vertexCode, GL_VERTEX_SHADER);
	glAttachShader(m_id, vertexShader);

	GLuint geometryShader = 0;
	if (!geometryCode.empty())
	{
		// Create and attach geometry shader to program
		geometryShader = AddShader(geometryCode, GL_GEOMETRY_SHADER);
		glAttachShader(m_id, geometryShader);
	}

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

	if (geometryShader)
		glDeleteShader(geometryShader);
}

bool Shader::ValidateRenderer()
{
	if (!m_id)
	{
		logError("Error creating program");
		return false;
	}
	return true;
}

bool Shader::ValidateProgramLink()
{
	GLint result = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	if (!result)
	{
		GLchar eLog[1024] = { 0 };
		glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
		logError("Error linking program: {}", eLog);
		return false;
	}

	glValidateProgram(m_id);

	glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		GLchar eLog[1024] = { 0 };
		glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
		logError("Error validating program: {}", eLog);
		return false;
	}
	return true;
}

void Shader::use() const
{
	s_activateShader = m_id;

	glUseProgram(m_id);
}

void Shader::release() const
{
	s_activateShader = 0;

	glUseProgram(0);
}

void Shader::clear()
{
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
}

uint32_t Shader::AddShader(const std::string& shaderCode, unsigned int shaderType)
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

bool Shader::validateCompilation(const unsigned int& shader, const unsigned int& shaderType)
{
	GLint result = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar eLog[1024] = { 0 };
		glGetShaderInfoLog(shader, 1024, NULL, eLog);
		logError("Error compiling the {} shader: {}", shaderType, eLog);
		return false;
	}
	return true;
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	int location = glGetUniformLocation(m_id, name.c_str());

	if (location == -1) {
		logWarning("Uniform {} doesn't exists!", name.c_str());
		return -1;
	}

	m_uniformLocationCache[name] = location;

	return location;
}

inline unsigned int Shader::getID() const
{
	return m_id;
}

void Shader::setFloat(const std::string& name, float v)
{
	if(s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniform1f(getUniformLocation(name), v);
}

void Shader::setFloat(const std::string& name, glm::vec2 v)
{
	if (s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniform2f(getUniformLocation(name), v.x, v.y);
}

void Shader::setFloat(const std::string& name, glm::vec3 v)
{
	if (s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
}

void Shader::setFloat(const std::string& name, glm::vec4 v)
{
	if (s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
}

void Shader::setInt(const std::string& name, int v)
{
	if (s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniform1i(getUniformLocation(name), v);
}

void Shader::setUInt(const std::string& name, unsigned int v)
{
	if (s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniform1ui(getUniformLocation(name), v);
}

void Shader::setMat3(const std::string& name, const glm::mat3& v)
{
	if (s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::setMat4(const std::string& name, const glm::mat4& v)
{
	if (s_activateShader != m_id)
	{
		logWarning("Shader: {} is not currently bound", m_id);
		return;
	}

	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v));
}

Shader* Shader::PhongShader()
{
	Shader* shader = new Shader("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag");
	shader->SetEnableLights(true);
	shader->SetEnableMaterials(true);
	shader->SetEnableTextures(true);
	shader->setEnableColors(true);

	return shader;
}

Shader* Shader::SolidColorShader()
{
	Shader* shader = new Shader("Resources\\Shaders\\LightShader.vert", "Resources\\Shaders\\LightShader.frag");
	shader->SetEnableLights(true);
	shader->SetEnableMaterials(true);
	shader->SetEnableTextures(true);

	return shader;
}

void Shader::setModelMatrix(glm::mat4 model)
{
	setMat4("model", model);
}

void Shader::setViewMatrix(glm::mat4 view)
{
	setMat4("view", view);
}

void Shader::setProjectionMatrix(glm::mat4 projection)
{
	setMat4("projection", projection);
}

void Shader::setTime(float time)
{
	setFloat("time", time);
}

Shader::~Shader() {
	clear();
}