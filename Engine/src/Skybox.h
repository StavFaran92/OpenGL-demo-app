#pragma once

#include "Box.h"
#include "Core.h"

class ObjectFactory;


class EngineAPI Skybox : public Box
{
public:
    static std::shared_ptr<Skybox> CreateSkybox();

    void draw(IRenderer& renderer, Shader* shader = nullptr) override;
    
private:
    friend class ObjectFactory;
    Skybox() = default;
};

