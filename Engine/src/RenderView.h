#pragma once

#include "Entity.h"

class RenderView
{
public:
	struct Viewport
	{
		int x, y, w, h;
	};

	RenderView(Viewport viewport, const Entity& camera, unsigned int renderTarget)
		: m_viewport(viewport), m_camera(camera), m_renderTarget(renderTarget)
	{
		
	}

	Viewport getViewport() const;

	const Entity& getCamera() const;

	void setCamera(const Entity& camera);

	unsigned int getRenderTargetID() const;

	void setRenderTargetID(unsigned int targetID);

private:
	Viewport m_viewport;
	Entity  m_camera = Entity::EmptyEntity;
	unsigned int m_renderTarget = 0; // this should hold FBO to be owner of it.
};