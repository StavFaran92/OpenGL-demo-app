#include "SkyboxRenderer.h"
#include "Shader.h"
#include "ICamera.h"
#include "Entity.h"
#include "Context.h"
#include "StandardShader.h"
#include "Mesh.h"
#include "Transformation.h"
#include "DefaultMaterial.h"
#include <GL/glew.h>
#include "Component.h"

void SkyboxRenderer::render(const DrawQueueRenderParams& renderParams)
{
	glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    Shader* shaderToUse = nullptr;

    if (renderParams.shader)
    {
        shaderToUse = renderParams.shader;
    }
    else if (renderParams.entity->HasComponent<StandardShader>())
    {
        shaderToUse = &renderParams.entity->getComponent<StandardShader>();
    }
    else
    {
        shaderToUse = Engine::get()->getContext()->getStandardShader();
    }

    assert(shaderToUse);

    shaderToUse->use();


    auto& mat = renderParams.entity->getComponent<DefaultMaterial>();
    mat.getTextureHandlers()[0]->bind();

    //auto view = glm::mat4(glm::mat3(dynamic_cast<Renderer*>(renderer.get())->GetCamera()->getView())); // remove translation from the view matrix
    shaderToUse->setValue("model", renderParams.transform->getMatrix());
    shaderToUse->setValue("view", glm::mat4(glm::mat3(renderParams.camera->getView())));
    shaderToUse->setValue("projection", m_projection);

    SetDrawType(Renderer::DrawType::Triangles);

    renderParams.mesh->render(*shaderToUse, *this);

    mat.getTextureHandlers()[0]->unbind();
    shaderToUse->release();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
