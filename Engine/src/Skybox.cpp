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
#include "DefaultMaterial.h"
#include "Component.h"
#include "Context.h"

#include "EditorCamera.h"
#include "Resources/Engine/Primitives/cube.h"

std::shared_ptr<Entity> Skybox::CreateSkybox(Scene* scene)
{
    if (!scene)
    {
        scene = Engine::get()->getContext()->getActiveScene().get();
    }
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
    textureHandler->setSlot(0);

    auto entity = ShapeFactory::createBox(scene);

    entity->addComponent<StandardShader>(shader);
    entity->RemoveComponent<RenderableComponent>();
    entity->getComponent<DefaultMaterial>().addTextureHandler(textureHandler);
    entity->addComponent<SkyboxComponent>();

    return entity;
}
