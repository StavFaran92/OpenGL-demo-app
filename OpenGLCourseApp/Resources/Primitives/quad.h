#pragma once

namespace Primtives
{
	namespace Quad {
		//static const float vertices[] = {
		//	 1.0f,  1.0f, 0.0f,  // top right
		//	 1.0f, -1.0f, 0.0f,  // bottom right
		//	-1.0f, -1.0f, 0.0f,  // bottom left
		//	-1.0f,  1.0f, 0.0f   // top left 
		//};

		//static const unsigned int indices[] = {
		//	0, 1, 3,   // first triangle
		//	1, 2, 3    // second triangle
		//};
		 		static const float vertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			-1.0f, -1.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, -1.0f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			-1.0f, 1.0f,0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, -1.0f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f,0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		static const unsigned int indices[] = {
			0, 1, 2,   // first triangle
			3,4,5    // second triangle
		};
	}
}