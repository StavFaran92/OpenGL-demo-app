#include "Animator.h"

#include "Animation.h"

void Animator::update(float dt)
{
	
	if (m_currentAnimation)
	{
		// Increment Animation time
		m_currentTime += m_currentAnimation->getTicksPerSecond() * dt;
		m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
		m_currentAnimation->calculateFinalBoneMatrices(m_currentTime, m_finalBoneMatrices);
	}
}

const std::unordered_map<std::string, glm::mat4>& Animator::getFinalBoneMatrices() const
{
	return m_finalBoneMatrices;
}

void Animator::playAnimation(std::shared_ptr<Animation> animation)
{
	m_currentAnimation = animation;
	m_currentTime = 0.f;
}
