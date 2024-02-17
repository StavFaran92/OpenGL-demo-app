#pragma once

#include "Core.h"
#include "Entity.h"

class TextureHandler;

class EngineAPI Skybox
{
public:
    static Entity CreateSkybox(Scene* scene = nullptr);

    static Entity CreateSkybox(TextureHandler* cubemap, Scene* scene = nullptr);

    
private:
    Skybox() = default;
};

