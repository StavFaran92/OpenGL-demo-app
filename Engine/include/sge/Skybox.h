#pragma once

#include "Model.h"
#include "Core.h"


class EngineAPI Skybox : public Model
{
public:
    static Skybox* CreateSkybox();

    void Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader = nullptr) override;
    
private:
    Skybox() = default;
};

