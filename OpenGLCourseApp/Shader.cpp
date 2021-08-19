#include "Shader.h"


Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath):
	mRendererID(0), mVertexFilePath(vertexFilePath), mFragmentFilePath(fragmentFilePath)
{
	ShaderProgramSource sources = ParseShader(mVertexFilePath, mFragmentFilePath);

	const char* vertexCode = sources.vertexSource.c_str();
	const char* fragmentCode = sources.fragmentSource.c_str();

	CompileShader(vertexCode, fragmentCode);
}

ShaderProgramSource Shader::ParseShader(const std::string& vertexLocation, const std::string& fragmentLocation) {
	std::string vertexString = Utils::ReadFile(vertexLocation.c_str());
	std::string fragmentString = Utils::ReadFile(fragmentLocation.c_str());

	return { vertexString, fragmentString };
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	mRendererID = glCreateProgram();

	if (!mRendererID)
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(mRendererID, vertexCode, GL_VERTEX_SHADER);
	AddShader(mRendererID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(mRendererID);
	glGetProgramiv(mRendererID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mRendererID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(mRendererID);
	glGetProgramiv(mRendererID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mRendererID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	//uniformModel = glGetUniformLocation(mRendererID, "model");
	//uniformProjection = glGetUniformLocation(mRendererID, "projection");
	//uniformView = glGetUniformLocation(mRendererID, "view");
	//uniformAmbientColor = glGetUniformLocation(mRendererID, "directionalLight.color");
	//uniformAmbientIntensity = glGetUniformLocation(mRendererID, "directionalLight.ambientIntensity");
}

void Shader::UseShader() const
{
	glUseProgram(mRendererID);
}

void Shader::ClearShader()
{
	if (mRendererID != 0) {
		glDeleteProgram(mRendererID);
		mRendererID = 0;
	}
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

int Shader::GetUniformLocation(const std::string & name)
{
	if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
		return mUniformLocationCache[name];

	int location = glGetUniformLocation(mRendererID, name.c_str());

	if (location == -1) {
		printf("Warning: uniform %s doesn't exists!", name.c_str());
	}

	mUniformLocationCache[name] = location;

	return location;
}

Shader::~Shader() {
	ClearShader();
}