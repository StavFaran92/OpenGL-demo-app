#include "ShaderExtender.h"

std::string newMain = "out vec4 colour; void main() {vec4 result = __main(); colour = result;}";

std::string ShaderExtender::ExtendFragmentShader(const std::string& code, ExtensionParams eParams)
{
    std::string newCode = code;
    std::string searchString = "main()";
    std::string replaceString = "__main()";

    size_t pos = 0;
    while ((pos = newCode.find(searchString, pos)) != std::string::npos)
    {
        newCode.replace(pos, searchString.length(), replaceString);
        pos += replaceString.length();
    }

    // Append new main that calls __main
    newCode += newMain;

    return newCode;
}
