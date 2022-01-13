#include "Mesh.h"

Mesh::Mesh(std::shared_ptr<std::vector<Vertex>> vertices, std::shared_ptr<std::vector<unsigned int>> indices, std::vector<std::shared_ptr<Texture>> textures) :
	m_vertices(vertices), m_indices(indices), m_textures(textures)
{
	logTrace( __FUNCTION__ );

	m_vao = std::make_shared<VertexArrayObjectWrapper>();
	m_ibo = std::make_shared<ElementBufferObjectWrapper>(&(m_indices->at(0)), indices->size());
	m_vbo = std::make_shared<VertexBufferObjectWrapper>(&(vertices->at(0)), vertices->size());

	m_vao->AttachBuffer(*m_vbo, *m_ibo);
}

//void Mesh::CreateMesh()
//{
//
//}

void Mesh::RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr < Renderer >renderer)
{
	if (shader->IsTexturesEnabled())
	{
		SetTexturesInShader(shader);
	}

	renderer->Draw(*m_vao, shader);
	glActiveTexture(GL_TEXTURE0);
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