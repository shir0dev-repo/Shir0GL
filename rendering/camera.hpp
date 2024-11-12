#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

#include "cameraData.hpp"

#include "../transform/matrix4f.hpp"
#include "../transform/transform.hpp"

namespace sogl {
	struct camera;

	typedef void (*CameraUpdateFunc)(camera*, const float&, const float&);
	
	// Default camera update behaviour.
	void defaultCameraUpdateFunc(camera*, const float&, const float&);

	struct SOGL_API camera {		
	private:
		matrix4f generateProjectionMatrix() const;
		matrix4f m_projectionMatrix;
		matrix4f m_inverseProjectionMatrix;
		matrix4f m_viewMatrix;
		
		CameraUpdateFunc m_updateFunc;

		void init();
	public:
		transform m_transform;

		cameraData m_cameraData;

		// Creates a default camera.
		camera();
		
		// Creates a camera with the given values.
		camera(const float& fov, const float& nearPlane, const float& farPlane,
			const unsigned& pixelWidth, const unsigned& pixelHeight);

		// Creates a camera with the given data.
		camera(const cameraData& data);

		// Creates a default camera with transform t.
		camera(const transform& t);

		// Creates a camera with a given transform, using the given values.
		camera(const transform& t, const float& fov, const float& nearPlane, const float& farPlane,
			const unsigned& pixelWidth, const unsigned& pixelHeight);

		// Creates a camera with the given transform, using the given camera data.
		camera(const transform& t, const cameraData& data);

		// Returns a reference to the camera's projection matrix.
		matrix4f const& projectionMatrix() const;

		// Returns a reference to the camera's inverse projection matrix.
		matrix4f const& inverseProjectionMatrix() const;

		// Returns a reference to the camera's view matrix.
		matrix4f const& viewMatrix() const;

		// Returns a reference to the camera's inverse view matrix.
		matrix4f const& inverseViewMatrix() const;

		// Updates the camera.
		void update(const float& deltaTime, const float& currentAspectRatio);
	};
}