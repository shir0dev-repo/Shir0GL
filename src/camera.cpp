#include <math.h>

#include "mathutil.h"
#include "transform.h"
#include "camera.h"

namespace sogl {
	camera::camera() {
		m_transform = transform();
		m_fov = 60.0f;
		m_nearPlane = 0.03f;
		m_farPlane = 1000.0f;

		m_pixelWidth = 800;
		m_pixelHeight = 800;
		m_aspectRatio = 1;

		m_projectionMatrix = generateProjectionMatrix();
		m_inverseProjectionMatrix = m_projectionMatrix.inverted();
	}

	camera::camera(const float& fov, const float& nearPlane, const float& farPlane,
		const unsigned& pixelWidth, const unsigned& pixelHeight) {
		m_transform = transform();
		
		m_fov = fov;
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
		
		m_pixelWidth = pixelWidth;
		m_pixelHeight = pixelHeight;
		m_aspectRatio = m_pixelWidth / m_pixelHeight;
	}

	camera::camera(const transform& t) : camera() {
		m_transform = t;
	}

	camera::camera(const transform& t, const float& fov, const float& nearPlane, const float& farPlane,
		const unsigned& pixelWidth, const unsigned& pixelHeight) {
		m_transform = transform(t);

		m_fov = fov;
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;

		m_pixelWidth = pixelWidth;
		m_pixelHeight = pixelHeight;
		m_aspectRatio = m_pixelWidth / m_pixelHeight;
	}

	matrix4f camera::generateProjectionMatrix() const {
		matrix4f proj;
		float tanFOV = tan(DEG2RAD * m_fov * 0.5f);
		float farMinusNear = m_farPlane - m_nearPlane;
		float farPlusNear = m_farPlane + m_nearPlane;

		proj(0, 0) = 1.0f / m_aspectRatio * tanFOV;
		proj(1, 1) = 1.0f / tanFOV;

		proj(2, 2) = -1.0f * farPlusNear / farMinusNear;
		proj(2, 3) = -1.0f;

		proj(3, 2) = 2 * m_farPlane * m_nearPlane / farMinusNear;
		proj(3, 3) = 0;

		return proj;
	}

	matrix4f const& camera::projectionMatrix() const {
		return m_projectionMatrix;
	}

	matrix4f const& camera::inverseProjectionMatrix() const {
		return m_inverseProjectionMatrix;
	}

	matrix4f const& camera::viewMatrix() const {
		return m_viewMatrix;
	}

	matrix4f const& camera::inverseViewMatrix() const {
		return m_transform.getTransformationMatrix();
	}
}