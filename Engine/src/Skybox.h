#pragma once

#include "Box.h"
#include "Core.h"

template<typename T> class ObjectHandler;
class ObjectFactory;


class EngineAPI Skybox : public Box
{
public:
    static ObjectHandler<Skybox> CreateSkybox();

    void draw(IRenderer& renderer, Shader* shader = nullptr) override;
    
private:
    friend class ObjectFactory;
    Skybox() = default;
};

