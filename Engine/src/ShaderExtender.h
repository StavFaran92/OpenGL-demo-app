#pragma once

#include <string>

class ShaderExtender
{
public:
	struct ExtensionParams
	{
		bool enableLight = false;
	};

	static std::string ExtendFragmentShader(const std::string& code, ExtensionParams eParams);
};