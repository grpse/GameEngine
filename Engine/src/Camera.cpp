#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}

const Matrix4 Camera::getViewMatrix() const
{
	if (transform.isDirty()) {
		const Vector3 position = transform.getLocalPosition();
		const Vector3 front = transform.getFront();
		const Vector3 target = position + front;

		mViewMatrix = Math::lookAt(
			position,
			target,
			Vector3(0, 1, 0)
		);
	}
	return mViewMatrix;
}