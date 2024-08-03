#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

class Animation;

class Animator
{
public:
	void update(float dt);
	const std::unordered_map<std::string, glm::mat4>& getFinalBoneMatrices() const;
	void playAnimation(std::shared_ptr<Animation> animation);

private:

	std::shared_ptr<Animation> m_currentAnimation;
	float m_currentTime = 0.f;
	std::unordered_map<std::string, glm::mat4> m_finalBoneMatrices;
};