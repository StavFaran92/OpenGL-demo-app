#include "Animator.h"

#include "Animation.h"

Animator::Animator(std::shared_ptr<Animation> animation)
	: m_currentAnimation(animation)
{

}

void Animator::update(float dt)
{
	
	if (m_currentAnimation)
	{
		// Increment Animation time
		m_currentTime += m_currentAnimation->getTicksPerSecond() * dt;
		m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
	}
}

void Animator::getFinalBoneMatrices(const Mesh* mesh, std::vector<glm::mat4>& outFinalBoneMatrices) const
{
	std::unordered_map<std::string, glm::mat4> m_intermediateBoneMatrices;
	m_currentAnimation->calculateFinalBoneMatrices(m_currentTime, m_intermediateBoneMatrices);
	
	outFinalBoneMatrices = mesh->getBoneOffsets();

	for (auto& [boneName, boneTransform] : m_intermediateBoneMatrices)
	{
		// Get static bone offset
		auto boneID = mesh->getBoneID(boneName);
		if (boneID == -1)
		{
			continue;
		}
		outFinalBoneMatrices[boneID] = boneTransform * outFinalBoneMatrices[boneID]; //todo check 
	}
}

void Animator::playAnimation(std::shared_ptr<Animation> animation)
{
	m_currentAnimation = animation;
	m_currentTime = 0.f;
}
