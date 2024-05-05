#include "MeshBuilder.h"

#include "Logger.h"
#include "VertexLayout.h"
#include "Factory.h"

MeshBuilder& MeshBuilder::addPosition(const glm::vec3& position)
{
	m_data.m_positions.push_back(position);

	return *this;
}

MeshBuilder& MeshBuilder::addPositions(const std::vector<glm::vec3>& positions)
{
	m_data.m_positions.insert(m_data.m_positions.end(), positions.begin(), positions.end());

	return *this;
}

MeshBuilder& MeshBuilder::addPositions(const float* positions, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		addPosition({ positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2] });
	}

	return *this;
}

MeshBuilder& MeshBuilder::addNormal(const glm::vec3& normal)
{
	m_data.m_normals.push_back(normal);

	return *this;
}

MeshBuilder& MeshBuilder::addNormals(const std::vector<glm::vec3>& normals)
{
	m_data.m_normals.insert(m_data.m_normals.end(), normals.begin(), normals.end());

	return *this;
}

MeshBuilder& MeshBuilder::addNormals(const float* normals, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		addNormal({ normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2] });
	}

	return *this;
}

MeshBuilder& MeshBuilder::addTexcoord(const glm::vec2& texCoord)
{
	m_data.m_texCoords.push_back(texCoord);

	return *this;
}

MeshBuilder& MeshBuilder::addTexcoords(const std::vector<glm::vec2>& texCoords)
{
	m_data.m_texCoords.insert(m_data.m_texCoords.end(), texCoords.begin(), texCoords.end());

	return *this;
}

MeshBuilder& MeshBuilder::addTexcoords(const float* texCoords, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		addTexcoord({ texCoords[i * 2 + 0], texCoords[i * 2 + 1] });
	}

	return *this;
}

MeshBuilder& MeshBuilder::addColor(const glm::vec3& color)
{
	m_data.m_colors.push_back(color);

	return *this;
}

MeshBuilder& MeshBuilder::addColors(const std::vector<glm::vec3>& colors)
{
	m_data.m_colors.insert(m_data.m_colors.end(), colors.begin(), colors.end());

	return *this;
}

MeshBuilder& MeshBuilder::addColors(const float* colors, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		addColor({ colors[i * 3 + 0], colors[i * 3 + 1], colors[i * 3 + 2] });
	}

	return *this;
}

MeshBuilder& MeshBuilder::addTangent(const glm::vec3& tangent)
{
	m_data.m_tangents.push_back(tangent);

	return *this;
}

MeshBuilder& MeshBuilder::addTangents(const std::vector<glm::vec3>& tangents)
{
	m_data.m_tangents.insert(m_data.m_tangents.end(), tangents.begin(), tangents.end());

	return *this;
}

MeshBuilder& MeshBuilder::addTangents(const float* tangents, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		addTangent({ tangents[i * 3 + 0], tangents[i * 3 + 1], tangents[i * 3 + 2] });
	}

	return *this;
}

MeshBuilder& MeshBuilder::addIndex(unsigned int index)
{
	m_data.m_indices.push_back(index);

	return *this;
}

MeshBuilder& MeshBuilder::addIndices(const std::vector<unsigned int>& indices)
{
	m_data.m_indices.insert(m_data.m_indices.end(), indices.begin(), indices.end());

	return *this;
}

MeshBuilder& MeshBuilder::addIndices(const unsigned int* indices, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		addIndex(indices[i]);
	}

	return *this;
}

//MeshBuilder& MeshBuilder::setPositions(std::vector<glm::vec3>& positions, bool copy)
//{
//	if (copy)
//	{
//		m_positions = std::make_shared<std::vector<glm::vec3>>(positions);
//	}
//	else
//	{
//		m_positions = std::shared_ptr<std::vector<glm::vec3>>(&positions);
//	}
//
//	enableAttribute(LayoutAttribute::Positions);
//	setNumOfVertices(positions.size());
//
//	return *this;
//}
//
//MeshBuilder& MeshBuilder::setPositions(const float* positions, size_t size)
//{
//	if (!positions)
//	{
//		logError("Specified ptr is null.");
//		return *this;
//	}
//
//	if (size == 0)
//	{
//		logError("Size cannot be set to 0.");
//		return *this;
//	}
//
//	m_positions = std::make_shared<std::vector<glm::vec3>>();
//
//	for (int i = 0; i < size; i++)
//	{
//		glm::vec3 pos{ positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2] };
//		m_positions->emplace_back(pos);
//	}
//
//	enableAttribute(LayoutAttribute::Positions);
//	setNumOfVertices(size);
//
//	return *this;
//}

