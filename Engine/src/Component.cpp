#include "Component.h"

#include "Engine.h"
#include "Context.h"
#include "Material.h"

MaterialComponent::MaterialComponent()
{
	auto mat = Engine::get()->getDefaultMaterial();
	materials.push_back(mat);
}