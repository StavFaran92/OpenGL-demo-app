#include "SkyboxRenderer.h"
#include "Shader.h"
#include "ICamera.h"
#include "Entity.h"
#include "Context.h"
#include "StandardShader.h"
#include "Mesh.h"
#include "Transformation.h"
#include "Material.h"
#include <GL/glew.h>

void SkyboxRenderer::SetMVP(Shader& shader) const
{
	shader.setValue("projection", m_projection);
	shader.setValue("view", glm::mat4(glm::mat3(m_camera->getView())));
}

void SkyboxRenderer::render(Entity* entity, Mesh* mesh, Transformation* transform, Material* mat, Shader* shader)
{
	glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    Shader* shaderToUse = nullptr;

    if (shader)
    {
        shaderToUse = shader;
    }
    else if (entity->HasComponent<StandardShader>())
    {
        shaderToUse = &entity->getComponent<StandardShader>();
    }
    else
    {
        shaderToUse = Engine::get()->getContext()->getStandardShader();
    }

    assert(shaderToUse);

    shaderToUse->use();

    mat->getTextureHandlers()[0]->bind();

    //auto view = glm::mat4(glm::mat3(dynamic_cast<Renderer*>(renderer.get())->GetCamera()->getView())); // remove translation from the view matrix
    shaderToUse->setValue("model", transform->getMatrix());

    SetDrawType(Renderer::DrawType::Triangles);

    mesh->render(*shaderToUse, *this);

    mat->getTextureHandlers()[0]->unbind();
    shaderToUse->release();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
