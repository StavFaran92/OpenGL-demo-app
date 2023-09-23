#include "ShadowSystem.h"

#include "FrameBufferObject.h"
#include "TextureHandler.h"
#include "gl/glew.h"
#include "Logger.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Component.h"
#include "Engine.h"
#include "Window.h"
#include "DirectionalLight.h"
#include "Transformation.h"
#include "Shader.h"
#include "ScreenBufferDisplay.h"
#include "Context.h"

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

ShadowSystem::ShadowSystem(Context* context, Scene* scene)
	: m_context(context), m_scene(scene)
{}

bool ShadowSystem::init()
{
	m_scene->addRenderCallback(Scene::RenderPhase::PRE_RENDER_BEGIN, [=](const IRenderer::DrawQueueRenderParams* params) {

		renderToDepthMap(params);
	});

	m_fbo.bind();

	// Generate 2D texture
	m_depthMapTexture = Texture::createEmptyTexture(SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

	// Attach texture to FBO
	m_fbo.attachTexture(m_depthMapTexture->getID(), GL_DEPTH_ATTACHMENT);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (!m_fbo.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_fbo.unbind();

	m_simpleDepthShader = Shader::createShared<Shader>("Resources/Engine/Shaders/SimpleDepthShader.vert", "Resources/Engine/Shaders/SimpleDepthShader.frag");

	//m_bufferDisplay = std::make_shared<ScreenBufferDisplay>(m_scene);
	//m_bufferDisplay->init(Engine::get()->getWindow()->getWidth(), Engine::get()->getWindow()->getHeight());

	return true;
}

void ShadowSystem::renderToDepthMap(const IRenderer::DrawQueueRenderParams* params)
{
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	// Set shadow map viewport
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Bind FBO
	m_fbo.bind();

	// Clear buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Configure Shadow pass matrices
	auto entt = params->registry->view<DirectionalLight>().front();
	Entity e{ entt, params->scene };
	auto& dirLight = e.getComponent<DirectionalLight>();

	//todo verify exists

	// Generate Orthogonal projection
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	// Generate lookAt light matrix 
	glm::mat4 dirLightView = glm::lookAt(
		glm::vec3(1.0f, 4.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = lightProjection * dirLightView;

	auto drawQueueRenderParams = *params;

	drawQueueRenderParams.shader = m_simpleDepthShader.get();
	drawQueueRenderParams.shader->setValue("lightSpaceMatrix", lightSpaceMatrix);

	// Render Scene 
	for (auto&& [entity, mesh, transform, renderable] : 
		params->registry->view<MeshComponent, Transformation, RenderableComponent>().each())
	{
		

		Entity entityhandler{ entity, m_scene };
		drawQueueRenderParams.entity = &entityhandler;
		drawQueueRenderParams.mesh = mesh.mesh.get();
		drawQueueRenderParams.shader->setValue("model", transform.getWorldTransformation());
		//auto tempModel = transform.getWorldTransformation();
		drawQueueRenderParams.model = nullptr;
		drawQueueRenderParams.view = nullptr;
		drawQueueRenderParams.projection = nullptr;

		// draw model
		params->renderer->render(drawQueueRenderParams);
	};

	// Unbind FBO
	m_fbo.unbind();

	// Set viewport back to normal
	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();
	glViewport(0, 0, width, height);

	auto phongShader = m_context->getStandardShader();

	// set lightSpaceMatrix in shader
	phongShader->setValue("lightSpaceMatrix", lightSpaceMatrix);

	//TODO Fix
	// set Depth map texture in shader
	phongShader->setValue("shadowMap", 2);
	m_depthMapTexture->setSlot(2);
	m_depthMapTexture->bind();

	//m_bufferDisplay->draw(m_depthMapTexture);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}
