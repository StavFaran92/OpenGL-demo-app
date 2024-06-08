#pragma once

#include "Core.h"
#include "Application.h"

#ifndef TESTBED
//#include "EntryPoint.h"
#endif

//external
#ifdef GUI
#include "ImguiHandler.h"
#include "imgui.h"
#endif

//sge
#include "Context.h"
#include "Window.h"
#include "Mesh.h"
#include "Object3D.h"
#include "Shader.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ICamera.h"
#include "FlyCamera.h"
#include "EditorCamera.h"
#include "Scene.h"
#include "Skybox.h"
#include "PostProcess.h"
#include "Input.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "EventSystem.h"
#include "Quad.h"
#include "Renderer.h"
#include "Box.h"
#include "Sphere.h"
#include "Configurations.h"
#include "MeshBuilder.h"
#include "ModelImporter.h"
#include "Resource.h"
#include "ObjectFactory.h"
#include "ObjectHandler.h"
#include "GuiMenu.h"
#include "Shader.h"
#include "Entity.h"
#include "ShapeFactory.h"
#include "Component.h"
#include "Random.h"
#include "TimeManager.h"
#include "ShaderBuilder.h"
#include "CommonTextures.h"
