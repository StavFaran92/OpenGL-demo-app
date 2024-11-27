#pragma once

#include <memory>

#include "Engine.h"
#include "Core.h"

#include <glm/glm.hpp>

class VertexArrayObject;
class VertexBufferObject;
class Shader;

class EngineAPI DebugHelper
{
public:
    // Delete copy constructor and assignment operator to prevent duplication
    DebugHelper(const DebugHelper&) = delete;
    DebugHelper& operator=(const DebugHelper&) = delete;

    // Static method to get the instance
    static DebugHelper& getInstance()
    {
        // Use local static variable for thread-safe lazy initialization
        static DebugHelper instance;
        return instance;
    }

    void drawPoint(const glm::vec3& pos, const glm::vec3& color = {1.f,0.f,0.f}, float size = 10.f);

	void drawLine(const glm::vec3& p1, const glm::vec3& p2);

	void drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

private:
    // Private constructor to prevent direct instantiation
    DebugHelper();

    // Optional: Private destructor
    ~DebugHelper() {  }

    std::shared_ptr<VertexArrayObject> m_pointVAO;
    std::shared_ptr<VertexBufferObject> m_pointVBO;
    std::shared_ptr<Shader> m_shader;
};