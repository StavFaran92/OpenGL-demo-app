#include "ShaderBuilder.h"

ShaderBuilder& ShaderBuilder::create(const std::string& filePath)
{
	ShaderBuilder* shaderBuilder = new ShaderBuilder(filePath);



	return *shaderBuilder;
}

ShaderBuilder::ShaderBuilder(const std::string& filePath)
{

}

ShaderBuilder& ShaderBuilder::addUniform(const std::string& name, UniformType uType)
{


	return *this;
}

Shader* ShaderBuilder::build()
{
	delete this;

	return false;
}
