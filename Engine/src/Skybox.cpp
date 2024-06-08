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

Entity Skybox::CreateSkybox(const SkyboxFaces& faces, Scene* scene)
{
    if (!scene)
    {
        scene = Engine::get()->getContext()->getActiveScene().get();
    }

    std::vector<std::string> facesVec{faces.right, faces.left, faces.top, faces.bottom, faces.front, faces.back};

    auto textureHandler = Cubemap::createCubemapFromFile(facesVec);
    
    return createSkyboxHelper(textureHandler, scene);
}

Entity Skybox::CreateSkybox(TexType type, Resource<Texture> texture, Scene* scene)
{
    if (!scene)
    {
        scene = Engine::get()->getContext()->getActiveScene().get();
    }

    if (type == TexType::EQUIRECTANGULAR)
    {
        texture = EquirectangularToCubemapConverter::fromEquirectangularToCubemap(texture);
    }

    return createSkyboxHelper(texture, scene);
}

Entity Skybox::createSkyboxHelper(Resource<Texture> texture, Scene* scene)
{
    // Create irradiance map using created cubemap
    auto irradianceMap = IBL::generateIrradianceMap(texture, scene);

    // Create prefilter env map using created cubemap
    auto prefilterEnvMap = IBL::generatePrefilterEnvMap(texture, scene);

    scene->setIBLData(irradianceMap, prefilterEnvMap);

    auto entity = ShapeFactory::createBox(&scene->getRegistry());

    entity.RemoveComponent<RenderableComponent>();
    entity.addComponent<SkyboxComponent>(texture);

    entity.RemoveComponent<ObjectComponent>();

    return entity;
}