//MeshBuilder& MeshBuilder::setNormals(std::vector<glm::vec3>& normals, bool copy)
//{
//	if (copy)
//	{
//		m_normals = std::make_shared<std::vector<glm::vec3>>(normals);
//	}
//	else
//	{
//		m_normals = std::shared_ptr<std::vector<glm::vec3>>(&normals);
//	}
//
//	enableAttribute(LayoutAttribute::Normals);
//
//	return *this;
//}
//
//MeshBuilder& MeshBuilder::setNormals(const float* normals, size_t size)
//{
//	if (!normals)
//	{
//		logError("Specified ptr is null");
//		return *this;
//	}
//
//	if (size == 0)
//	{
//		logError("Size cannot be set to 0.");
//		return *this;
//	}
//
//	m_normals = std::make_shared<std::vector<glm::vec3>>();
//
//	for (int i = 0; i < size; i++)
//	{
//		glm::vec3 normal{ normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2] };
//		m_normals->emplace_back(normal);
//	}
//
//	enableAttribute(LayoutAttribute::Normals);
//	setNumOfVertices(size);
//
//	return *this;
//}

//MeshBuilder& MeshBuilder::setTexcoords(std::vector<glm::vec2>& texCoords, bool copy)
//{
//	if (copy)
//	{
//		m_texCoords = std::make_shared<std::vector<glm::vec2>>(texCoords);
//	}
//	else
//	{
//		m_texCoords = std::shared_ptr<std::vector<glm::vec2>>(&texCoords);
//	}
//
//	enableAttribute(LayoutAttribute::Texcoords);
//
//	return *this;
//}

//MeshBuilder& MeshBuilder::setTexcoords(const float* texCoords, size_t size)
//{
//	if (!texCoords)
//	{
//		logError("Specified ptr is null");
//		return *this;
//	}
//
//	if (size == 0)
//	{
//		logError("Size cannot be set to 0.");
//		return *this;
//	}
//
//	m_texCoords = std::make_shared<std::vector<glm::vec2>>();
//
//	for (int i = 0; i < size; i++)
//	{
//		glm::vec3 texCoord{ texCoords[i * 3 + 0], texCoords[i * 3 + 1], texCoords[i * 3 + 2] };
//		m_texCoords->emplace_back(texCoord);
//	}
//
//	enableAttribute(LayoutAttribute::Texcoords);
//	setNumOfVertices(size);
//
//	return *this;
//}

//MeshBuilder& MeshBuilder::setColors(std::vector<glm::vec3>& colors, bool copy)
//{
//	if (copy)
//	{
//		m_colors = std::make_shared<std::vector<glm::vec3>>(colors);
//	}
//	else
//	{
//		m_colors = std::shared_ptr<std::vector<glm::vec3>>(&colors);
//	}
//
//	enableAttribute(LayoutAttribute::Colors);
//
//	return *this;
//}
//
//MeshBuilder& MeshBuilder::setColors(const float* colors, size_t size)
//{
//	if (!colors)
//	{
//		logError("Specified ptr is null");
//		return *this;
//	}
//
//	if (size == 0)
//	{
//		logError("Size cannot be set to 0.");
//		return *this;
//	}
//
//	m_colors = std::make_shared<std::vector<glm::vec3>>();
//
//	for (int i = 0; i < size; i++)
//	{
//		glm::vec3 color{ colors[i * 3 + 0], colors[i * 3 + 1], colors[i * 3 + 2] };
//		m_colors->emplace_back(color);
//	}
//
//	enableAttribute(LayoutAttribute::Colors);
//	setNumOfVertices(size); // todo verift instead of write
//
//	return *this;
//}

//MeshBuilder& MeshBuilder::setIndices(std::vector<unsigned int>& indices, bool copy)
//{
//	if (copy)
//	{
//		m_indices = std::make_shared<std::vector<unsigned int>>(indices);
//	}
//	else
//	{
//		m_indices = std::shared_ptr<std::vector<unsigned int>>(&indices);
//	}
//
//	return *this;
//}

//MeshBuilder& MeshBuilder::setTangents(std::vector<glm::vec3>& tangents, bool copy)
//{
//	if (copy)
//	{
//		m_tangents = std::make_shared<std::vector<glm::vec3>>(tangents);
//	}
//	else
//	{
//		m_tangents = std::shared_ptr<std::vector<glm::vec3>>(&tangents);
//	}
//
//	enableAttribute(LayoutAttribute::Tangents);
//
//	return *this;
//}

