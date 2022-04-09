#pragma once
#include "gtest/gtest.h"
#include <gtest/gtest_prod.h>
#include "sge.h"
#include "Engine.h"
#include <thread>
#include <chrono>

class TestEngine
{
public:
	void startEngine()
	{
		if (engine)
		{
			std::cout << "Engine already started." << std::endl;
			return;
		}

		engine = Engine::get();
		engine->init();
	}
	void stopEngine()
	{
		if (!engine)
		{
			std::cout << "Engine not started." << std::endl;
			return;
		}

		engine->close();
	}

private:
	Engine* engine = nullptr;
};

