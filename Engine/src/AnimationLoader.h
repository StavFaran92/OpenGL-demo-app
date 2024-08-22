#pragma once

#include <memory>
#include "Animation.h"
#include "Resource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Exporter.hpp>

#include "Core.h"

class EngineAPI AnimationLoader
{
public:
	Resource<Animation> import(const std::string& path);

	Resource<Animation> load(const std::string& path, Resource<Animation>& animation);

private:
	Assimp::Importer m_importer;
};