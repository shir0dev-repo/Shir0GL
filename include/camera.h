#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

#include "transform.h"
#include "matrix4f.h"

namespace sogl {
	struct SOGL_API camera {
	private:
		matrix4f m_projectionMatrix;
		matrix4f m_inverseProjectionMatrix;
		matrix4f generateProjectionMatrix() const;

		matrix4f m_viewMatrix;
	public:
		transform m_transform;

		float m_fov;
		float m_nearPlane;
		float m_farPlane;

		unsigned int m_pixelWidth;
		unsigned int m_pixelHeight;
		float m_aspectRatio;

		// Creates a default camera.
		camera();
		
		// Creates a camera with the given values.
		camera(const float& fov, const float& nearPlane, const float& farPlane,
			const unsigned& pixelWidth, const unsigned& pixelHeight);

		// Creates a default camera with transform t.
		camera(const transform& t);

		// Creates a camera with a given transform, using the given values.
		camera(const transform& t, const float& fov, const float& nearPlane, const float& farPlane,
			const unsigned& pixelWidth, const unsigned& pixelHeight);

		matrix4f const& projectionMatrix() const;
		matrix4f const& inverseProjectionMatrix() const;

		matrix4f const& viewMatrix() const;
		matrix4f const& inverseViewMatrix() const;
	};
}