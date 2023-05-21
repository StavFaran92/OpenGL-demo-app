#pragma once

#include "sge.h"

class GUIHandler : public GuiMenu
{
public:
    GUIHandler(Shader* shader);
    void display() override;


private:
    Shader* m_shader = nullptr;
};



