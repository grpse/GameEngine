#pragma once
#include "LinearMath.h"

struct Light {
	enum class Type {
		Directional, Area, Point, Spot
	};

	Vector3 position;
	Vector3 direction;
};