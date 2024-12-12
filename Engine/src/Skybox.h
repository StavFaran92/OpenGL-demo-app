#pragma once

#include "Core.h"
#include "Entity.h"
#include "Resource.h"

class TextureHandler;
class Scene;

class EngineAPI Skybox
{
public:
    enum class TexType
    {
        EQUIRECTANGULAR,
        CUBEMAP
    };
    struct SkyboxFaces
    {
        std::string right;
        std::string left;
        std::string top;
        std::string bottom;
        std::string front;
        std::string back;
    };
public:
    static Entity CreateSkyboxFromEquirectangularMap(const std::string& equirectnagularMap, Scene* scene = nullptr);

    static Entity CreateSkyboxFromCubemap_DEPRECATED(const SkyboxFaces& faces, Scene* scene = nullptr);

    static Entity CreateSkyboxFromEquirectangularMap(Resource<Texture> equirectnagularMap, Scene* scene = nullptr);
    static Entity CreateSkyboxFromEquirectangularMap(Resource<Texture> texture, Entity& entity, Scene* scene = nullptr);

private:
    static Entity createSkyboxHelper(Resource<Texture> cubemap, Resource<Texture> equirectangularMap, Entity& entity, Scene* scene);

    
private:
    Skybox() = default;
};

