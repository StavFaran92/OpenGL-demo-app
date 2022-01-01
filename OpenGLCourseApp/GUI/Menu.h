#pragma once

#include "imgui.h"

#include "Utils/IO/FileUtil.h"
#include "Utils/Logger/Logger.h"

#include "Core/Application.h"
#include "Core/Context.h"
#include "Renderer/Geometry/Model.h"

void DisplayMenu();
void ShowMenuFile();
void ShowAppMainMenuBar();
void OpenFile();
void SaveFile();