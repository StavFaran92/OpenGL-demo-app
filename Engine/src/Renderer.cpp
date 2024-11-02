#include "Renderer.h"

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "VertexArrayObject.h"
#include "Shader.h"
#include "ICamera.h"
#include "EditorCamera.h"
#include "SkyboxRenderer.h"
#include "Logger.h"
#include "Mesh.h"
#include "Entity.h"
#include "Engine.h"
#include "Context.h"
#include "Scene.h"
#include "TimeManager.h"
#include "Entity.h"
#include "Skybox.h"
#include "Transformation.h"
#include "Material.h"
#include "Shader.h"
#include "Component.h"
#include "Transformation.h"
#include "CommonShaders.h"


Renderer::Renderer(std::shared_ptr<FrameBufferObject> renderTarget, Scene* scene)
    : m_renderTargetFBO(renderTarget), m_scene(scene)
{
}

bool Renderer::init()
{
    return true;
}

void Renderer::render(const DrawQueueRenderParams& renderParams)
{
    // Setup
    renderParams.shader->use();
    setUniforms(renderParams);

    // Draw
    draw(*renderParams.mesh->getVAO());


    // Release
    if (renderParams.material)
    {
        renderParams.material->release();
    }

    renderParams.shader->release();
}

void Renderer::enableWireframeMode(bool enable)
{
    m_wireFrameMode = enable;
}

void Renderer::renderScene(DrawQueueRenderParams& renderParams)
{
    glEnable(GL_DEPTH_TEST);
    SetDrawType(Renderer::DrawType::Triangles);

    m_renderTargetFBO->bind();

    // Render Phase
    for (auto& entityHandler : *renderParams.entityGroup)
	{
        renderParams.entity = &entityHandler;
        renderParams.mesh = entityHandler.getComponent<MeshComponent>().mesh.get();
        auto tempModel = entityHandler.getComponent<Transformation>().getWorldTransformation();
        renderParams.model = &tempModel;
        renderParams.shader = Engine::get()->getCommonShaders()->getShader(CommonShaders::ShaderType::PHONG_SHADER).get();

        // TODO rethink this feature
        Shader* attachedShader = renderParams.entity->tryGetComponentInParent<Shader>();
        if (attachedShader)
        {
            renderParams.shader = attachedShader;
        }

        Material* mat = renderParams.entity->tryGetComponentInParent<Material>();

        if (mat)
        {
            renderParams.material = mat;
        }

        // draw model
        render(renderParams);

        renderParams.entity = nullptr;
        renderParams.mesh = nullptr;
        renderParams.model = nullptr;
    };

    m_renderTargetFBO->unbind();
}

void Renderer::setUniforms(const DrawQueueRenderParams& renderParams)
{

    //auto context = Engine::get()->getContext();
    //if (context->getActiveScene()->getSkybox() && entity->HasComponent<Material>())
    //{
    //    auto mat = entity->getComponent<Material>();
    //    if (mat.isReflective())
    //    {
    //        shaderToUse = context->GetReflectionShader();
    //        shaderToUse->use();
    //        shaderToUse->setValue("skybox", 0);
    //        auto textures = context->getActiveScene()->getSkybox()->getTextureHandlers();
    //        if (textures.size() <= 0)
    //        {
    //            logError("Skybox does not contain cubemap texture.");
    //            return;
    //        }
    //        textures[0]->bind();
    //    }

    //    if (mat.isRefractive())
    //    {
    //        shaderToUse = context->GetRefractiveShader();
    //        shaderToUse->use();
    //        shaderToUse->setValue("skybox", 0);
    //        shaderToUse->setValue("refractiveRatio", 1 / 1.52f);
    //        auto textures = context->getActiveScene()->getSkybox()->getTextureHandlers();
    //        if (textures.size() <= 0)
    //        {
    //            logError("Skybox does not contain cubemap texture.");
    //            return;
    //        }
    //        textures[0]->bind();
    //    }
    //}

    // Model
    if (renderParams.model)
    {
        renderParams.shader->setModelMatrix(*renderParams.model);
    }

    // View
    if (renderParams.view)
    {
        renderParams.shader->setViewMatrix(*renderParams.view);
    }

    // Projection
    if (renderParams.projection)
    {
        renderParams.shader->setProjectionMatrix(*renderParams.projection);
    }

    if (renderParams.material)
    {
        renderParams.material->use(*renderParams.shader);
    }

    renderParams.shader->bindUniformBlockToBindPoint("Time", 0);
    renderParams.shader->bindUniformBlockToBindPoint("Lights", 1);


}

uint32_t Renderer::getRenderTarget() const
{
    return m_renderTargetFBO->getID();
}