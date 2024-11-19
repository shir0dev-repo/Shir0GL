#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <algorithm>
#include <iostream>

#include <sogl/rendering/camera.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/mathutil.hpp>

namespace sogl {
	void defaultCameraUpdateFunc(camera* camera, const vec3f& translation, const vec3f& rotation, const float& aspect) {
		bool shouldRegenerateView = false;
		
		// rotation needs to be applied
		if (rotation.lengthSquared() > 0.01f) {
			// flag view matrix regeneration
			shouldRegenerateView = true;

			// set target rotation values
			camera->yaw += rotation.y;
			camera->pitch += rotation.x;
			camera->roll += rotation.z;

			// clamp rotation values
			clampAngle(&camera->yaw);
			clampAngle(&camera->pitch);
			clampAngle(&camera->roll);
		}

		// translation needs to be applied
		if (translation.lengthSquared() > 0.01f) {
			shouldRegenerateView = true;

			camera->position += translation;
		}

		// aspect ratio changed, regenerate projection matrix
		if (camera->aspectRatio != aspect) {
			camera->aspectRatio = aspect;
			camera->regenerateProjectionMatrix();
		}

		// regenerate view matrix if needed
		if (shouldRegenerateView) {
			camera->regenerateViewMatrix();
		}
	}

	camera::camera() {
		screenWidth = windowWidth();
		screenHeight = windowHeight();
		aspectRatio = sogl::aspectRatio();

		fov = 60;
		nearPlane = 0.03f;
		farPlane = 100.0f;

		init();
	}

	void camera::init() {
		regenerateProjectionMatrix();
		viewMatrix = matrix4f();
		updateFunc = defaultCameraUpdateFunc;
	}

	void camera::update(const vec3f& translation, const vec3f& rotation, const float& aspect) {
		updateFunc(this, translation, rotation, aspect);
	}

	void camera::setPosition(const vec3f& position) {
		this->position = position;
		regenerateViewMatrix();
	}

	void camera::setRotation(const vec3f& eulers) {
		this->yaw = eulers.y;
		this->pitch = eulers.x;
		this->roll = eulers.z;
		
		clampAngle(&yaw);
		clampAngle(&pitch);
		clampAngle(&roll);

		regenerateViewMatrix();
	}

	void camera::regenerateProjectionMatrix() {
		projectionMatrix = matrix4f();

		float scale = 1.0f / tan(fov * 0.5f * DEG2RAD);
		projectionMatrix(0, 0) = (1.0f / aspectRatio) * scale;
		projectionMatrix(1, 1) = scale;
		projectionMatrix(2, 2) = -((farPlane + nearPlane) / (farPlane - nearPlane));
		projectionMatrix(2, 3) = -((2.0f * farPlane * nearPlane) / (farPlane - nearPlane));
		projectionMatrix(3, 2) = -1;
		projectionMatrix(3, 3) = 0;

		// to this day i have no clue why i need to transpose it to work, but oh well
		projectionMatrix.transpose();
	}

	matrix4f camera::inverseProjectionMatrix() const {
		return projectionMatrix.inverted();
	}

	void camera::regenerateViewMatrix() {
		viewMatrix = matrix4f();

		// apply rotation first
		quat yRotation(yaw, vec3f::UP);
		quat xRotation(pitch, vec3f::RIGHT);
		quat zRotation(roll, vec3f::FORWARD);

		// apply rotation in YXZ order
		viewMatrix.rotate(zRotation * xRotation * yRotation);

		// apply translation after rotation
		viewMatrix.translate(position);

		// invert to get target view matrix
		viewMatrix.invert();
	}

	matrix4f camera::inverseViewMatrix() const {
		matrix4f invView;
		quat yRotation(yaw, vec3f::UP);
		quat xRotation(pitch, vec3f::RIGHT);
		quat zRotation(roll, vec3f::FORWARD);

		// apply rotation first
		invView.rotate(zRotation * xRotation * yRotation);
		// apply translation after rotation
		invView.translate(position);

		// technically, an inverted view matrix is a world transformation matrix, so no inversion needed here
		return invView;
	}
}