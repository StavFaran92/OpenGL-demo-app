#include "ShadowSystem.h"

#include "FrameBufferObject.h"
#include "Resource.h"
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
#include "Texture.h"

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

ShadowSystem::ShadowSystem(Context* context, Scene* scene)
	: m_context(context), m_scene(scene)
{}

bool ShadowSystem::init()
{
	//m_scene->addRenderCallback(Scene::RenderPhase::PRE_RENDER_BEGIN, [=](const IRenderer::DrawQueueRenderParams* params) {

	//	renderToDepthMap(params);
	//});

	m_fbo.bind();

	// Generate 2D texture
	m_depthMapTexture = Texture::createEmptyTexture(SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attach texture to FBO
	m_fbo.attachTexture(m_depthMapTexture.get()->getID(), GL_DEPTH_ATTACHMENT);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (!m_fbo.isComplete())
	{
		logError("FBO is not complete!");
		return false;
	}

	m_fbo.unbind();

	m_simpleDepthShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/SimpleDepthShader.glsl");

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
	float near_plane = 1.0f, far_plane = 40.f;
	glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);

	auto& trans = e.getComponent<Transformation>();
	auto& direction = trans.getLocalRotationVec3();

	auto right = glm::normalize(glm::cross(direction, { 0,0,1 }));
	auto up = glm::normalize(glm::cross(direction, right));

	// Generate lookAt light matrix 
	glm::mat4 dirLightView = glm::lookAt(
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.0f, 10.0f, 0.0f) + direction,
		up);

	m_lightSpaceMatrix = lightProjection * dirLightView;

	auto drawQueueRenderParams = *params;

	drawQueueRenderParams.shader = m_simpleDepthShader.get();
	drawQueueRenderParams.shader->setUniformValue("lightSpaceMatrix", m_lightSpaceMatrix);

	// Render Scene 
	for (auto&& [entity, mesh, transform, renderable] : 
		params->registry->view<MeshComponent, Transformation, RenderableComponent>().each())
	{
		

		Entity entityhandler{ entity, m_scene };
		drawQueueRenderParams.entity = &entityhandler;
		drawQueueRenderParams.mesh = mesh.mesh.get();
		drawQueueRenderParams.shader->setUniformValue("model", transform.getWorldTransformation());
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

	//auto phongShader = m_context->getStandardShader();

	//// set lightSpaceMatrix in shader
	//phongShader->setValue("lightSpaceMatrix", lightSpaceMatrix);

	////TODO Fix
	//// set Depth map texture in shader
	//phongShader->setValue("shadowMap", 2);
	//m_depthMapTexture->setSlot(2);
	//m_depthMapTexture->bind();

	//m_bufferDisplay->draw(m_depthMapTexture);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}

Resource<Texture> ShadowSystem::getShadowMap() const
{
	return m_depthMapTexture;
}

glm::mat4 ShadowSystem::getLightSpaceMat() const
{
	return m_lightSpaceMatrix;
}
