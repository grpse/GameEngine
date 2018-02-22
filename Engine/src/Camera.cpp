#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}

void Camera::setFormat(const Camera::Format& format, const Camera::Type& type)
{
	if (type == Camera::Type::Orthographic) {
		mProjectionMatrix = Math::ortho(
			-format.viewport.width / 2,
			+format.viewport.width / 2,
			+format.viewport.height / 2,
			-format.viewport.height / 2,
			format.nearPlane,
			format.farPlane
		);
	}
	else {
		mProjectionMatrix = Math::perspective(
			format.fieldOfView,
			format.aspectRatio,
			format.nearPlane,
			format.farPlane
		);
	}
}

const Matrix4& Camera::getViewMatrix() const
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

const Matrix4 & Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
}
