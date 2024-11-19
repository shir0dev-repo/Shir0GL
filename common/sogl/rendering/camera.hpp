#pragma once

#include "cameraData.hpp"

#include <sogl/transform/matrix4f.hpp>
#include <sogl/transform/transform.hpp>

namespace sogl {
	struct camera;

	typedef void (*CameraUpdateFunc)(camera* instance, const vec3f& translation, const vec3f& rotation, const float& aspect);
	static void defaultCameraUpdateFunc(camera* instance, const vec3f& translation, const vec3f& rotation, const float& aspect);

	typedef struct camera {
		int screenWidth = 0;
		int screenHeight = 0;
		float aspectRatio = 0;
		float fov = 0;
		float nearPlane = 0;
		float farPlane = 0;

		vec3f position = vec3f::ZERO;

		float yaw = 0;
		float pitch = 0;
		float roll = 0;

		matrix4f projectionMatrix = matrix4f::IDENTITY;
		matrix4f viewMatrix = matrix4f::IDENTITY;
	
	private:
		CameraUpdateFunc updateFunc = defaultCameraUpdateFunc;
	public:
		camera();

		void init();
		void update(const vec3f& translation, const vec3f& rotation, const float& aspect);

		void setPosition(const vec3f& position);
		void setRotation(const vec3f& eulerRotation);

		void regenerateProjectionMatrix();
		matrix4f inverseProjectionMatrix() const;
		
		void regenerateViewMatrix();
		matrix4f inverseViewMatrix() const;

	} camera;
}