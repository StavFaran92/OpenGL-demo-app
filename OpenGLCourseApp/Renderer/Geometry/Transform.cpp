#include "Transform.h"

void Transform::Update(float deltaTime)
{
    if (m_change)
    {
        m_transformation = glm::mat4(1.0f);
        m_transformation = glm::scale(m_transformation, m_scale);
        m_transformation = glm::rotate(m_transformation, glm::radians(m_rotationAngle), m_rotationAxis);
        m_transformation = glm::translate(m_transformation, m_translation);

        m_change = false;
    }
}
void Transform::SetPosition(glm::vec3 pos)
{
    m_translation = pos;

    m_change = true;
}
void Transform::SetRotation(float angle, glm::vec3 axis)
{
    m_rotationAngle = angle;
    m_rotationAxis = axis;

    m_change = true;
}
void Transform::SetScale(glm::vec3 scale)
{
    m_scale = scale;

    m_change = true;
}