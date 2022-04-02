#include "Quad.h"

#include "Mesh.h"

static const float vertices[] = {
	// positions          // colors           // texture coords
 1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
 1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
-1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // top left 
};

static const unsigned int indices[] = {
	3, 2, 1,   // first triangle
	1, 0, 3    // second triangle
};

Quad* Quad::generateQuad()
{
	auto quad = new Quad();

	auto mesh = generateMesh();

	auto texturediff = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	texturediff->setType(Texture::Type::Diffuse);

	auto textureSpec = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	textureSpec->setType(Texture::Type::Specular);

	mesh->addTexture(texturediff);
	mesh->addTexture(textureSpec);

	std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
	quad->UseMaterial(material);

	quad->m_meshes.push_back(std::shared_ptr<Mesh>(mesh));

	return quad;
}

Mesh* Quad::generateMesh()
{
	auto mesh = new Mesh();
	//Mesh::VerticesLayout layout;
	//layout.numOfVertices = 4;
	//layout.attribs.emplace_back(LayoutAttributes::Positions);
	//layout.attribs.emplace_back(LayoutAttributes::Normals);
	//layout.attribs.emplace_back(LayoutAttributes::Texcoords);
	//mesh->setRawVertices((float*)vertices, layout);
	//mesh->setRawIndices((unsigned int*)indices, sizeof(indices) / sizeof(unsigned int));
	//mesh->build();

	return mesh;
}
