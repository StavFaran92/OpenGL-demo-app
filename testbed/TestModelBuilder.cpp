#include "gtest/gtest.h"
#include <gtest/gtest_prod.h>
#include "sge.h"
#include "TestEngine.h"

/**
 * Test basic funcionallity of the model builder.
 * 
 * \param 
 * \param 
 * \return 
 */
TEST(TestModelBuilder, basic)
{
	TestEngine testEngine;
	testEngine.startEngine();

	Model* model = ModelBuilder::builder<Box>()
		.build();

	ASSERT_TRUE(model != nullptr);

	testEngine.stopEngine();
}

/**
 * False test, we check if the engine will not create a model when invalid data speicifed
 *
 * \param
 * \param
 * \return
 */
TEST(TestModelBuilder, invalidModel)
{
	TestEngine testEngine;
	testEngine.startEngine();

	Model* model = ModelBuilder::builder<Model>()
		.build();

	ASSERT_TRUE(model == nullptr);

	testEngine.stopEngine();
}

/**
 * Test if setShader in model builder actually sets the shader in the created model
 * 
 * \param 
 * \param 
 * \return 
 */
TEST(TestModelBuilder, setShader)
{
	TestEngine testEngine;
	testEngine.startEngine();

	Shader* shader = Shader::PhongShader();
	int expectedID = shader->getID();

	Model* model = ModelBuilder::builder<Box>()
		.setShader(*shader)
		.build();

	ASSERT_TRUE(model != nullptr);

	int actualID = model->getShader()->getID();

	EXPECT_EQ(expectedID, actualID);

	testEngine.stopEngine();
}


/**
 * Test if setShader works With copy 
 *
 * \param
 * \param
 * \return
 */
TEST(TestModelBuilder, setShaderWithCopy)
{
	TestEngine testEngine;
	testEngine.startEngine();

	Shader* shader = Shader::PhongShader();

	Model* model = ModelBuilder::builder<Box>()
		.setShader(*shader, true)
		.build();

	// We delete the original Shader to check if a copy was succesflly made
	delete shader;

	ASSERT_TRUE(model != nullptr);

	int actualID = model->getShader()->getID();

	EXPECT_NE(0, actualID);

	testEngine.stopEngine();
}