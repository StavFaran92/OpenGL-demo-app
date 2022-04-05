#pragma once

#include "Box.h"
#include "Core.h"

class ModelBuilder;


class EngineAPI Skybox : public Box
{
public:
    static Skybox* CreateSkybox();

    void Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader = nullptr) override;
    
private:
    friend class ModelBuilder;
    Skybox() = default;
};

