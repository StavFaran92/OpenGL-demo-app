#include "RenderView.h"

RenderView::Viewport RenderView::getViewport() const
{
    return m_viewport;
}

void RenderView::setCamera(const Entity& camera)
{
    m_camera = camera;
}

unsigned int RenderView::getRenderTargetID() const
{
    return m_renderTarget;
}

void RenderView::setRenderTargetID(unsigned int targetID)
{
    m_renderTarget = targetID;
}

const Entity& RenderView::getCamera() const
{
    return m_camera;
}
