#pragma once

#include <string>

class ShaderExtender
{
public:
	struct ExtensionParams
	{
		bool enableLight = false;
		bool enableMaterial = false;
	};

	static std::string ExtendFragmentShader(const std::string& code, ExtensionParams eParams);
};