#pragma once

#include <algorithm>
#include <sogl/transform/vec3f.hpp>
namespace sogl {
	const float PI = 3.14159f;
	const float DEG2RAD = 0.01745f;
	const float RAD2DEG = 57.296f;

	template <typename T> 
	inline T clamp(T in, T min, T max) {
		return std::max(min, std::min(in, max));
	}

	inline void clampAngle(float* angle, const bool& radians = false) {
		if (!radians) {
			if (*angle >= 180)
				*angle -= 360;
			else if (*angle <= -180)
				*angle += 360;
		}
		else {
			float twoPI = 2 * PI;

			if (*angle >= twoPI)
				*angle -= twoPI;
			else if (*angle <= 0)
				*angle += twoPI;
		}
	}

	inline void clampEulers(vec3f* eulers, const bool& radians = false) {
		if (!radians) {
			clampAngle(&eulers->y);
			clampAngle(&eulers->x);
			clampAngle(&eulers->z);
		}
		else {
			clampAngle(&eulers->y, true);
			clampAngle(&eulers->x, true);
			clampAngle(&eulers->z, true);
		}
	}
}