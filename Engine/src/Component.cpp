#include "Component.h"

#include "Engine.h"
#include "Context.h"
#include "Material.h"

MaterialComponent::MaterialComponent()
{
	auto mat = std::make_shared<Material>(*Engine::get()->getContext()->getDefaultMaterial().get());
	materials.push_back(mat);
}