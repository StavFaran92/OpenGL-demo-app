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

#include "EditorCamera.h"
#include "Resources/Primitives/cube.h"

ObjectHandler<Skybox> Skybox::CreateSkybox()
{
    auto shader = new StandardShader("Resources\\Shaders\\SkyboxShader.vert", "Resources\\Shaders\\SkyboxShader.frag");

    std::vector<std::string> faces
    {
        "Resources\\Textures\\Skybox\\right.jpg",
        "Resources\\Textures\\Skybox\\left.jpg",
        "Resources\\Textures\\Skybox\\top.jpg",
        "Resources\\Textures\\Skybox\\bottom.jpg",
        "Resources\\Textures\\Skybox\\front.jpg",
        "Resources\\Textures\\Skybox\\back.jpg"
    };
    auto textureHandler = Texture::loadCubemapTexture(faces);

    return ModelBuilder::builder<Skybox>()
        .setShader(shader)
        .addTextureHandler(textureHandler)
        .getMeshBuilder()
        .getModelBuilder()
        .build();
}

void Skybox::draw(IRenderer& renderer, Shader* shader)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    m_shader->use();
    getTextureHandlers()[0]->bind();

    //auto view = glm::mat4(glm::mat3(dynamic_cast<Renderer*>(renderer.get())->GetCamera()->getView())); // remove translation from the view matrix
    m_shader->setMat4("model", m_transformation->getMatrix());

    renderer.SetDrawType(Renderer::DrawType::Triangles);

    for (auto i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i]->render(*m_shader, renderer);
    }

    getTextureHandlers()[0]->unbind();
    m_shader->release();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
