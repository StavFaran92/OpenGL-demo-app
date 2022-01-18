#include "Mesh.h"

Mesh::Mesh(std::shared_ptr<std::vector<Vertex>> vertices, std::shared_ptr<std::vector<unsigned int>> indices) :
	m_vertices(vertices), m_indices(indices)
{
	logTrace( __FUNCTION__ );

	m_vao = std::make_shared<VertexArrayObject>();
	m_ibo = std::make_shared<ElementBufferObject>(&(m_indices->at(0)), indices->size());
	m_vbo = std::make_shared<VertexBufferObject>(&(vertices->at(0)), vertices->size());

	m_vao->AttachBuffer(*m_vbo, *m_ibo);
}

Mesh::Mesh(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize)
{
	logTrace(__FUNCTION__);

	m_vao = std::make_shared<VertexArrayObject>();
	m_ibo = std::make_shared<ElementBufferObject>(indices, indicesSize);
	m_vbo = std::make_shared<VertexBufferObject>(vertices, verticesSize / sizeof(Vertex));

	m_vao->AttachBuffer(*m_vbo, *m_ibo);
}

Mesh::Mesh(std::shared_ptr<std::vector<Vertex>> vertices) :
	m_vertices(vertices), m_indices(nullptr)
{
	logTrace(__FUNCTION__);

	m_vao = std::make_shared<VertexArrayObject>();
	m_vbo = std::make_shared<VertexBufferObject>(&(vertices->at(0)), vertices->size());

	m_vao->AttachBuffer(*m_vbo);
}

Mesh::Mesh(float* vertices, size_t verticesSize)
{
	logTrace(__FUNCTION__);

	m_vao = std::make_shared<VertexArrayObject>();
	m_vbo = std::make_shared<VertexBufferObject>(vertices, verticesSize / sizeof(Vertex));

	m_vao->AttachBuffer(*m_vbo);
}

//void Mesh::CreateMesh()
//{
//
//}

void Mesh::RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr < IRenderer >renderer)
{
	if (shader->IsTexturesEnabled())
	{
		SetTexturesInShader(shader);
	}

	renderer->Draw(*m_vao, shader);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::AddTexture(std::shared_ptr<Texture> texture)
{
	m_textures.push_back(texture);
}

void Mesh::AddTextures(std::vector<std::shared_ptr<Texture>> textures)
{
	m_textures.insert(m_textures.end(), textures.begin(), textures.end());
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
void Mesh::ClearMesh()
{
	m_vao = nullptr;
	m_ibo = nullptr;
	m_vbo = nullptr;
}

Mesh::~Mesh()
{
	logTrace( __FUNCTION__ );
	ClearMesh();
}