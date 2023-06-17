#pragma once

#include "Core.h"
#include "Entity.h"

class EngineAPI Skybox
{
public:
    static std::shared_ptr<Entity> CreateSkybox(Scene* scene);

    
private:
    Skybox() = default;
};

