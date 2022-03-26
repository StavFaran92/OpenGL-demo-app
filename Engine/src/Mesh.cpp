#include "Mesh.h"

//Mesh::Mesh(std::shared_ptr<std::vector<Vertex>> vertices, std::shared_ptr<std::vector<unsigned int>> indices) :
//	m_vertices(vertices), m_indices(indices)
//{
//	logTrace( __FUNCTION__ );
//
//	m_vao = std::make_shared<VertexArrayObject>();
//	m_ibo = std::make_shared<ElementBufferObject>(&(m_indices->at(0)), indices->size());
//	m_vbo = std::make_shared<VertexBufferObject>(&(vertices->at(0)), vertices->size(), sizeof(Vertex));
//
//	m_vao->AttachBuffer(*m_vbo, *m_ibo);
//}
//
//Mesh::Mesh(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize)
//{
//	logTrace(__FUNCTION__);
//
//	m_vao = std::make_shared<VertexArrayObject>();
//	m_ibo = std::make_shared<ElementBufferObject>(indices, indicesSize);
//	m_vbo = std::make_shared<VertexBufferObject>(vertices, verticesSize / sizeof(Vertex), sizeof(Vertex));
//
//	m_vao->AttachBuffer(*m_vbo, *m_ibo);
//}
//
//Mesh::Mesh(std::shared_ptr<std::vector<Vertex>> vertices) :
//	m_vertices(vertices), m_indices(nullptr)
//{
//	logTrace(__FUNCTION__);
//
//	m_vao = std::make_shared<VertexArrayObject>();
//	m_vbo = std::make_shared<VertexBufferObject>(&(vertices->at(0)), vertices->size(), sizeof(Vertex));
//
//	m_vao->AttachBuffer(*m_vbo);
//}
//
//Mesh::Mesh(float* vertices, size_t verticesSize)
//{
//	logTrace(__FUNCTION__);
//
//	m_vao = std::make_shared<VertexArrayObject>();
//	m_vbo = std::make_shared<VertexBufferObject>(vertices, verticesSize / sizeof(Vertex), sizeof(Vertex));
//
//	m_vao->AttachBuffer(*m_vbo);
//}

void Mesh::renderMesh(std::shared_ptr<Shader> shader, std::shared_ptr < IRenderer >renderer)
{
	if (shader->IsTexturesEnabled())
	{
		SetTexturesInShader(shader);
	}

	renderer->Draw(*m_vao, shader);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::addTexture(std::shared_ptr<Texture> texture)
{
	m_textures.push_back(texture);
}

void Mesh::addTextures(std::vector<std::shared_ptr<Texture>> textures)
{
	m_textures.insert(m_textures.end(), textures.begin(), textures.end());
}

void Mesh::setRawVertices(float* vertices, VerticesLayout& layout)
{
	m_numOfVertices = layout.numOfVertices;
	// calculate stride
	int stride = 0;
	for (auto entry : layout.entries)
	{
		stride += entry.second;
	}

	int offset = 0;

	// Parse vertices
	auto positions = std::make_shared<std::vector<glm::vec3>>();
	auto normals = std::make_shared<std::vector<glm::vec3>>();
	auto texcoords = std::make_shared<std::vector<glm::vec2>>();

	for (auto entry : layout.entries)
	{
		// Parse positions
		if (LayoutAttributes::Positions == entry.first)
		{
			positions->reserve(layout.numOfVertices * entry.second);
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 pos;
				for (int j = 0; j < entry.second; j++)
				{
					pos[j] = vertices[stride * i + j + offset];
				}
				positions->emplace_back(pos);
			}
			setPositions(positions);
		}

		// Parse normals
		else if (LayoutAttributes::Normals == entry.first)
		{
			normals->reserve(layout.numOfVertices * entry.second);
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 normal;
				for (int j = 0; j < entry.second; j++)
				{
					normal[j] = vertices[stride * i + j + offset];
				}
				normals->emplace_back(normal);
			}
			setNormals(normals);
		}

		// Parse texcoords
		else if (LayoutAttributes::Texcoords == entry.first)
		{
			texcoords->reserve(layout.numOfVertices * entry.second);
			for (int i = 0; i < layout.numOfVertices; i++)
			{
				glm::vec3 vec;
				for (int j = 0; j < entry.second; j++)
				{
					vec[j] = vertices[stride * i + j + offset];
				}
				texcoords->emplace_back(vec);
			}
			setTexcoords(texcoords);
		}

		offset += entry.second;
	}

	
}

