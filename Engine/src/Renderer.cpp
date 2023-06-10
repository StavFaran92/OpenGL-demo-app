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
//#include "Model.h"
#include "Entity.h"
#include "Engine.h"
#include "Context.h"
#include "Scene.h"
#include "TimeManager.h"
#include "Entity.h"
#include "Skybox.h"
#include "Transformation.h"
#include "Material.h"
#include "DefaultMaterial.h"
#include "StandardShader.h"


Renderer::Renderer()
{
	//m_camera = std::make_shared<FlyCamera>(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 1.0f, .5f);
	m_camera = std::make_shared<EditorCamera>(glm::vec3(5.0f, 5.0f, 5.0f), 1.0f, .5f);
	m_projection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);
}

Renderer::Renderer(const Renderer& other)
{
	m_camera = other.m_camera;
	m_projection = other.m_projection;
}

//void Renderer::render(Model* model, Shader* shader /* = nullptr*/)
//{
//	model->draw(*this, shader);
//}

void Renderer::draw(const VertexArrayObject& vao, Shader& shader) const
{
	SetMVP(shader);

	vao.Bind();

	if (vao.GetIndexCount() == 0)
	{
		glDrawArrays(m_drawType, 0, vao.GetVerticesCount());
	}
	else 
	{
		glDrawElements(m_drawType, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}

}

void Renderer::SetMVP(Shader& shader) const
{
	shader.setProjectionMatrix(m_projection);
	shader.setViewMatrix(m_camera->getView());
}

std::shared_ptr<ICamera> Renderer::getCamera() const
{
	return m_camera;
}

void Renderer::setCamera(std::shared_ptr<ICamera> camera)
{
	m_camera = camera;
}

glm::mat4 Renderer::getProjection() const
{
	return m_projection;
}

void Renderer::render(Entity* entity, Mesh* mesh, Transformation* transform, Shader* shader)
{
    //model->draw(*this, shader);

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

    // Set model matrix
    auto transformation = entity->getComponent<Transformation>();
    shaderToUse->setModelMatrix(transformation.getMatrix());

    // Set time elapsed
    auto elapsed = (float)Engine::get()->getTimeManager()->getElapsedTime(TimeManager::Duration::MilliSeconds) / 1000;
    shaderToUse->setTime(elapsed);

    if (shaderToUse->IsMaterialsEnabled() && entity->HasComponent<DefaultMaterial>())
    {
        auto mat = entity->getComponent<DefaultMaterial>();
        mat.UseMaterial(*shaderToUse);
    }

    SetDrawType(Renderer::DrawType::Triangles);

    mesh->render(*shaderToUse, *this);

    shaderToUse->release();
}

void Renderer::clear() const
{
	// Clear window
	//glClearStencil(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}