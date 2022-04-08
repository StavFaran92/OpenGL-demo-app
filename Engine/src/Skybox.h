#pragma once

#include "Box.h"
#include "Core.h"

class ModelBuilder;


class EngineAPI Skybox : public Box
{
public:
    static Skybox* CreateSkybox();

    void Draw(IRenderer& renderer, Shader* shader = nullptr) override;
    
private:
    friend class ModelBuilder;
    Skybox() = default;
};

