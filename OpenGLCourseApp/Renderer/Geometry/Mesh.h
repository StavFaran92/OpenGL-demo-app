#pragma once

#include <memory>

#include <GL\glew.h>

#include "Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"

#include "Renderer/Buffers/VertexBufferObjectWrapper.h"
#include "Renderer/Buffers/ElementBufferObjectWrapper.h"
#include "Renderer/Buffers/VertexBufferLayout.h"
#include "Renderer/Buffers/VertexArrayObjectWrapper.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh
{  
public:
	Mesh(Vertex* vertices, int numOfVertices,
		unsigned int* indices, int numOfIndices,
		Texture* textures, int numOfTextures);
	
	void RenderMesh(const Shader& shader, const Renderer& renderer);

	~Mesh();
private:
	void ClearMesh();
private:
	// mesh data
	Vertex* m_vertices;
	unsigned int* m_indices;
	Texture* m_textures;

	// render data
	std::shared_ptr<ElementBufferObjectWrapper>  m_ibo;
	std::shared_ptr < VertexBufferObjectWrapper> m_vbo;
	std::shared_ptr < VertexArrayObjectWrapper>  m_vao;

	GLsizei m_indexCount = 0;
};

