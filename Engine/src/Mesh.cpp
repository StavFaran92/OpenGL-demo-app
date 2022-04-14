#include "Mesh.h"

#include "MeshBuilder.h"
#include "Logger.h"

#include <GL\glew.h>

Mesh::Mesh()
{
	logInfo(__FUNCTION__);
}

std::vector<const Texture*> Mesh::getTextures() const
{
	std::vector<const Texture*> textures;

	for (auto texture : m_textures)
	{
		textures.emplace_back(texture.get());
	}

	return textures;
}

void Mesh::render(Shader& shader, IRenderer& renderer)
{
	if (shader.IsTexturesEnabled())
	{
		SetTexturesInShader(shader);
	}

	renderer.draw(*m_vao, shader);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::addTexture(Texture* texture)
{
	if (texture == nullptr)
	{
		logError("Cannot add a null texture to mesh.");
		return;
	}

	m_textures.push_back(std::shared_ptr<Texture>(texture));
}

void Mesh::addTextures(std::vector<Texture*>& textures)
{
	for (auto texture : textures)
	{
		addTexture(texture);
	}
}

void Mesh::addTexture(const std::shared_ptr<Texture>& texture)
{
	m_textures.push_back(texture);
}

void Mesh::addTextures(const std::vector<std::shared_ptr<Texture>>& textures)
{
	for (auto texture : textures)
	{
		addTexture(texture);
	}
}

void Mesh::SetTexturesInShader(Shader& shader)
{
	// Initialized counters
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;

	// Iterate the mesh's textures
	for (auto i = 0; i < m_textures.size(); i++)
	{
		// get type count
		std::string count;
		auto type = m_textures[i]->getType();
		if (type == Texture::Type::Diffuse)
			count = std::to_string(diffuseNr++);
		else if (type == Texture::Type::Specular)
			count = std::to_string(specularNr++);

		// Get type as string
		auto typeStr = Texture::textureTypeToString(type);

		// Activate texture unit i
		glActiveTexture(GL_TEXTURE0 + i);

		// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
		glBindTexture(GL_TEXTURE_2D, m_textures[i]->getID());

		// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
		shader.SetInt(("material." + typeStr + count).c_str(), i);
	}
}
void Mesh::setPositions(std::shared_ptr<std::vector<glm::vec3>> positions)
{
	m_positions = positions;
}

const std::vector<glm::vec3>* Mesh::getPositions() const
{
	return m_positions.get();
}

void Mesh::setNormals(std::shared_ptr<std::vector<glm::vec3>> normals)
{
	m_normals = normals;
}

const std::vector<glm::vec3>* Mesh::getNormals() const
{
	return m_normals.get();
}

void Mesh::setNumOfVertices(size_t size)
{
	m_numOfVertices = size;
}

size_t Mesh::getNumOfVertices() const
{
	return m_numOfVertices;
}

void Mesh::setTexcoords(std::shared_ptr<std::vector<glm::vec2>> texCoords)
{
	m_texcoords = texCoords;
}

const std::vector<glm::vec2>* Mesh::getTexcoords() const
{
	return m_texcoords.get();
}

void Mesh::setIndices(std::shared_ptr<std::vector<unsigned int>> indices)
{
	m_indices = indices;
}

const std::vector<unsigned int>* Mesh::getIndices() const
{
	return m_indices.get();
}

void Mesh::setColors(std::shared_ptr<std::vector<glm::vec3>> colors)
{
	m_colors = colors;
}

const std::vector<glm::vec3>* Mesh::getColors() const
{
	return m_colors.get();
}

bool Mesh::build()
{
	// validate mesh data
	if (!m_positions || m_positions->empty())
	{
		logError("Cannot build mesh without position data.");
		return false;
	}

	if (m_numOfVertices == 0)
	{
		logError("Number of vertices cannot be 0.");
		return false;
	}

	if (m_layout.attribs.size() == 0)
	{
		logError("Number of attributes cannot be 0.");
		return false;
	}

	// if mesh doesn't have normals calculate them
	if (!m_normals || m_normals->empty())
	{
		calculateNormals();
	}

	// calculate stride
	int stride = 0;
	for (auto entry : m_layout.attribs)
	{
		stride += getAttributeSize(entry);
	}

	// Update layout info
	m_layout.numOfVertices = m_numOfVertices;
	m_layout.stride = stride;

	// Create verticies array
	int offset = 0;
	std::vector<float> vertices(stride * m_numOfVertices, 0); //TODO optimize

	for (auto entry : m_layout.attribs)
	{
		// Parse positions
		if (LayoutAttribute::Positions == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto pos = m_positions->at(i);
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
				auto normal = m_normals->at(i);
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
				auto texCoord = m_texcoords->at(i);
				vertices[stride * i + offset + 0] = texCoord.x;
				vertices[stride * i + offset + 1] = texCoord.y;
			}
		}

		// Parse colors
		else if (LayoutAttribute::Colors == entry)
		{
			for (int i = 0; i < m_layout.numOfVertices; i++)
			{
				auto color = m_colors->at(i);
				vertices[stride * i + offset + 0] = color.x;
				vertices[stride * i + offset + 1] = color.y;
				vertices[stride * i + offset + 2] = color.z;
			}
		}

		offset += getAttributeSize(entry);
	}

	// Create buffers
	m_vao = std::make_shared<VertexArrayObject>();

	if(m_indices)
		m_ibo = std::make_shared<ElementBufferObject>(&(m_indices->at(0)), m_indices->size());

	m_vbo = std::make_shared<VertexBufferObject>(&(vertices[0]), m_numOfVertices, sizeof(float) * stride);

	m_vao->AttachBuffer(*m_vbo, m_ibo.get(), m_layout);

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
	m_positions = nullptr;
	m_normals = nullptr;
	m_texcoords = nullptr;
	m_indices = nullptr;
	m_textures.clear();
	m_colors = nullptr;
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

Mesh::~Mesh()
{
	logInfo( __FUNCTION__ );
	clearMesh();
}