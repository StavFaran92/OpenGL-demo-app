#include "TestMenu.h"
#include "../Vendor/imgui/imgui.h"


namespace test
{

	TestMenu::TestMenu(Test*& currentTest)
		:mCurrentTest(currentTest)
	{

	}

	TestMenu::~TestMenu()
	{
	}

	void TestMenu::OnGuiRender()
	{
		for (auto& test : mTests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				mCurrentTest = test.second();
			}
		}
		
	}



}