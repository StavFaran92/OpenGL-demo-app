#include "gtest/gtest.h"
#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;

	void start() override
	{

	}

	void draw() override
	{

	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}