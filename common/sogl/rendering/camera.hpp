#pragma once

#include <sogl/transform/matrix4f.hpp>
#include <sogl/transform/vec3f.hpp>

namespace sogl {
	struct camera;
	struct uniformBufferObject;

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

		// The Y-rotation of the camera in world space
		float yaw = 0;
		// The X-rotation of the camera in world space
		float pitch = 0;
		// The Z-rotation of the camera in world space
		float roll = 0;

		matrix4f projectionMatrix = matrix4f::IDENTITY;
		matrix4f viewMatrix = matrix4f::IDENTITY;
	
	private:
		CameraUpdateFunc updateFunc = defaultCameraUpdateFunc;
		uniformBufferObject* cameraBufferBlock;
	public:
		camera();

		void init();
		void update(const vec3f& translation, const vec3f& rotation, const float& aspect);

		void setPosition(const vec3f& position);
		void setRotation(const vec3f& eulerRotation);
		quat getRotation() const;
		void regenerateProjectionMatrix();
		matrix4f inverseProjectionMatrix() const;
		
		void regenerateViewMatrix();
		void regenerateViewMatrix(const quat& rotation, const vec3f& position);
		matrix4f inverseViewMatrix() const;

		friend std::ostream& operator<<(std::ostream& os, const camera& camera);
	} camera;
}