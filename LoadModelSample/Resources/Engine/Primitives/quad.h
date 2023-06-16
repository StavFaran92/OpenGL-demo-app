#pragma once

namespace Primtives
{
	namespace Quad {
		static const float vertices[] = {
			// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // top left 
		};

		static const unsigned int indices[] = {
			3, 2, 1,   // first triangle
			1, 0, 3    // second triangle
		};
	}
}