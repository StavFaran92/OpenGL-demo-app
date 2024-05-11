#include "ProjectManager.h"

std::shared_ptr<Context> ProjectManager::loadProject(const std::string& filePath)
{
	// create context

	// open file

	// get scene registry file

	// populate scenes ECS in context

	// get mesh list

	// create mesh list

	// for each mesh
		// open bin file

		// create Mesh Data object

		// create mesh

		// place mesh in map

	// do the above for textures

	// get scene list 

	// for each scene
		// get mesh component list
		
		// for each mesh component
			// place mesh resource in mesh component

		// get material component list

		// for each material component
			// place texture resource in material component

	// return context

	return nullptr;
}
