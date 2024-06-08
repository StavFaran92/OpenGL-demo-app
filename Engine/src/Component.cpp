#include "Component.h"

#include "Engine.h"
#include "Context.h"
#include "Material.h"
#include "Transformation.h"
#include <GL/glew.h>

MaterialComponent::MaterialComponent()
{
	auto mat = Engine::get()->getDefaultMaterial();
	materials.push_back(mat);
}

InstanceBatch::InstanceBatch()
{
	glGenBuffers(1, &m_id);
}

InstanceBatch::InstanceBatch(const std::vector<Transformation>& transformations, Resource<Mesh> mesh)
	: mesh(mesh)
{
	glGenBuffers(1, &m_id);

	matrices.reserve(transformations.size());

	for (int i = 0; i < transformations.size(); i++)
	{
		matrices.push_back(transformations[i].getWorldTransformation());
	}

	build();
}

void InstanceBatch::addTransformation(const Transformation& transformation)
{
	matrices.push_back(transformation.getWorldTransformation());

	build();
}

void InstanceBatch::build()
{
	if (mesh.isEmpty() || matrices.empty()) return;

	mesh.get()->getVAO()->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, getCount() * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

	//VertexLayout layout;
	//layout.attribs = {
	//	LayoutAttribute::InstanceModel_0,
	//	LayoutAttribute::InstanceModel_1,
	//	LayoutAttribute::InstanceModel_2,
	//	LayoutAttribute::InstanceModel_3,
	//};

	//mesh.get()->setVertexLayout(layout);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);
}