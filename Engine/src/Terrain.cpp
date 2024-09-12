#include "Terrain.h"
#include "Texture.h"
#include "VertexLayout.h"
#include "Assets.h"
#include "MeshBuilder.h"
#include "Factory.h"

Terrain Terrain::generateTerrain(int width, int height, float scale, const std::string& heightMapFilepath)
{
	int xRez = 10;
	int yRez = 10;

	// vertex generation
	std::vector<float> vertices;
	int vertexCount = xRez * yRez * 4;

	vertices.reserve(vertexCount * 5);

	for (unsigned i = 0; i < xRez; i++)
	{
		for (unsigned j = 0; j < yRez; j++)
		{
			vertices.push_back((i / (float)xRez) - .5f);				// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back(j / (float)yRez - .5f);					// v.z
			vertices.push_back(i / (float)xRez);						// u
			vertices.push_back(j / (float)yRez);						// v

			vertices.push_back(((i + 1) / (float)xRez) - .5f);			// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back(j / (float)yRez - .5f);					// v.z
			vertices.push_back((i + 1) / (float)xRez);					// u
			vertices.push_back(j / (float)yRez);						// v

			vertices.push_back((i / (float)xRez) - .5f);				// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back((j + 1) / (float)yRez - .5f);			// v.z
			vertices.push_back((i) / (float)xRez);						// u
			vertices.push_back((j + 1) / (float)yRez);					// v

			vertices.push_back(((i + 1) / (float)xRez) - .5f);			// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back((j + 1) / (float)yRez - .5f);			// v.z
			vertices.push_back((i + 1) / (float)xRez);					// u
			vertices.push_back((j + 1) / (float)yRez);					// v
		}
	}

	Resource<Mesh> mesh = Factory<Mesh>::create();

	VertexLayout layout;
	layout.attribs.push_back(LayoutAttribute::Positions);
	layout.attribs.push_back(LayoutAttribute::Texcoords);
	layout.numOfVertices = vertexCount;
	layout.build();

	MeshBuilder::builder()
		.addRawVertices(vertices.data(), layout)
		.build(mesh);

	Terrain terrain;
	terrain.m_heightmap = Engine::get()->getSubSystem<Assets>()->importTexture2D(heightMapFilepath);
	terrain.m_scale = scale;
	terrain.m_width = width;
	terrain.m_height = height;
	terrain.m_mesh = mesh;

	return terrain; // todo fix
}

Resource<Mesh> Terrain::getMesh() const
{
	return m_mesh;
}

float Terrain::getScale() const
{
	return m_scale;
}

Resource<Texture> Terrain::getHeightmap() const
{
	return m_heightmap;
}

int Terrain::getWidth() const
{
	return m_width;
}

int Terrain::getHeight() const
{
	return m_height;
}
