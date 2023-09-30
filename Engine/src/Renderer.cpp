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
#include "Shader.h"
#include "Component.h"


Renderer::Renderer()
{
	m_projection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);
}

void Renderer::draw(const VertexArrayObject& vao, Shader& shader) const
{
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//SetMVP(shader);

	vao.Bind();

	if (vao.GetIndexCount() == 0)
	{
		glDrawArrays(m_drawType, 0, vao.GetVerticesCount());
	}
	else 
	{
		glDrawElements(m_drawType, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}


    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

glm::mat4 Renderer::getProjection() const
{
	return m_projection;
}

void Renderer::render(const DrawQueueRenderParams& renderParams)
{
    //model->draw(*this, shader);

    glEnable(GL_DEPTH_TEST);

    Shader* shaderToUse = renderParams.shader;

    if(!shaderToUse)
    {
        shaderToUse = renderParams.entity->tryGetComponentInParent<Shader>();
    }
   
    if (!shaderToUse)
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

    // Model
    if (renderParams.model)
    {
        shaderToUse->setModelMatrix(*renderParams.model);
    }

    // View
    if (renderParams.view)
    {
        shaderToUse->setViewMatrix(*renderParams.view);
    }

    // Projection
    if (renderParams.projection)
    {
        shaderToUse->setProjectionMatrix(*renderParams.projection);
    }

    shaderToUse->bindUniformBlockToBindPoint("Time", 0);
    
    DefaultMaterial* mat = nullptr;
    if (shaderToUse->IsMaterialsEnabled())
    {
        mat = renderParams.entity->tryGetComponentInParent<DefaultMaterial>();
        
        if (mat)
        {
            mat->use(*shaderToUse);
        }
    }

    SetDrawType(Renderer::DrawType::Triangles);

    renderParams.mesh->render(*shaderToUse, *this);

    if (mat)
    {
        mat->release();
    }

    shaderToUse->release();
}

void Renderer::enableWireframeMode(bool enable)
{
    m_wireFrameMode = enable;
}

void Renderer::clear() const
{
	// Clear window
	//glClearStencil(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}