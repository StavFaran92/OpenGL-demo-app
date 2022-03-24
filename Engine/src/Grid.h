#pragma once

// Includes
// STL
#include <memory>
#include <vector>

#include "glm/glm.hpp"
#include "Model.h"

//Forward Delclerations

class Grid : public Model
{
public:
	Grid() {
		//m_shader = std::make_shared < Shader >("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag");
	}
	static std::shared_ptr<Grid> GenerateGrid(int slices);
	void Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader = nullptr) override;
};

