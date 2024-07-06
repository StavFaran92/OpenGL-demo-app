#pragma once

#include "sge.h"

class CursorScript : public ScriptableEntity
{
public:
	void onCreate() override
	{
		SDL_ShowCursor(SDL_DISABLE);

		Entity cursor = Engine::get()->getContext()->getActiveScene()->getEntityByName("Cursor");
		auto& cursorImage = cursor.getComponent<ImageComponent>();
		cursorImage.position.x = (Engine::get()->getWindow()->getWidth() - cursorImage.size.x) / 2.f;
		cursorImage.position.y = (Engine::get()->getWindow()->getHeight() - cursorImage.size.y) / 2.f;
	}

	void onUpdate(float deltaTime) override
	{
	}
};
