#pragma once

#include "Plane.h"

struct Frustum
{
	Frustum(Plane znear, Plane zfar, Plane right, Plane left, Plane up, Plane down)
		: znear(znear), zfar(zfar), right(right), left(left), up(up), down(down)
	{

	}

	Plane znear;
	Plane zfar;
	Plane right;
	Plane left;
	Plane up;
	Plane down;
};