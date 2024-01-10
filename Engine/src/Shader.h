#pragma once

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <memory>
#include <queue>
#include <variant>

#include "Core.h"

#include "glm/glm.hpp"

using Value = std::variant<float, glm::vec2, glm::vec3, glm::vec4, int, unsigned int, glm::mat3, glm::mat4>;

class EngineAPI Shader : public std::enable_shared_from_this<Shader>
{
public:
	template<typename T, typename... _Types>
	static T* create(_Types&&... _Args)
	{
		static_assert(std::is_base_of<Shader, T>::value, "T must be a type derived from Shader");

		auto shader = new T(std::forward<_Types>(_Args)...);
		shader->init();

		return shader;
	}

	template<typename T, typename... _Types>
	static std::shared_ptr<T> createShared(_Types&&... _Args)
	{
		return std::shared_ptr<T>(create<T>(std::forward<_Types>(_Args)...));
	}

	void use();
	void release() const;

	inline unsigned int getID() const;

	void setValue(const std::string& name, const Value& v);

	int getUniformLocation(const std::string& name);

	void setModelMatrix(glm::mat4 model);
	void setViewMatrix(glm::mat4 view);
	void setProjectionMatrix(glm::mat4 projection);
	void setTime(float time);
	void init();

	void bindUniformBlockToBindPoint(const std::string& uniformBlockName, int bindPointIndex);

	static Shader* PhongShader();
	static Shader* SolidColorShader();

	virtual ~Shader();

	/** Copy Constructor */
	Shader(const Shader& other) = default;

	/** Copy Assignemnt operator */
	Shader& operator=(const Shader & other) = default;

protected:
	Shader();

	/** Constructor */
	Shader(const std::string& vertexfilePath, const std::string& fragmentFilePath, const std::string& geometryShader = "");



	void clear();
	virtual void BuildShaders(const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryShader);
	uint32_t AddShader(const std::string& shaderCode, unsigned int shaderType);
	bool ValidateProgramLink();
	bool validateCompilation(const unsigned int& theShader, const unsigned int& shaderType);
private:
	friend class Context;
	inline void SetID(uint32_t id) { m_id = id; }

	void setFloat(const std::string& name, float v);
	void setFloat(const std::string& name, glm::vec2 v);
	void setFloat(const std::string& name, glm::vec3 v);
	void setFloat(const std::string& name, glm::vec4 v);
	void setInt(const std::string& name, int v);
	void setUInt(const std::string& name, unsigned int v);
	void setMat3(const std::string& name, const glm::mat3& v);
	void setMat4(const std::string& name, const glm::mat4& v);

	friend class ShaderBuilder;

protected:
	unsigned int m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;
	std::string m_vertexShaderFilepath;
	std::string m_FragmentShaderFilepath;
	std::string m_geometryShaderFilepath;

	static uint32_t s_activeShader;

	std::queue<std::pair<std::string, Value>> m_delayedProperties;
};
