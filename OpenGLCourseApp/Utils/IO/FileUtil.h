#pragma once

#include "Core/Application.h"

#include "Window.h"
#include "commdlg.h"

class FileUtil
{
public:
    static std::string OpenFile(const char* filter);
    static std::string SaveFile(const char* filter);
};

