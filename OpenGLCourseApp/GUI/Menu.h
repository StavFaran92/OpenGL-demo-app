#pragma once

#include "imgui.h"

#include "Utils/IO/FileUtil.h"
#include "Utils/Logger/Logger.h"

#include "Core/Application.h"
#include "Core/Context.h"
#include "Renderer/Geometry/Model.h"

#include "ApplicationConstants.h"

using namespace Constants;

void DisplayMenu();
void ShowMenuFile();
void ShowAppMainMenuBar();
void LoadModel();
void SaveFile();
void LightCreatorWindow();
void ShowModelCreatorWindow();
void ShowModelInspectorWindow();