#pragma once

#include "Model.h"

class Skybox : public Model
{
public:
    static std::shared_ptr<Skybox> CreateSkybox();

    void Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader = nullptr) override;
    
};

