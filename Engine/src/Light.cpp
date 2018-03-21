#include "Light.h"
#include "LinearMath.h"

Matrix4 Light::getLightProjection(const Camera& camera, const Transform& transform) const
{
	// TODO: calculate correct bounding box using 
	//		some sort of ray caster to calculate distance
	return Math::ortho(-4.0, 4.0, -4.0, 4.0, 0.00001, 10.0);
}

Matrix4 Light::getLightView(const Camera& camera, const Transform& transform) const
{
	// TODO: Check if this kind of light view and position is correct
	//			to the bounding box that should be generated
	return Math::lookAt(-direction, direction, Vector3(0, 1, 0));
}