#include "ScreenBufferDisplay.h"

#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "Renderer2D.h"
#include "ScreenQuad.h"
#include "Engine.h"
#include "Window.h"
#include "Logger.h"
#include "Resource.h"
#include "ObjectHandler.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "Texture.h"

#include "GL/glew.h"

ScreenBufferDisplay::ScreenBufferDisplay(Scene* scene)
{
	m_scene = scene;
}

bool ScreenBufferDisplay::init(int windowWidth, int windowHeight)
{
	// Generate screen quad
	m_quad = ScreenQuad::GenerateScreenQuad(m_scene);
	
	// Generate screen shader
	m_screenShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/ScreenBufferDisplayShader.glsl");

	// Generate screen renderer
	m_renderer = std::make_shared<Renderer2D>();

	return true;
}

void ScreenBufferDisplay::draw(Resource<Texture> textureHandler)
{
	// Clean buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_screenShader->use();

	textureHandler.get()->bind();

	m_renderer->SetDrawType(Renderer::DrawType::Triangles);
	
	auto& mesh = m_quad.getComponent<MeshComponent>();

	IRenderer::DrawQueueRenderParams renderParams;
	renderParams.mesh = mesh.mesh.get();
	renderParams.shader = m_screenShader.get();
	m_renderer->render(renderParams);

	//mesh.mesh->render(*m_screenShader, *m_renderer);

	textureHandler.get()->unbind();
}