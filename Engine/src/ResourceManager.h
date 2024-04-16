#pragma once

#include <string>

class ResourceManager
{
public:
	std::string getRootDir() const;
	void setRootDir(const std::string& rootDir);

private:
	std::string m_rootResourceDir;
};