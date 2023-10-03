#include "SkyboxRenderer.h"
#include "Shader.h"
#include "ICamera.h"
#include "Entity.h"
#include "Context.h"
#include "Shader.h"
#include "Mesh.h"
#include "Transformation.h"
#include "Material.h"
#include <GL/glew.h>
#include "Component.h"
#include "Engine.h"

void SkyboxRenderer::render(const DrawQueueRenderParams& renderParams)
{
	glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    Shader* shaderToUse = nullptr;

    if (renderParams.shader)
    {
        shaderToUse = renderParams.shader;
    }
    else if (renderParams.entity->HasComponent<Shader>())
    {
        shaderToUse = &renderParams.entity->getComponent<Shader>();
    }
    else
    {
        shaderToUse = Engine::get()->getContext()->getStandardShader();
    }

    assert(shaderToUse);

    shaderToUse->use();


    auto& mat = renderParams.entity->getComponent<Material>();
    mat.getTexture(Texture::Type::Diffuse)->bind();

    //auto view = glm::mat4(glm::mat3(dynamic_cast<Renderer*>(renderer.get())->GetCamera()->getView())); // remove translation from the view matrix
    // Model
    if (renderParams.model)
    {
        shaderToUse->setModelMatrix(*renderParams.model);
    }

    // View
    if (renderParams.view)
    {
        shaderToUse->setViewMatrix(glm::mat4(glm::mat3(*renderParams.view)));
    }

    // Projection
    if (renderParams.projection)
    {
        shaderToUse->setProjectionMatrix(*renderParams.projection);
    }

    SetDrawType(Renderer::DrawType::Triangles);

    renderParams.mesh->render(*shaderToUse, *this);

    mat.getTexture(Texture::Type::Diffuse)->unbind();
    shaderToUse->release();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