void Mesh::setRawIndices(unsigned int* indices, size_t size)
{
	auto vec = std::make_shared<std::vector<unsigned int>>();
	vec->reserve(size);
	for (auto i=0; i<size; i++)
	{
		vec->push_back(indices[i]);
	}
	setIndices(vec);
}

void Mesh::SetTexturesInShader(std::shared_ptr<Shader>& shader)
{
	// TODO extract this logic into texture class
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;

	// Iterate the mesh's textures
	for (auto i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		std::string number = "";
		std::string name = m_textures[i]->GetType();
		if (name == Constants::g_textureDiffuse)
			number = std::to_string(diffuseNr++);
		else if (name == Constants::g_textureSpecular)
			number = std::to_string(specularNr++);

		shader->SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]->GetID());
	}
}
void Mesh::setPositions(std::shared_ptr<std::vector<glm::vec3>> positions)
{
	m_positions = positions;
}
void Mesh::setNormals(std::shared_ptr<std::vector<glm::vec3>> normals)
{
	m_normals = normals;
}
void Mesh::setNumOfVertices(size_t size)
{
	m_numOfVertices = size;
}
void Mesh::setTexcoords(std::shared_ptr<std::vector<glm::vec2>> texCoords)
{
	m_texcoords = texCoords;
}
void Mesh::setIndices(std::shared_ptr<std::vector<unsigned int>> indices)
{
	m_indices = indices;
}
void Mesh::build()
{
	// validate mesh data
	if (!m_positions || m_positions->empty())
	{
		logError("Cannot build mesh without position data.");
		return;
	}

	// validate numOfVertices of texcoord
	if (!m_texcoords)
	{
		logError("Missing texcoords data, mesh could not be built.");
		return;
	}
	// if mesh doesn't have normals calculate them
	if (!m_normals || m_normals->empty())
	{
		calculateNormals();
	}

	// Create verticies array
	std::vector<float> vertices;
	vertices.reserve(8 * m_numOfVertices);

	for (int i=0; i< m_numOfVertices; i++)
	{
		auto position = m_positions->at(i);
		vertices.emplace_back(position.x);
		vertices.emplace_back(position.y);
		vertices.emplace_back(position.z);

		auto normal = m_normals->at(i);
		vertices.emplace_back(normal.x);
		vertices.emplace_back(normal.y);
		vertices.emplace_back(normal.z);

		auto texcoord = m_texcoords->at(i);
		vertices.emplace_back(texcoord.x);
		vertices.emplace_back(texcoord.y);
	}

	// Create buffers
	m_vao = std::make_shared<VertexArrayObject>();
	if(m_indices)
		m_ibo = std::make_shared<ElementBufferObject>(&(m_indices->at(0)), m_indices->size());
	m_vbo = std::make_shared<VertexBufferObject>(&(vertices[0]), m_numOfVertices, sizeof(float) * 8);

	if (m_indices)
		m_vao->AttachBuffer(*m_vbo, *m_ibo);
	else
		m_vao->AttachBuffer(*m_vbo);
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
	m_vao = nullptr;
	m_ibo = nullptr;
	m_vbo = nullptr;
}

Mesh::~Mesh()
{
	logTrace( __FUNCTION__ );
	clearMesh();
}