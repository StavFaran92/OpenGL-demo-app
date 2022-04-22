#include "Skybox.h"

#include <GL/glew.h>

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Box.h"
#include "ModelBuilder.h"
#include "TextureHandler.h"

#include "EditorCamera.h"
#include "Resources/Primitives/cube.h"

Skybox* Skybox::CreateSkybox()
{
    auto shader = new Shader("Resources\\Shaders\\SkyboxShader.vert", "Resources\\Shaders\\SkyboxShader.frag");

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

    auto model = (Skybox*)ModelBuilder::builder<Skybox>()
        .setShader(*shader)
        .getMeshBuilder()
        .addTextureHandler(textureHandler)
        .getModelBuilder()
        .build();

    return model;
}

void Skybox::draw(IRenderer& renderer, Shader* shader)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    Shader* currShader = m_shader.get();

    if (shader)
        currShader = shader;

    //auto view = glm::mat4(glm::mat3(dynamic_cast<Renderer*>(renderer.get())->GetCamera()->getView())); // remove translation from the view matrix
    currShader->SetMat4("model", m_transformation->getTransformation());



    renderer.SetDrawType(Renderer::DrawType::Triangles);

    for (auto i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i]->render(*currShader, renderer);
    }

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
