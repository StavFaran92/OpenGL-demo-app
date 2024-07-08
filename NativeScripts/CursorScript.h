#pragma once

#include "sge.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>

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

CEREAL_REGISTER_TYPE(CursorScript);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptableEntity, CursorScript)