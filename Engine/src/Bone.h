#pragma once

#include <vector>
#include <map>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtx/quaternion.hpp>

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;

    bool operator<(const KeyPosition& other) const
    {
        return this->timeStamp < other.timeStamp;
    }
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;

    bool operator<(const KeyPosition& other) const
    {
        return this->timeStamp < other.timeStamp;
    }
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;

    bool operator<(const KeyPosition& other) const
    {
        return this->timeStamp < other.timeStamp;
    }
};

class Bone
{
public:
	void update(float animationTime);
	const glm::mat4& getLocalTransform() const;
	glm::mat4 interpolatePosition(float animationTime) const;
	glm::mat4 interpolateRotation(float animationTime) const;
	glm::mat4 interpolateScale(float animationTime) const;
private:
    float getProgress(float t1, float t2, float v) const;
private:

	glm::mat4 m_localTransform;

	std::vector<KeyPosition> m_positions;
	std::vector<KeyRotation> m_rotations;
	std::vector<KeyScale> m_scalings;
};