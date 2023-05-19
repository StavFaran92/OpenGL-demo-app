#pragma once

#include <memory>

#include "Shader.h"

class DirectionalLight;
class PointLight;

class EngineAPI StandardShader : public Shader
{
public:
	StandardShader(const std::string& vertexfilePath, const std::string& fragmentFilePath, const std::string& geometryShader = "");

	void setDirLightCount(int count);
	void setPointLightCount(int count);
	void setUseColors(bool enable);
	void setViewPos(glm::vec3 viewPosition);
	void setColorMul(glm::vec4 colorMul);
	void updateDirLights(std::unordered_map<uint32_t, std::shared_ptr<DirectionalLight>>& dirLights);
	void updatePointLights(std::unordered_map<uint32_t, std::shared_ptr<PointLight>>& pointLights);

	void BuildShaders(const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryCode) override;
};