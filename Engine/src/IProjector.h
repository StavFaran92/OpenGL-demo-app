#pragma once

#include "Core.h"

class IProjector
{
public:
	virtual bool init() = 0;
	virtual void enableWriting() = 0;
	virtual void disableWriting() = 0;
};

