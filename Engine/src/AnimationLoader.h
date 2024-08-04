#pragma once

#include <memory>
#include "Animation.h"

#include "Core.h"

class EngineAPI AnimationLoader
{
public:
	static std::shared_ptr<Animation> loadAnimation(const std::string& path);
};