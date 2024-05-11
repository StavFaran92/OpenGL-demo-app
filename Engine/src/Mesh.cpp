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
		stride += getAttributeSize(entry);
	}

	// Update layout info
	m_layout.numOfVertices = mData.m_positions.size();
	m_layout.stride = stride;

	// Create verticies array
	int offset = 0;
	std::vector<float> vertices(stride * mData.m_positions.size(), 0); //TODO optimize

	for (auto entry : m_layout.attribs)
	{
		// Parse positions
		if (LayoutAttribute::Positions == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto pos = mData.m_positions.at(i);
				vertices[stride * i + offset + 0] = pos.x;
				vertices[stride * i + offset + 1] = pos.y;
				vertices[stride * i + offset + 2] = pos.z;
			}
		}

		// Parse normals
		else if (LayoutAttribute::Normals == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto normal = mData.m_normals.at(i);
				vertices[stride * i + offset + 0] = normal.x;
				vertices[stride * i + offset + 1] = normal.y;
				vertices[stride * i + offset + 2] = normal.z;
			}
		}

		// Parse texcoords
		else if (LayoutAttribute::Texcoords == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto texCoord = mData.m_texCoords.at(i);
				vertices[stride * i + offset + 0] = texCoord.x;
				vertices[stride * i + offset + 1] = texCoord.y;
			}
		}

		// Parse colors
		else if (LayoutAttribute::Colors == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto color = mData.m_colors.at(i);
				vertices[stride * i + offset + 0] = color.x;
				vertices[stride * i + offset + 1] = color.y;
				vertices[stride * i + offset + 2] = color.z;
			}
		}

		// Parse colors
		else if (LayoutAttribute::Tangents == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto tangent = mData.m_tangents.at(i);
				vertices[stride * i + offset + 0] = tangent.x;
				vertices[stride * i + offset + 1] = tangent.y;
			}
		}

		offset += getAttributeSize(entry);
	}

	// Create buffers
	m_vao = std::make_shared<VertexArrayObject>();

	if (mData.m_indices.size() > 0)
	{
		m_ibo = std::make_shared<ElementBufferObject>((unsigned int*) & (mData.m_indices[0]), mData.m_indices.size());
	}

	m_vbo = std::make_shared<VertexBufferObject>(&(vertices[0]), (unsigned int)mData.m_positions.size(), sizeof(float) * stride);

	m_vao->AttachBuffer(*m_vbo, m_ibo.get(), m_layout);

	//m_positions = std::move(mData.m_positions);
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