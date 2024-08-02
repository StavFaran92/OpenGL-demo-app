#include "Animation.h"

#include "Bone.h"

void Animation::calculateFinalBoneMatricesHelper(const MeshNodeData& nodeData, glm::mat4 parentTransform, float currentTime, std::vector<glm::mat4>& finalBoneMatrices)
{
	std::string nodeName = nodeData.name;
	glm::mat4 nodeTransform = nodeData.transformation;

	auto iter = m_bones.find(nodeName);
	if (iter != m_bones.end())
	{
		auto& bone = (*iter).second;
		if (bone)
		{
			bone->update(currentTime);
			nodeTransform = bone->getLocalTransform(); // todo verify this behaviour
		}
	}

	/* 
	We essentialy wish to go from Bone "Bind" space to Pose space,
	let Bind space be S(x) and final pose space be T(x)
	then to take Bone from one to the other we need S^-1(x)*T(x)
	by multiplying by parentTransform we generate T(x),
	and S^-1(x) is given to us by Assimp,
	in the case it was not provided we could have created it recursivly by inversing all the 
	transformations until the bone is bind space
	more can be explained on the matter in 
	https://www.youtube.com/watch?v=cieheqt7eqc
	at 11:57
	*/

	glm::mat4 globalTransformation = parentTransform * nodeTransform; // transform from local space to model space

	auto boneInfoIter = m_bonesInfo.find(nodeName);
	if (boneInfoIter != m_bonesInfo.end())
	{
		auto id = (*boneInfoIter).second.id;
		auto& offset = (*boneInfoIter).second.offset;
		finalBoneMatrices[id] = globalTransformation * offset; // tranform by inverse offset matrix
	}

	for (int i = 0; i < nodeData.childrenCount; i++)
	{
		calculateFinalBoneMatricesHelper(nodeData.children[i], globalTransformation, currentTime, finalBoneMatrices);
	}


}

void Animation::calculateFinalBoneMatrices(float currentTime, std::vector<glm::mat4>& outFinalBoneMatrices)
{
	return calculateFinalBoneMatricesHelper(m_rootNode, glm::mat4(1.0f), currentTime, outFinalBoneMatrices);
}

float Animation::getDuration() const
{
	return 0.0f;
}

float Animation::getTicksPerSecond() const
{
	return 0.0f;
}
