#pragma once

#include <memory>
#include "Animation.h"

class AnimationLoader
{
public:
	static std::shared_ptr<Animation> loadAnimation(const std::string& path);
};