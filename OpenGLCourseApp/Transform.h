#pragma once
#include "glm/glm.hpp"

class Transform
{
public:

private:
    glm::vec3 m_translation;

    glm::vec3 m_rotationAxis;
    int m_rotationAngle;

    glm::vec3 m_scale;

    glm::mat4 m_transform;
};

