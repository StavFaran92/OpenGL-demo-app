#pragma once

#include "sge.h"

class RubiksCubeFace : public Object3D
{
public:
	void setCube_Bottom();
	void setCube_Top();
	void setCube_Left();
	void setCube_Right();
	void setCube_TopRight();
	void setCube_TopLeft();
	void setCube_BottomRight();
	void setCube_BottomLeft();

	void getCube_Bottom();
	void getCube_Top();
	void getCube_Left();
	void getCube_Right();
	void getCube_TopRight();
	void getCube_TopLeft();
	void getCube_BottomRight();
	void getCube_BottomLeft();

private:



};