#include "gtest/gtest.h"
#include <gtest/gtest_prod.h>
#include "sge.h"
#include "TestEngine.h"

TEST(TestModelBuilder, basic)
{
	TestEngine testEngine;
	testEngine.startEngine();

	ModelBuilder::builder<Box>()
		.build();

	testEngine.stopEngine();
}
