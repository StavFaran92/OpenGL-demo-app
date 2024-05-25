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

    auto textureHandler = Texture::loadCubemapTexture(facesVec);

    auto equirectnagular = EquirectangularToCubemapConverter::fromCubemapToEquirectangular(textureHandler, scene);
    EquirectangularToCubemapConverter::fromEquirectangularToCubemap(equirectnagular, scene);
    
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
        texture = EquirectangularToCubemapConverter::fromEquirectangularToCubemap(texture, scene);
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

    texture.get()->setSlot(0);

    auto entity = ShapeFactory::createBox(scene);

    entity.RemoveComponent<RenderableComponent>();
    auto& mat = entity.getComponent<MaterialComponent>();
    mat.begin()->get()->setTexture(Texture::Type::Diffuse, texture);
    entity.addComponent<SkyboxComponent>();

    entity.RemoveComponent<ObjectComponent>();

    return entity;
}
