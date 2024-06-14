#include "Skybox.h"

#include <GL/glew.h>

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Box.h"
#include "Resource.h"
#include "ObjectHandler.h"
#include "Shader.h"
#include "ShapeFactory.h"
#include "Material.h"
#include "Component.h"
#include "Context.h"
#include "Engine.h"
#include "Scene.h"
#include "Cubemap.h"

#include "EditorCamera.h"
#include "cube.h"
#include "EquirectangularToCubemapConverter.h"
#include "IBL.h"

Entity Skybox::CreateSkybox(const std::string& equirectnagularMap, Scene* scene)
{
    auto textureHandler = Cubemap::createCubemapFromEquirectangularFile(equirectnagularMap);

    return CreateSkybox(textureHandler, scene);
}

Entity Skybox::CreateSkybox(const SkyboxFaces& faces, Scene* scene)
{
    std::vector<std::string> facesVec{faces.right, faces.left, faces.top, faces.bottom, faces.front, faces.back};

    auto textureHandler = Cubemap::createCubemapFromCubemapFiles(facesVec);
    
    return CreateSkybox(textureHandler, scene);
}

Entity Skybox::CreateSkybox(Resource<Texture> texture, Scene* scene)
{
    if (!scene)
    {
        scene = Engine::get()->getContext()->getActiveScene().get();
    }

    auto entity = ShapeFactory::createBox(&scene->getRegistry());

    return CreateSkybox(texture, entity, scene);
}

Entity Skybox::CreateSkybox(Resource<Texture> texture, Entity& entity, Scene* scene)
{
    texture = EquirectangularToCubemapConverter::fromEquirectangularToCubemap(texture);

    return createSkyboxHelper(texture, entity, scene);
}

Entity Skybox::createSkyboxHelper(Resource<Texture> texture, Entity& entity, Scene* scene)
{
    if (!scene)
    {
        scene = Engine::get()->getContext()->getActiveScene().get();
    }

    // Create irradiance map using created cubemap
    auto irradianceMap = IBL::generateIrradianceMap(texture, scene);

    // Create prefilter env map using created cubemap
    auto prefilterEnvMap = IBL::generatePrefilterEnvMap(texture, scene);

    scene->setIBLData(irradianceMap, prefilterEnvMap);

    entity.RemoveComponent<RenderableComponent>();
    entity.RemoveComponent<MaterialComponent>();
    entity.addComponent<SkyboxComponent>(texture);

    entity.getComponent<ObjectComponent>().name = "Skybox";

    return entity;
}
