#include "Skybox.h"

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Box.h"

#include "EditorCamera.h"
#include "Resources/Primitives/cube.h"

Skybox* Skybox::CreateSkybox()
{
	auto model = new Skybox();

    auto mesh = Box::generateMesh();

    std::vector<std::string> faces
    {
        "Resources\\Textures\\Skybox\\right.jpg",
            "Resources\\Textures\\Skybox\\left.jpg",
            "Resources\\Textures\\Skybox\\top.jpg",
            "Resources\\Textures\\Skybox\\bottom.jpg",
            "Resources\\Textures\\Skybox\\front.jpg",
            "Resources\\Textures\\Skybox\\back.jpg"
    };
	auto texture = Texture::LoadCubemap(faces);
    mesh->addTexture(texture);

    model->m_meshes.push_back(std::shared_ptr<Mesh>(mesh));

    auto shader = std::make_shared<Shader>("Resources\\Shaders\\SkyboxShader.vert", "Resources\\Shaders\\SkyboxShader.frag");
    model->AttachShader(shader);

    return model;
}

void Skybox::Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    auto currShader = m_shader;

    if (shader)
        currShader = shader;

    //auto view = glm::mat4(glm::mat3(dynamic_cast<Renderer*>(renderer.get())->GetCamera()->getView())); // remove translation from the view matrix
    currShader->SetMat4("model", m_transformation->GetTransformation());



    renderer->SetDrawType(Renderer::DrawType::Triangles);

    for (auto i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i]->renderMesh(currShader, renderer);
    }

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
