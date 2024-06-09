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
    static Entity CreateSkybox(const std::string& equirectnagularMap, Scene* scene = nullptr);

    static Entity CreateSkybox(const SkyboxFaces& faces, Scene* scene = nullptr);

    static Entity CreateSkybox(TexType type, Resource<Texture> texture, Scene* scene = nullptr);

private:
    static Entity createSkyboxHelper(Resource<Texture> texture, Scene* scene);

    
private:
    Skybox() = default;
};

