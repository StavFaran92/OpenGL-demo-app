#include "ShaderLoader.h"
#include "Utils.h"

ShaderLoader::ShaderLoader(std::shared_ptr<IShaderParser> shaderParser, LoadParams eParams)
    : m_shaderParser(shaderParser), m_params(eParams)
{
}

std::string ShaderLoader::readShader(const std::string& path)
{
    // If extender is not active, simply read the file content and return it as is.
    if (!m_params.extendShader)
    {
        return Utils::ReadFile(path);
    }

    return m_shaderParser->parse(path);
}
