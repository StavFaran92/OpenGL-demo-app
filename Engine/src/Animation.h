#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

class Bone;

struct BoneInfo
{
	int id = -1;
	glm::mat4 offset;
};

struct MeshNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<MeshNodeData> children;
};

class Animation
{
public:
	void calculateFinalBoneMatrices(float currentTime, std::vector<glm::mat4>& outFinalBoneMatrices);
	
	float getDuration() const;
	float getTicksPerSecond() const;
private:
	void calculateFinalBoneMatricesHelper(const MeshNodeData& nodeData, glm::mat4 parentTransform, float currentTime, std::vector<glm::mat4>& finalBoneMatrices);
private:
	MeshNodeData m_rootNode;
	std::unordered_map<std::string, std::shared_ptr<Bone>> m_bones;
	std::unordered_map<std::string, BoneInfo> m_bonesInfo;
};