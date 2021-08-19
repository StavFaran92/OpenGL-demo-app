#include "TestClearColor.h"

#include "GL/glew.h"
#include "../Vendor/imgui/imgui.h"

test::TestClearColor::TestClearColor()
	:mClearColor{0.2f, 0.5f, 0.1f, 1.0f}
{
}

test::TestClearColor::~TestClearColor()
{
}

void test::TestClearColor::OnUpdate(float deltaTime)
{
}

void test::TestClearColor::OnRender()
{
	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2], mClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void test::TestClearColor::OnGuiRender()
{
	ImGui::ColorEdit4("Clear Color", mClearColor);
}
