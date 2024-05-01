#include "Quad.h"

#include "Mesh.h"
#include "MeshBuilder.h"
#include "VertexLayout.h"

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

Resource<Mesh> Quad::createMesh()
{
	VertexLayout layout;
	layout.numOfVertices = 4;
	layout.attribs.emplace_back(LayoutAttribute::Positions);
	layout.attribs.emplace_back(LayoutAttribute::Normals);
	layout.attribs.emplace_back(LayoutAttribute::Texcoords);

	return MeshBuilder::builder()
		.setRawVertices((float*)vertices, layout)
		.setRawIndices((unsigned int*)indices, sizeof(indices) / sizeof(unsigned int))
		.build();
}

