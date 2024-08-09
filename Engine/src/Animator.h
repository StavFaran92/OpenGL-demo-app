#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Component.h"

class Animation;

class EngineAPI Animator : public Component
{
public:
	Animator() = default;
	Animator(std::shared_ptr<Animation> animation);
	void update(float dt);
	void getFinalBoneMatrices(const Mesh* mesh, std::vector<glm::mat4>& outFinalBoneMatrices) const;
	void playAnimation(std::shared_ptr<Animation> animation);

private:

	std::shared_ptr<Animation> m_currentAnimation;
	float m_currentTime = 0.f;
	std::unordered_map<std::string, glm::mat4> m_finalBoneMatrices;
};