#include <math.h>

#include "../rendering/camera.hpp"
#include "../transform/mathutil.hpp"

namespace sogl {
	void defaultCameraUpdateFunc(camera* camera, const float& deltaTime, const float& aspectRatio) {
		camera->m_transform.translate(vec3f::RIGHT * deltaTime);
	}

	camera::camera() {
		m_transform = transform();
		m_cameraData = cameraData();

		init();
	}

	camera::camera(const float& fov, const float& nearPlane, const float& farPlane,
		const unsigned& pixelWidth, const unsigned& pixelHeight) {
		m_transform = transform();
		m_cameraData = cameraData(fov, nearPlane, farPlane, pixelWidth, pixelHeight);
		
		init();
	}

	camera::camera(const cameraData& data) {
		m_transform = transform();
		m_cameraData = data;

		init();
	}

	camera::camera(const transform& t) {
		m_transform = transform(t);
		m_cameraData = cameraData();

		init();
	}

	camera::camera(const transform& t, const float& fov, const float& nearPlane, const float& farPlane,
		const unsigned& pixelWidth, const unsigned& pixelHeight) {
		m_transform = transform(t);
		m_cameraData = cameraData(fov, nearPlane, farPlane, pixelWidth, pixelHeight);

		init();
	}

	camera::camera(const transform& t, const cameraData& data) {
		m_transform = transform(t);
		m_cameraData = data;

		init();
	}

	void camera::init() {
		m_projectionMatrix = generateProjectionMatrix();
		m_inverseProjectionMatrix = m_projectionMatrix.inverted();
		m_viewMatrix = matrix4f();
		m_updateFunc = defaultCameraUpdateFunc;
	}

	matrix4f camera::generateProjectionMatrix() const {
		matrix4f proj;
		float np = m_cameraData.nearPlane;
		float fp = m_cameraData.farPlane;

		float tanFOV = tan(DEG2RAD * m_cameraData.fov * 0.5f);
		float deltaFN = fp - np;

		proj(0, 0) = 1.0f / m_cameraData.aspectRatio * tanFOV;
		proj(1, 1) = 1.0f / tanFOV;

		proj(2, 2) = -1.0f * (fp + np) / deltaFN;
		proj(2, 3) = -1.0f;

		proj(3, 2) = 2 * fp * np / deltaFN;
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

	void camera::update(const float& deltaTime, const float& aspectRatio) {
		m_updateFunc(this, deltaTime, aspectRatio);

		// aspect ratio changed, recalculate projection
		if (aspectRatio != m_cameraData.aspectRatio) {
			m_projectionMatrix = generateProjectionMatrix();
			m_inverseProjectionMatrix = m_projectionMatrix.inverted();
		}
	}
}