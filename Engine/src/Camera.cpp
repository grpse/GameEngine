#include "Camera.h"
#include "Transform.h"

Camera::Camera()
{
}

Camera::Camera(const Camera& other)
{
	memcpy(this, &other, sizeof(Camera));
}

Camera::~Camera()
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
		#define ENGINE_USING_OPENGL_RIGHT_HANDED_PERPECTIVE_PROJECTION
		//TODO: If some day use DirectX, should make some wrapper to LEFT_HANDED
		float const tanHalfFovy = Math::tan(format.fieldOfView / 2.0f);

		mProjectionMatrix = Matrix4(0);
		mProjectionMatrix[0][0] = 1.0f / (format.aspectRatio * tanHalfFovy);
		mProjectionMatrix[1][1] = 1.0f / (tanHalfFovy);
		mProjectionMatrix[2][3] = -1.0f;

		mProjectionMatrix[2][2] = -(format.farPlane + format.nearPlane) / (format.farPlane - format.nearPlane);
		mProjectionMatrix[3][2] = -(2.0f * format.farPlane * format.nearPlane) / (format.farPlane - format.nearPlane);
	}
}

const Matrix4& Camera::getViewMatrix() const
{
	if (transform.isDirty()) {
		const Vector3 eyePosition = transform.getLocalPosition();
		const Vector3 targetToLookAt = transform.getFront();
		const Vector3 up = Vector3(0, 1, 0);

		#define ENGINE_USING_OPENGL_RIGHT_HANDED
		//TODO: If some day use DirectX, should make some wrapper to LEFT_HANDED
		
		Vector3 const f(normalize(targetToLookAt - eyePosition));
		Vector3 const s(normalize(cross(f, up)));
		Vector3 const u(cross(s, f));
		
		// s -> side (right in this case)
		// u -> up (+y)
		// f -> forward/front, target on this coordinate system (this is because target - eye)
		// d -> distance from origin. Because it is a right handed, -z is front and not -dot on dz

		// sx ux fx 0
		// sy uy fy 0
		// sz uz fz 0
		// dx dy dz 1

		mViewMatrix[0][0] = s.x;
		mViewMatrix[1][0] = s.y;
		mViewMatrix[2][0] = s.z;
		mViewMatrix[0][1] = u.x;
		mViewMatrix[1][1] = u.y;
		mViewMatrix[2][1] = u.z;
		mViewMatrix[0][2] = -f.x;
		mViewMatrix[1][2] = -f.y;
		mViewMatrix[2][2] = -f.z;
		mViewMatrix[3][0] = -dot(s, eyePosition);
		mViewMatrix[3][1] = -dot(u, eyePosition);
		mViewMatrix[3][2] =  dot(f, eyePosition);		
	}
	return mViewMatrix;
}

const Matrix4 & Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
}