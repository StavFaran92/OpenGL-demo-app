#include "RubiksCube.h"


Box* createRubiksCubeBox()
{
	auto texture1 = Texture::loadTextureFromFile("Resources\\Textures\\plane.png");
	texture1->setType(Texture::Type::Diffuse);

	auto texture2 = Texture::loadTextureFromFile("Resources\\Textures\\plane.png");
	texture2->setType(Texture::Type::Specular);

	return (Box*)ModelBuilder::builder<Box>()
	.getMeshBuilder()
	.setColors(colors, 36)
	.addTexture(texture1)
	.addTexture(texture2)
	.getModelBuilder()
	.build();
}