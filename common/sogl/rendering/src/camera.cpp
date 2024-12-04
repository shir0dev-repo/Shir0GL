#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include <sogl/rendering/camera.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/mathutil.hpp>
#include <sogl/rendering/factories/uniformBufferFactory.hpp>

namespace sogl {
	static const char* CameraBuffer = "Matrices";

	void defaultCameraUpdateFunc(camera* camera, const vec3f& translation, const vec3f& rotation, const float& aspect) {
		static vec3f inputCorrection(1, 1, -1);
		// set target rotation values
		camera->yaw += rotation.y;
		camera->pitch += rotation.x;
		camera->roll += rotation.z;

		// clamp x to avoid camera flipping
		clampAngle(&camera->pitch, -89.9f, 89.9f);
		// clamp y to wrap between -180/180
		clampAngle(&camera->yaw);
		// clamp Z (for continuity)
		clampAngle(&camera->roll);

		
		quat newRotation = camera->getRotation().conjugate().normalized();
		
		camera->position += (newRotation * (inputCorrection * translation)) * getDeltaTime();

		// aspect ratio changed, regenerate projection matrix
		if (camera->aspectRatio != aspect) {
			camera->aspectRatio = aspect;
			camera->screenWidth = getWindowWidth();
			camera->screenHeight = getWindowHeight();
			camera->regenerateProjectionMatrix();
		}

		// regenerate view matrix if needed

		camera->regenerateViewMatrix();
	}

	camera::camera() : cameraBufferBlock(nullptr) {}

	void camera::init() {
		cameraBufferBlock = uniformBufferFactory::createNew(CameraBuffer, 2 * sizeof(matrix4f));
		regenerateProjectionMatrix();
		regenerateViewMatrix();
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

		regenerateViewMatrix(getRotation(), vec3f::ZERO);
	}

	quat camera::getRotation() const {
		quat yRot(yaw, vec3f::UP);
		quat xRot(pitch, yRot.conjugate() * vec3f::RIGHT);
		quat zRot(roll, xRot.conjugate() * vec3f::FORWARD);
		quat rotation = /*zRot * */xRot * yRot;
		rotation.normalize();
		return rotation;
	}

	void camera::regenerateProjectionMatrix() {
		projectionMatrix = matrix4f::IDENTITY;

		float scale = 1.0f / tan(fov * 0.5f * DEG2RAD);
		projectionMatrix(0, 0) = (1.0f / aspectRatio) * scale;
		projectionMatrix(1, 1) = scale;
		projectionMatrix(2, 2) = -((farPlane + nearPlane) / (farPlane - nearPlane));
		projectionMatrix(2, 3) = -((2.0f * farPlane * nearPlane) / (farPlane - nearPlane));
		projectionMatrix(3, 2) = -1;
		projectionMatrix(3, 3) = 0;

		// to this day i have no clue why i need to transpose it to work, but oh well
		projectionMatrix.transpose();
		cameraBufferBlock->bufferData(projectionMatrix.getPointer(), sizeof(matrix4f), sizeof(matrix4f));
	}

	void camera::regenerateViewMatrix() {
		viewMatrix = matrix4f::IDENTITY;
		// apply rotation first (YXZ order)
		viewMatrix.setRotation(getRotation());
		// apply translation after rotation
		viewMatrix.setPosition(position);
		// invert to get target view matrix		
		viewMatrix.invert();

		cameraBufferBlock->bufferData(viewMatrix.getPointer(), sizeof(matrix4f), 0);
	}

	void camera::regenerateViewMatrix(const quat& rotation, const vec3f& position) {		
		viewMatrix = matrix4f::IDENTITY;
		// apply rotation first (YXZ order)
		viewMatrix.setRotation(rotation);
		// apply translation after rotation
		viewMatrix.setPosition(position);
		// invert to get target view matrix		
		viewMatrix.invert();

		cameraBufferBlock->bufferData(viewMatrix.getPointer(), sizeof(matrix4f), 0);
	}

	matrix4f camera::inverseProjectionMatrix() const {
		return projectionMatrix.inverted();
	}

	matrix4f camera::inverseViewMatrix() const {
		matrix4f invView;
		quat yRotation(yaw, vec3f::UP);
		quat xRotation(pitch, vec3f::RIGHT);
		quat zRotation(roll, vec3f::FORWARD);

		// apply rotation first
		invView.setRotation(zRotation * xRotation * yRotation);
		// apply translation after rotation
		invView.setPosition(position);

		// technically, an inverted view matrix is a world transformation matrix, so no inversion needed here
		return invView;
	}

	std::ostream& operator<<(std::ostream& os, const camera& cam) {
		os << "[CAMERA]\n";
		quat y(cam.yaw, vec3f::UP);
		quat x(cam.pitch, vec3f::RIGHT);
		quat z(cam.roll, vec3f::FORWARD);

		matrix4f m;
		m.setRotation(z * x * y);
		m.setPosition(cam.position);

		os << "|--TRANSFORM:" << m;
		os << "|-- POS: " << cam.position << '\n';
		quat q = m.getRotation();
		os << "|-- ROT: " << q.x << q.y << q.z << q.w << '\n';
		os << "|-- EUL: " << cam.pitch << " " << cam.yaw << " " << cam.roll << '\n';
		os << "  |-- RHT: " << (vec3f)cam.viewMatrix.getColumn(0) << '\n';
		os << "  |--  UP: " << (vec3f)cam.viewMatrix.getColumn(1) << '\n';
		os << "  |-- FWD: " << (vec3f)cam.viewMatrix.getColumn(2) << '\n';
		os << "|-- VIEW: " << cam.viewMatrix << '\n';
		os << "|-- PROJ: " << cam.projectionMatrix << '\n';
		os << std::endl;

		return os;
	}
}