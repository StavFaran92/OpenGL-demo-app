#include "Component.h"

#include "Engine.h"
#include "Context.h"

MaterialComponent::MaterialComponent()
{
	auto mat = Engine::get()->getContext()->getDefaultMaterial();
	materials.push_back(mat);
}