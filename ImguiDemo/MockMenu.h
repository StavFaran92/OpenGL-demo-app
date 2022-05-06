#pragma once

#include "sge.h"

class MockMenu : public GuiMenu
{
	// Inherited via GuiMenu
	virtual void display() override;
};

