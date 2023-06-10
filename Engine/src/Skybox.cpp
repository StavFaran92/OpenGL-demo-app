#include "Skybox.h"

#include <GL/glew.h>

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Box.h"
#include "ModelBuilder.h"
#include "TextureHandler.h"
#include "ObjectHandler.h"
#include "StandardShader.h"
#include "ShapeFactory.h"
#include "SkyboxRenderer.h"

#include "EditorCamera.h"
#include "Resources/Engine/Primitives/cube.h"

std::shared_ptr<Entity> Skybox::CreateSkybox(Scene* scene)
{
    auto shader = Shader::create<StandardShader>("Resources/Engine/Shaders/SkyboxShader.vert", "Resources/Engine/Shaders/SkyboxShader.frag");

    std::vector<std::string> faces
    {
        "Resources/Engine/Textures/Skybox/right.jpg",
        "Resources/Engine/Textures/Skybox/left.jpg",
        "Resources/Engine/Textures/Skybox/top.jpg",
        "Resources/Engine/Textures/Skybox/bottom.jpg",
        "Resources/Engine/Textures/Skybox/front.jpg",
        "Resources/Engine/Textures/Skybox/back.jpg"
    };
    auto textureHandler = Texture::loadCubemapTexture(faces);

    auto entity = ShapeFactory::createBox(scene);

    entity->addComponent<StandardShader>(shader);
    entity->addComponent<TextureHandler>(textureHandler);
    entity->addComponent<SkyboxRenderer>();

    return entity;
}

//void Skybox::draw(IRenderer& renderer, Shader* shader)
//{
//    glDepthMask(GL_FALSE);
//    glDepthFunc(GL_LEQUAL);
//
//    m_shader->use();
//    getTextureHandlers()[0]->bind();
//
//    //auto view = glm::mat4(glm::mat3(dynamic_cast<Renderer*>(renderer.get())->GetCamera()->getView())); // remove translation from the view matrix
//    m_shader->setValue("model", m_transformation->getMatrix());
//
//    renderer.SetDrawType(Renderer::DrawType::Triangles);
//
//    for (auto i = 0; i < m_meshes.size(); i++)
//    {
//        m_meshes[i]->render(*m_shader, renderer);
//    }
//
//    getTextureHandlers()[0]->unbind();
//    m_shader->release();
//
//    glDepthMask(GL_TRUE);
//    glDepthFunc(GL_LESS);
//}
