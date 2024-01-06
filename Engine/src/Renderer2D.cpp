#include "Renderer2D.h"

#include "Logger.h"
#include <GL/glew.h>

#include "VertexArrayObject.h"
#include "ICamera.h"
#include "Mesh.h"

void Renderer2D::render(const DrawQueueRenderParams& renderParams)
{
	draw(*renderParams.mesh->getVAO());
}

void Renderer2D::renderScene(DrawQueueRenderParams& renderParams)
{
}
