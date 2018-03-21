#include "Camera.h"
#include "Transform.h"

Camera::Camera()
{
	mTransform = new Transform;
}

Camera::~Camera()
{
	delete mTransform;
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
	if (getTransform().isDirty()) {
		const Vector3 position = getTransform().getLocalPosition();
		const Vector3 front = getTransform().getFront();
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

Transform& Camera::getTransform() const
{
	return *mTransform;
}