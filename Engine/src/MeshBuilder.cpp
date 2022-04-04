#include "MeshBuilder.h"

#include "Model.h"

MeshBuilder& MeshBuilder::setNumOfVertices(size_t size)
{
	m_numOfVertices = size;

	return *this;
}

MeshBuilder& MeshBuilder::setPositions(std::vector<glm::vec3>& positions, bool copy)
{
	if (copy)
	{
		m_positions = std::make_shared<std::vector<glm::vec3>>(positions);
	}
	else
	{
		m_positions = std::shared_ptr<std::vector<glm::vec3>>(&positions);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setPositions(const float* positions, size_t size)
{
	m_positions = std::make_shared<std::vector<glm::vec3>>();

	for (int i = 0; i < size; i++)
	{
		glm::vec3 pos{ positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2] };
		m_positions->emplace_back(pos);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setNormals(std::vector<glm::vec3>& normals, bool copy)
{
	if (copy)
	{
		m_normals = std::make_shared<std::vector<glm::vec3>>(normals);
	}
	else
	{
		m_normals = std::shared_ptr<std::vector<glm::vec3>>(&normals);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setNormals(const float* normals, size_t size)
{
	m_normals = std::make_shared<std::vector<glm::vec3>>();

	for (int i = 0; i < size; i++)
	{
		glm::vec3 normal{ normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2] };
		m_normals->emplace_back(normal);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setTexcoords(std::vector<glm::vec2>& texCoords, bool copy)
{
	if (copy)
	{
		m_texCoords = std::make_shared<std::vector<glm::vec2>>(texCoords);
	}
	else
	{
		m_texCoords = std::shared_ptr<std::vector<glm::vec2>>(&texCoords);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setTexcoords(const float* texCoords, size_t size)
{
	m_texCoords = std::make_shared<std::vector<glm::vec2>>();

	for (int i = 0; i < size; i++)
	{
		glm::vec3 texCoord{ texCoords[i * 3 + 0], texCoords[i * 3 + 1], texCoords[i * 3 + 2] };
		m_texCoords->emplace_back(texCoord);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setColors(std::vector<glm::vec3>& colors, bool copy)
{
	if (copy)
	{
		m_colors = std::make_shared<std::vector<glm::vec3>>(colors);
	}
	else
	{
		m_colors = std::shared_ptr<std::vector<glm::vec3>>(&colors);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setColors(const float* colors, size_t size)
{
	m_colors = std::make_shared<std::vector<glm::vec3>>();

	for (int i = 0; i < size; i++)
	{
		glm::vec3 color{ colors[i * 3 + 0], colors[i * 3 + 1], colors[i * 3 + 2] };
		m_colors->emplace_back(color);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setIndices(std::vector<unsigned int>& indices, bool copy)
{
	if (copy)
	{
		m_indices = std::make_shared<std::vector<unsigned int>>(indices);
	}
	else
	{
		m_indices = std::shared_ptr<std::vector<unsigned int>>(&indices);
	}

	return *this;
}

MeshBuilder& MeshBuilder::setRawVertices(const float* vertices, Mesh::VerticesLayout layout)
{
	setNumOfVertices(layout.numOfVertices);
	// calculate stride
	int stride = 0;
	for (auto entry : layout.attribs)
	{
		stride += getAttributeSize(entry);
	}

	int offset = 0;

	// Parse vertices
	auto positions = new std::vector<glm::vec3>();
	auto normals = new std::vector<glm::vec3>();
	auto texcoords = new std::vector<glm::vec2>();
	auto colors = new std::vector<glm::vec3>();

	for (auto entry : layout.attribs)
	{
		// Parse positions
		if (LayoutAttributes::Positions == entry)
		{
			positions->reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 pos;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					pos[j] = vertices[stride * i + j + offset];
				}
				positions->emplace_back(pos);
			}
			setPositions(*positions);
		}

		// Parse normals
		else if (LayoutAttributes::Normals == entry)
		{
			normals->reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 normal;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					normal[j] = vertices[stride * i + j + offset];
				}
				normals->emplace_back(normal);
			}
			setNormals(*normals);
		}

		// Parse texcoords
		else if (LayoutAttributes::Texcoords == entry)
		{
			texcoords->reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec2 vec;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					vec[j] = vertices[stride * i + j + offset];
				}
				texcoords->emplace_back(vec);
			}
			setTexcoords(*texcoords);
		}

		// Parse colors
		else if (LayoutAttributes::Colors == entry)
		{
			colors->reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 color;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					color[j] = vertices[stride * i + j + offset];
				}
				colors->emplace_back(color);
			}
			setColors(*colors);
		}

		offset += getAttributeSize(entry);
	}

	return *this;
}


MeshBuilder& MeshBuilder::setRawIndices(const unsigned int* indices, size_t size)
{
	auto vec = new std::vector<unsigned int>();
	vec->reserve(size);
	for (auto i = 0; i < size; i++)
	{
		vec->push_back(indices[i]);
	}
	setIndices(*vec);

	return *this;
}


Mesh* MeshBuilder::build()
{
	auto mesh = new Mesh();

	if(m_positions)
		mesh->setPositions(m_positions);

	if (m_normals)
		mesh->setNormals(m_normals);

	if (m_texCoords)
		mesh->setTexcoords(m_texCoords);

	if (m_colors)
		mesh->setColors(m_colors);

	if (m_indices)
		mesh->setIndices(m_indices);

	mesh->setNumOfVertices(m_numOfVertices);

	mesh->build();

	m_isBuilt = true;

	return mesh;
}

void MeshBuilder::setModelBuilder(ModelBuilder* modelBuilder)
{
	m_modelBuilder = modelBuilder;
}

ModelBuilder& MeshBuilder::getModelBuilder() const
{
	return *m_modelBuilder;
}

bool MeshBuilder::isBuilt() const
{
	return m_isBuilt;
}
