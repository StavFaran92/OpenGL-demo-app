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


Renderer::Renderer()
{
}

bool Renderer::init()
{
    //m_gBuffer.bind();

    //auto width = Engine::get()->getWindow()->getWidth();
    //auto height = Engine::get()->getWindow()->getHeight();

    //// Generate Texture for Position data
    //m_positionTexture = Texture::createEmptyTexture(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    //m_gBuffer.attachTexture(m_positionTexture->getID(), GL_COLOR_ATTACHMENT0);

    //unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
    //glDrawBuffers(1, attachments);

    //// Create RBO and attach to FBO
    //m_gBuffer.attachRenderBuffer(m_renderBuffer.GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

    //if (!m_gBuffer.isComplete())
    //{
    //    logError("FBO is not complete!");
    //    return false;
    //}

    //m_gBuffer.unbind();

    //// Generate screen quad
    //m_quad = ScreenQuad::GenerateScreenQuad(m_scene);

    //// Generate screen shader
    //m_screenShader = Shader::createShared<Shader>(
    //    "Resources/Engine/Shaders/PostProcess/PostProcessShader_default.vert",
    //    "Resources/Engine/Shaders/PostProcess/PostProcessShader_default.frag");

    //// Generate screen renderer
    //m_2DRenderer = std::make_shared<Renderer2D>();

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

    // Render Phase
    for (auto&& [entity, mesh, transform, renderable] :
        renderParams.registry->view<MeshComponent, Transformation, RenderableComponent>().each())
    {
        Entity entityhandler{ entity, renderParams.scene };
        renderParams.entity = &entityhandler;
        renderParams.mesh = mesh.mesh.get();
        auto tempModel = transform.getWorldTransformation();
        renderParams.model = &tempModel;
        renderParams.shader = Engine::get()->getContext()->getStandardShader();

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