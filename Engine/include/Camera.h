#pragma once
#include "LinearMath.h"
#include "Rect.h"
#include "Transform.h"



class Camera {
public:

	enum class Type {
		Orthographic,
		Perspective
	};

	struct Format {
		float aspectRatio;
		float nearPlane;
		float farPlane;
		float fieldOfView;
		Rect viewport;
	};

	Camera();
	Transform transform;
	void setFormat(const Camera::Format& format, const Camera::Type& type);
	const Matrix4& getViewMatrix() const;
	const Matrix4& getProjectionMatrix() const;

private:
	mutable Matrix4 mViewMatrix;
	mutable Matrix4 mProjectionMatrix;
};