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
#include "MeshCollection.h"
#include "Graphics.h"


Renderer::Renderer(std::shared_ptr<FrameBufferObject> renderTarget, Scene* scene)
    : m_renderTargetFBO(renderTarget), m_scene(scene)
{
}

bool Renderer::init()
{
    return true;
}

void Renderer::render()
{
    auto graphics = Engine::get()->getSubSystem<Graphics>();

    // Setup
    graphics->shader->use();
    setUniforms();

    // Draw
    draw(*graphics->mesh->getVAO());


    // Release
    if (graphics->material)
    {
        graphics->material->release();
    }

    graphics->shader->release();
}

void Renderer::enableWireframeMode(bool enable)
{
    m_wireFrameMode = enable;
}

void Renderer::renderScene(Scene* scene)
{
    auto graphics = Engine::get()->getSubSystem<Graphics>();

    glEnable(GL_DEPTH_TEST);
    SetDrawType(Renderer::DrawType::Triangles);

    m_renderTargetFBO->bind();

    // Render Phase
    for (auto& entityHandler : *graphics->entityGroup)
	{
        graphics->entity = &entityHandler;
        for (auto& mesh : entityHandler.getComponent<MeshComponent>().mesh.get()->getMeshes())
        {

            auto tempModel = entityHandler.getComponent<Transformation>().getWorldTransformation();
            graphics->model = &tempModel;
            graphics->shader = Engine::get()->getCommonShaders()->getShader(CommonShaders::ShaderType::PHONG_SHADER).get();
            graphics->mesh = mesh.get();

            // TODO rethink this feature
            Shader* attachedShader = graphics->entity->tryGetComponentInParent<Shader>();
            if (attachedShader)
            {
                graphics->shader = attachedShader;
            }

            Material* mat = graphics->entity->tryGetComponentInParent<Material>();

            if (mat)
            {
                graphics->material = mat;
            }

            // draw model
            render();
        }
    };

    m_renderTargetFBO->unbind();
}

void Renderer::setUniforms()
{

    auto graphics = Engine::get()->getSubSystem<Graphics>();

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
    if (graphics->model)
    {
        graphics->shader->setModelMatrix(*graphics->model);
    }

    // View
    if (graphics->view)
    {
        graphics->shader->setViewMatrix(*graphics->view);
    }

    // Projection
    if (graphics->projection)
    {
        graphics->shader->setProjectionMatrix(*graphics->projection);
    }

    if (graphics->material)
    {
        graphics->material->use(*graphics->shader);
    }

    graphics->shader->bindUniformBlockToBindPoint("Time", 0);
    graphics->shader->bindUniformBlockToBindPoint("Lights", 1);


}

uint32_t Renderer::getRenderTarget() const
{
    return m_renderTargetFBO->getID();
}