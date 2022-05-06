#pragma once

#include "sge.h"

class MockMenu : public GuiMenu
{
public:
	MockMenu();
	// Inherited via GuiMenu
	virtual void display() override;
};