MeshBuilder& MeshBuilder::merge(const MeshBuilder& other)
{

	addPositions(other.m_data.m_positions);
	addNormals(other.m_data.m_normals);
	addTexcoords(other.m_data.m_texCoords);
	addColors(other.m_data.m_colors);
	addIndices(other.m_data.m_indices);

	m_data.m_numOfVertices += other.m_data.m_numOfVertices;
	m_data.m_layout = other.m_data.m_layout;

	return *this;
}

MeshBuilder& MeshBuilder::addRawVertices(const float* vertices, VertexLayout layout)
{
	if (!vertices)
	{
		logError("Specified ptr is null");
		return *this;
	}

	if (layout.numOfVertices == 0)
	{
		logError("Size cannot be set to 0.");
		return *this;
	}

	// calculate stride
	int stride = 0;
	for (auto entry : layout.attribs)
	{
		stride += getAttributeSize(entry);
	}

	int offset = 0;

	// Parse vertices
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> tangents;

	for (auto entry : layout.attribs)
	{
		// Parse positions
		if (LayoutAttribute::Positions == entry)
		{
			positions.reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 pos;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					pos[j] = vertices[stride * i + j + offset];
				}
				positions.emplace_back(pos);
			}
			addPositions(positions);
		}

		// Parse normals
		else if (LayoutAttribute::Normals == entry)
		{
			normals.reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 normal;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					normal[j] = vertices[stride * i + j + offset];
				}
				normals.emplace_back(normal);
			}
			addNormals(normals);
		}

		// Parse texcoords
		else if (LayoutAttribute::Texcoords == entry)
		{
			texcoords.reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec2 vec;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					vec[j] = vertices[stride * i + j + offset];
				}
				texcoords.emplace_back(vec);
			}
			addTexcoords(texcoords);
		}

		// Parse colors
		else if (LayoutAttribute::Colors == entry)
		{
			colors.reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 color;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					color[j] = vertices[stride * i + j + offset];
				}
				colors.emplace_back(color);
			}
			addColors(colors);
		}

		// Parse Tangents
		else if (LayoutAttribute::Tangents == entry)
		{
			tangents.reserve(layout.numOfVertices * getAttributeSize(entry));
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 tangent;
				for (int j = 0; j < getAttributeSize(entry); j++)
				{
					tangent[j] = vertices[stride * i + j + offset];
				}
				colors.emplace_back(tangent);
			}
			addTangents(tangents);
		}

		offset += getAttributeSize(entry);
	}

	return *this;
}



Resource<Mesh> MeshBuilder::build()
{
	Resource<Mesh> mesh = Factory<Mesh>::create();

	if (m_data.m_positions.size() > 0)
	{
		enableAttribute(LayoutAttribute::Positions);
	}
	if (m_data.m_normals.size() > 0)
	{
		enableAttribute(LayoutAttribute::Normals);
	}
	if (m_data.m_texCoords.size() > 0)
	{
		enableAttribute(LayoutAttribute::Texcoords);
	}
	if (m_data.m_colors.size() > 0)
	{
		enableAttribute(LayoutAttribute::Colors);
	}
	if (m_data.m_tangents.size() > 0)
	{
		enableAttribute(LayoutAttribute::Tangents);
	}

	std::sort(m_data.m_layout.attribs.begin(), m_data.m_layout.attribs.end(),
		[](LayoutAttribute l1, LayoutAttribute l2)
	{
		return getAttributeLocationInShader(l1) < getAttributeLocationInShader(l2);
	});

	mesh.get()->setVertexLayout(m_data.m_layout);

	if (!mesh.get()->build(m_data))
	{
		logError("Mesh Builder failed to build mesh.");

		delete this;

		return nullptr;
	}

	delete this;

	return mesh;
}

MeshBuilder& MeshBuilder::builder()
{
	return *new MeshBuilder();
}

MeshBuilder::MeshBuilder()
{
	
}

void MeshBuilder::enableAttribute(LayoutAttribute attribute)
{
	if (std::find(m_data.m_layout.attribs.begin(), m_data.m_layout.attribs.end(), attribute) == m_data.m_layout.attribs.end())
		m_data.m_layout.attribs.emplace_back(attribute);
}

void MeshBuilder::disableAttribute(LayoutAttribute attribute)
{
	auto iter = std::find(m_data.m_layout.attribs.begin(), m_data.m_layout.attribs.end(), attribute);
	if (iter != m_data.m_layout.attribs.end())
		m_data.m_layout.attribs.erase(iter);
}