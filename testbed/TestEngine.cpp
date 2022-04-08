#include "gtest/gtest.h"
#include <gtest/gtest_prod.h>
#include "sge.h"
#include <thread>
#include <chrono>


//class MockApp : public Application
//{
//	// Inherited via Application
//	virtual void start() override
//	{
//	}
//	virtual void draw() override
//	{
//	}
//};
//
//class MockEngine : public Engine
//{
//public:
//	MockEngine() = default;
//	FRIEND_TEST(TestEngine, Basic);
//};
//
//Application* CreateApplication()
//{
//	return new MockApp();
//}
//
//TEST(TestEngine, Basic) {
//	
//	Engine* engine = MockEngine::get();
//	engine->init();
//
//	auto app = CreateApplication();
//	app->start();
//
//	engine.run(app);
//
//	app->close();
//	engine.close();
//}


