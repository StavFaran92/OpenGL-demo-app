#pragma once
#include <vector>
#include <string>
#include <functional>



#include "Test.h"

namespace test {
	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTest);
		~TestMenu() override;

		void OnGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string name)
		{
			mTests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& mCurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> mTests;
	};

}