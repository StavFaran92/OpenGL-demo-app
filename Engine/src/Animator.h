#pragma once

#include <memory>
#include <vector>
#include "glm/glm.hpp"

class Animation;

class Animator
{
public:
	void update(float dt);
	const std::vector<glm::mat4>& getFinalBoneMatrices() const;
	void playAnimation(std::shared_ptr<Animation> animation);

private:

	std::shared_ptr<Animation> m_currentAnimation;
	float m_currentTime = 0.f;
	std::vector<glm::mat4> m_finalBoneMatrices;
};