#pragma once
#include "LinearMath.h"

class Camera;
class Transform;

struct Light {
	enum class Type {
		Directional, Area, Point, Spot
	};

	Light::Type type;

	float intensity;
	Color32 color;
	Vector3 position;
	Vector3 direction;

	Matrix4 getLightProjection(const Camera& camera, const Transform& transform) const;
	Matrix4 getLightView(const Camera& camera, const Transform& transform) const;
};