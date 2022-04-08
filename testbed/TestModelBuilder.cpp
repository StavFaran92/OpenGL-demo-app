#include "gtest/gtest.h"
#include <gtest/gtest_prod.h>
#include "sge.h"

TEST(TestModelBuilder, basic)
{
	ModelBuilder::builder<Box>()
		.build();
}
