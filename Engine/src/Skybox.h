#pragma once

#include "Core.h"
#include "Entity.h"

class EngineAPI Skybox
{
public:
    static Entity CreateSkybox(Scene* scene = nullptr);

    
private:
    Skybox() = default;
};

