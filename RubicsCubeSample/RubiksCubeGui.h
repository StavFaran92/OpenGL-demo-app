#pragma once

#include "sge.h"

#include "RubiksCube.h"

class RubiksCubeGui : public GuiMenu
{
public:
	RubiksCubeGui(RubiksCube* rubiksCube);
	// Inherited via GuiMenu
	virtual void display() override;

private:
	RubiksCube* m_rubiksCube = nullptr;
};

