#pragma once
#include "LinearMath.h"

struct DirectionalLight {
	Vector3 direction;
	Color32 color;
	float intensity;
};