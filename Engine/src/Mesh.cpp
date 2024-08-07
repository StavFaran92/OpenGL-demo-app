#include "Mesh.h"

#include "MeshBuilder.h"
#include "Logger.h"

#include <GL\glew.h>

Mesh::Mesh()
{
	logInfo(__FUNCTION__);
}

const std::vector<glm::vec3>& Mesh::getPositions() const
{
	return m_positions;
}

const std::vector<glm::vec3>& Mesh::getNormals() const
{
	return m_normals;
}

size_t Mesh::getNumOfVertices() const
{
	return m_positions.size();
}

bool Mesh::build(MeshData& mData)
{
	// validate mesh data
	if (mData.m_positions.size() == 0)
	{
		logError("Cannot build mesh without position data.");
		return false;
	}

	if (mData.m_layout.attribs.size() == 0)
	{
		logError("Number of attributes cannot be 0.");
		return false;
	}

	// if mesh doesn't have normals calculate them
	if (mData.m_normals.size() == 0)
	{
		calculateNormals();
	}

	m_layout = mData.m_layout;

	// calculate stride
	int stride = 0;
	for (auto entry : m_layout.attribs)
	{
		auto& attribData = getAttributeData(entry);
		stride += attribData.length * attribData.size;
	}

	// Update layout info
	m_layout.numOfVertices = mData.m_positions.size();
	m_layout.stride = stride;

	// Create verticies array
	// array size = size of each attribute * size of elements in attribute * vertices count
	int offset = 0;
	unsigned int bufferSize = stride * m_layout.numOfVertices;
	unsigned char* vertices = new unsigned char[bufferSize];

	for (auto entry : m_layout.attribs)
	{
		// Parse positions
		if (LayoutAttribute::Positions == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto pos = mData.m_positions.at(i);
				auto vOffset = stride * i + offset + sizeof(float) * 0;
				memcpy(vertices + vOffset, &pos.x, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 1, &pos.y, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 2, &pos.z, sizeof(float));
			}
		}

		// Parse normals
		else if (LayoutAttribute::Normals == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto normal = mData.m_normals.at(i);
				memcpy(vertices + stride * i + offset + sizeof(float) * 0, &normal.x, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 1, &normal.y, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 2, &normal.z, sizeof(float));
			}
		}

		// Parse texcoords
		else if (LayoutAttribute::Texcoords == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto texCoord = mData.m_texCoords.at(i);
				memcpy(vertices + stride * i + offset + sizeof(float) * 0, &texCoord.x, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 1, &texCoord.y, sizeof(float));
			}
		}

		// Parse colors
		else if (LayoutAttribute::Colors == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto color = mData.m_colors.at(i);
				memcpy(vertices + stride * i + offset + sizeof(float) * 0, &color.x, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 1, &color.y, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 2, &color.z, sizeof(float));
			}
		}

		// Parse colors
		else if (LayoutAttribute::Tangents == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto tangent = mData.m_tangents.at(i);
				memcpy(vertices + stride * i + offset + sizeof(float) * 0, &tangent.x, sizeof(float));
				memcpy(vertices + stride * i + offset + sizeof(float) * 1, &tangent.y, sizeof(float));
			}
		}

		auto& attribData = getAttributeData(entry);
		offset += attribData.length * attribData.size;
	}

	// Create buffers
	m_vao = std::make_shared<VertexArrayObject>();

	if (mData.m_indices.size() > 0)
	{
		m_ibo = std::make_shared<ElementBufferObject>((unsigned int*)&(mData.m_indices[0]), mData.m_indices.size());
	}

	m_vbo = std::make_shared<VertexBufferObject>(&(vertices[0]), m_layout.numOfVertices, bufferSize);

	delete[] vertices;

	m_vao->AttachBuffer(*m_vbo, m_ibo.get(), m_layout);

	m_positions = mData.m_positions;
	//m_normals = std::move(mData.m_normals);

	return true;
}

// Compute the normals of the mesh
void Mesh::calculateNormals()
{
	//m_normals = std::vector<glm::vec3>(getNbVertices(), Vector3(0, 0, 0));

	//// For each triangular face
	//for (uint i = 0; i < getNbFaces(); i++) {

	//	// Get the three vertices index of the current face
	//	uint v1 = getVertexIndexInFace(i, 0);
	//	uint v2 = getVertexIndexInFace(i, 1);
	//	uint v3 = getVertexIndexInFace(i, 2);

	//	assert(v1 < getNbVertices());
	//	assert(v2 < getNbVertices());
	//	assert(v3 < getNbVertices());

	//	// Compute the normal of the face
	//	Vector3 p = getVertex(v1);
	//	Vector3 q = getVertex(v2);
	//	Vector3 r = getVertex(v3);
	//	Vector3 normal = (q - p).cross(r - p).normalize();

	//	// Add the face surface normal to the sum of normals at
	//	// each vertex of the face
	//	mNormals[v1] += normal;
	//	mNormals[v2] += normal;
	//	mNormals[v3] += normal;
	//}

	//// Normalize the normal at each vertex
	//for (uint i = 0; i < getNbVertices(); i++) {
	//	mNormals[i] = mNormals[i].normalize();
	//}
}

void Mesh::clearMesh()
{
	m_positions.clear();
	m_normals.clear();
	m_vao = nullptr;
	m_ibo = nullptr;
	m_vbo = nullptr;
}

void Mesh::setVertexLayout(VertexLayout layout)
{
	m_layout = layout;
}

VertexLayout Mesh::getVertexLayout()
{
	return m_layout;
}

VertexArrayObject* Mesh::getVAO() const
{
	return m_vao.get();
}

Mesh::~Mesh()
{
	logInfo( __FUNCTION__ );
	clearMesh();
}