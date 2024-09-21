#pragma once

#include <string>
#include <vector>

#include "Core.h"
#include "VertexArrayObject.h"
#include "Component.h"
#include "TerrainMaterial.h"
#include "TextureArray.h"

static const int MAX_TEXTURE_COUNT = 4;

struct TextureBlend
{
	Resource<Texture> texture;
	float blend = 0;
	float scaleX = 1;
	float scaleY = 1;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(texture, blend, scaleX, scaleY);
	}
};

class EngineAPI Terrain : public Component
{
public:
	static Terrain generateTerrain(int width, int height, float scale, const std::string& heightMapFilepath);
	static Terrain generateTerrain(int width, int height, float scale, Resource<Texture> heightMap);


	Terrain() = default;

	Resource<Mesh> getMesh() const;
	Resource<Texture> getHeightmap() const;

	float getScale() const;
	int getWidth() const;
	int getHeight() const;

	void setTexture(int index, Resource<Texture> texture);
	void setTextureScaleX(int index, float scaleX);
	void setTextureScaleY(int index, float scaleY);
	void setTextureBlend(int index, float val);

	Resource<Texture>& getTexture(int index);
	float getTextureBlend(int index) const;
	glm::vec2 getTextureScale(int index) const;

	int getTextureCount() const;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_mesh, m_heightmap, m_width, m_height, m_scale, m_textureCount, m_textureBlends);
	}

	
	Resource<Texture> m_heightmap;

	int m_width = 100;
	int m_height = 100;
	int m_scale = 1;
	int m_textureCount = 0;

	std::vector<TextureBlend> m_textureBlends {  };
	//std::vector<Resource<Texture>> m_textures{  };
	//std::vector<float> m_blends{ };

private:
	Resource<Mesh> m_mesh;
	//std::shared_ptr<TextureArray> m_textures;
	
};