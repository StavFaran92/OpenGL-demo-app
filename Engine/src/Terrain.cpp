#include "Terrain.h"
#include "Texture.h"
#include "VertexLayout.h"
#include "Assets.h"

Terrain Terrain::generateTerrain(int rez, float scale, const std::string& heightMapFilepath)
{
	auto heightmap = Engine::get()->getSubSystem<Assets>()->importTexture2D(heightMapFilepath);
	auto height = heightmap.get()->getHeight();
	auto width = heightmap.get()->getWidth();

	// vertex generation
	std::vector<float> vertices;
	int vertexCount = rez * rez * 4;

	vertices.reserve(vertexCount * 5);

	for (unsigned i = 0; i <= rez - 1; i++)
	{
		for (unsigned j = 0; j <= rez - 1; j++)
		{
			vertices.push_back(-width / 2.0f + width * i / (float)rez);			// v.x
			vertices.push_back(0.0f);											// v.y
			vertices.push_back(-height / 2.0f + height * j / (float)rez);		// v.z
			vertices.push_back(i / (float)rez);									// u
			vertices.push_back(j / (float)rez);									// v

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez);	// v.x
			vertices.push_back(0.0f);											// v.y
			vertices.push_back(-height / 2.0f + height * j / (float)rez);		// v.z
			vertices.push_back((i + 1) / (float)rez);							// u
			vertices.push_back(j / (float)rez);									// v

			vertices.push_back(-width / 2.0f + width * i / (float)rez);			// v.x
			vertices.push_back(0.0f);											// v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
			vertices.push_back(i / (float)rez);									// u
			vertices.push_back((j + 1) / (float)rez);							// v

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez);	// v.x
			vertices.push_back(0.0f);											// v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
			vertices.push_back((i + 1) / (float)rez);							// u
			vertices.push_back((j + 1) / (float)rez);							// v
		}
	}

	Terrain terrain;
	terrain.m_heightmap = heightmap;
	terrain.m_scale = scale;
	terrain.m_rez = rez;
	terrain.m_vao = std::make_shared<VertexArrayObject>();
	terrain.m_vbo = std::make_shared<VertexBufferObject>(&(vertices[0]), vertexCount, vertices.size() * sizeof(float));

	VertexLayout layout;
	layout.attribs.push_back(LayoutAttribute::Positions);
	layout.attribs.push_back(LayoutAttribute::Texcoords);

	// calculate stride
	int stride = 0;
	for (auto entry : layout.attribs)
	{
		auto& attribData = getAttributeData(entry);
		stride += attribData.length * attribData.size;
	}

	// Update layout info
	layout.stride = stride;

	terrain.m_vao->AttachBuffer(*terrain.m_vbo, nullptr, layout);

	return terrain; // todo fix
}

VertexArrayObject* Terrain::getVAO() const
{
	return m_vao.get();
}

float Terrain::getScale() const
{
	return m_scale;
}

Resource<Texture> Terrain::getHeightmap() const
{
	return m_heightmap;
}
