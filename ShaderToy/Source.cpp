#include "EntryPoint.h"
#include "sge.h"

static Color getPixel(int i, int j)
{
	int width = Engine::get()->getWindow()->getWidth();
	int height = Engine::get()->getWindow()->getHeight();

	unsigned char r = static_cast<unsigned char>(((float)j / width) * 255);
	unsigned char g = static_cast<unsigned char>(((float)i / height) * 255);

	return { r, g, 0 };
}

class ShaderToy : public Application
{
public:

	void start() override
	{
		auto ent = Engine::get()->getContext()->getActiveScene()->createEntity();

		int width = Engine::get()->getWindow()->getWidth();
		int height = Engine::get()->getWindow()->getHeight();

		auto tex = Texture::createEmptyTexture(width, height);

		auto& img = ent.addComponent<ImageComponent>(tex);
		img.size = glm::vec2(width, height);
		img.position = glm::vec2(0, 0);

		int* pixels = new int[width * height];
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				auto pixel = getPixel(i, j);
				pixels[i * width + j] = 0xFF << 24 | (pixel.b << 16) | (pixel.g << 8) | pixel.r;
			}
		}

		tex.get()->setData(0, 0, width, height, pixels);
	}

};

Application* CreateApplication()
{
	return new ShaderToy();
}