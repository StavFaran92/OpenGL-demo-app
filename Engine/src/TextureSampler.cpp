#include "TextureSampler.h"
#include "CommonTextures.h"

TextureSampler::TextureSampler()
	: texture( Engine::get()->getCommonTextures()->getTexture(CommonTextures::TextureType::WHITE_1X1))
{
}
