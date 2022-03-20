#pragma once

#include <memory>

class Shader;

class Material
{
public:
	Material();
	Material(float shine);

	void UseMaterial(std::shared_ptr<Shader> shader);

	~Material();

private:
	float m_specularIntensity;
	float m_shininess;
};
