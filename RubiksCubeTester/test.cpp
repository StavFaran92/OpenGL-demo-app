#include "gtest/gtest.h"

#include "sge.h"

#include "TestEngine.h"
#include "RubiksCubeFace.h"
#include "RubiksCubeEnt.h"
#include "RubiksCube.h"

TEST(TestRubiksCubeEnt, TestBasicSwap) {

	TestEngine testEngine;
	testEngine.startEngine();

	auto face_a = new RubiksCubeFace(Axis::X, 0, 3);
	auto face_b = new RubiksCubeFace(Axis::X, 1, 3);

	auto box_a = new RubiksCubeEnt();
	auto box_b = new RubiksCubeEnt();

	box_a->setFaceData(Axis::X, face_a, 0, 0);
	box_b->setFaceData(Axis::X, face_b, 1, 1);

	auto oldFaceData_a = box_a->getFaceData(Axis::X);
	auto oldFace_a = oldFaceData_a.face;
	auto oldFaceX_a = oldFaceData_a.x;
	auto oldFaceY_a = oldFaceData_a.y;

	auto oldFaceData_b = box_b->getFaceData(Axis::X);
	auto oldFace_b = oldFaceData_b.face;
	auto oldFaceX_b = oldFaceData_b.x;
	auto oldFaceY_b = oldFaceData_b.y;

	RubiksCubeEnt::swapFaces(box_a, box_b);

	auto newFaceData_a = box_a->getFaceData(Axis::X);
	auto newFace_a  = newFaceData_a.face;
	auto newFaceX_a = newFaceData_a.x;
	auto newFaceY_a = newFaceData_a.y;

	auto newFaceData_b = box_b->getFaceData(Axis::X);
	auto newFace_b = newFaceData_b.face;
	auto newFaceX_b = newFaceData_b.x;
	auto newFaceY_b = newFaceData_b.y;

	EXPECT_EQ(oldFace_a, newFace_b);
	EXPECT_EQ(oldFace_b, newFace_a);

	EXPECT_EQ(oldFaceX_a, newFaceX_b);
	EXPECT_EQ(oldFaceX_b, newFaceX_a);

	EXPECT_EQ(oldFaceY_a, newFaceY_b);
	EXPECT_EQ(oldFaceY_b, newFaceY_a);
}