#pragma once

namespace sogl {
	typedef struct cameraData {
		float fov;
		float nearPlane;
		float farPlane;

		float aspectRatio;
		unsigned int screenWidth;
		unsigned int screenHeight;

		inline cameraData() : cameraData(60.0f, 0.03f, 1000.0f, 800, 800) { }

		inline cameraData(const float& fov, const float& nearPlane, const float& farPlane,
			const unsigned int& screenWidth, const unsigned int& screenHeight) {
			this->fov = fov;
			this->nearPlane = nearPlane;
			this->farPlane = farPlane;

			this->screenWidth = screenWidth;
			this->screenHeight = screenHeight;
			this->aspectRatio = screenWidth / screenHeight;
		}
	} cameraData;
}