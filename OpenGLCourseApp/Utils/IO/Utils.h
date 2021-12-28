#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils/Logger/Logger.h"

class Utils
{
public:
	static std::string ReadFile(const std::string& filePath);
};
