#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	//Box boxes[3][3][3];

	Model* test;

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		//for (int i = 0; i < 3; i++)
		//{
		//	for (int j = 0; j < 3; j++)
		//	{
		//		for (int k = 0; k < 3; k++)
		//		{
		//			boxes[i][j][k] = *createBox();
		//			boxes[i][j][k].translate(1.1f*i, 1.1f * j, 1.1f * k);
		//		}
		//	}
		//}
		
		//// Create a default Box ->
		//// Build
		//Box::builder().
		//	build();

		//// Create a default Box -> 
		//// Update it's color vertices -> 
		//// Build
		//float colors[3]{1, 1, 1};
		//Box::builder().
		//	setColors(colors, 3).
		//	build();

		//// Create a default Box -> 
		//// Override it's content entirely using different vertices data and layout -> 
		//// Change it's shader from default ->
		//// Build
		//float vertices[3];
		//Mesh::VerticesLayout layout;
		//layout.numOfVertices = 3;
		//layout.attribs.push_back(LayoutAttributes::Positions);
		//Shader shader("Resources\\Shaders\\LightShader.vert", "Resources\\Shaders\\LightShader.frag");
		//Box::builder().
		//	setRawVertices(vertices, layout).
		//	setShader(shader).
		//	build();

		// Create a default Model ->
		// Set vertices data and layout -> 
		// Build
		float vertices[8]{1,1,1, 1,1,1, 1,1};
		Mesh::VerticesLayout layout;
		layout.numOfVertices = 1;
		layout.attribs.push_back(LayoutAttributes::Positions);
		layout.attribs.push_back(LayoutAttributes::Normals);
		layout.attribs.push_back(LayoutAttributes::Texcoords);

		//auto builder = ModelBuilder<Model>::builder();
		//builder.getMeshBuilder()
		//	.setRawVertices(vertices, layout)
		//	.build();
		//builder.build();

		auto builder = (Box*)ModelBuilder<Box>::builder()
			.getMeshBuilder()
			.setRawVertices(vertices, layout)
			.getModelBuilder()
			.build();




		//Model* test2 = Quad::builder()
		//	.setRawVertices(vertices, layout)
		//	.build();

		//Box* box = Model::builder<Box>()
		//	.build();

		//quad = createQuad();
		//quad->translate(0, 0, -2);
		//quad->rotateZ(90);

		//sphere = Sphere::generateSphere(1, 36, 18);
		//sphere->translate(2, 0, 0);

		postProcess(PostProcess::grayscale());
	}

	void draw() override
	{
		// Push event to quit
		//SDL_Event e;
		//e.type = SDL_QUIT;
		//SDL_PushEvent(&e);
		
		//addEventListener(SDL_EventType::SDL_MOUSEBUTTONUP, []() {std::cout << "print somethin" << std::endl; });
		//addEventListener(SDL_EventType type, []() {std::cout << "print somethin" << std::endl; });
		//
		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_X))
		//{
		//	boxes[0][0][0].rotateX(1);
		//	boxes[0][0][1].rotateX(1);
		//	boxes[0][0][2].rotateX(1);

		//	boxes[0][1][0].rotateX(1);
		//	boxes[0][1][1].rotateX(1);
		//	boxes[0][1][2].rotateX(1);

		//	boxes[0][2][0].rotateX(1);
		//	boxes[0][2][1].rotateX(1);
		//	boxes[0][2][2].rotateX(1);
		//}

		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Y))
		//{
		//	box->rotateY(1);
		//}

		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Z))
		//{
		//	box->rotateZ(1);
		//}

		draw(test);
		
		//box->rotateY(1);
		//for (int i = 0; i < 3; i++)
		//{
		//	for (int j = 0; j < 3; j++)
		//	{
		//		for (int k = 0; k < 3; k++)
		//		{
		//			draw(&boxes[i][j][k]);
		//		}
		//	}
		//}
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}