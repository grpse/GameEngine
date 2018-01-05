#include "Camera.h"
#include <iostream>

Matrix4 Camera::createViewMatrix() const
{
	const Vector3 position = transform.getLocalPosition();
	const Vector3 front = transform.getFront();
	const Vector3 target = position + front;

	return Math::lookAt(
		position,
		target,
		Vector3(0, 1, 0)
	);
}


void Camera::printPositionAndTarget()
{
	const Vector3 position = transform.getLocalPosition();
	const Vector3 front = transform.getFront();
	const Vector3 target = position + front;

	std::cout << "Position: " << Math::to_string(position) << std::endl;
	std::cout << "Target  : " << Math::to_string(target) << std::endl;
}