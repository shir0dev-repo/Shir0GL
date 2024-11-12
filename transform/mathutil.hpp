#pragma once

#include <algorithm>

namespace sogl {
	const float PI = 3.14159f;
	const float DEG2RAD = 0.01745f;
	const float RAD2DEG = 57.296f;

	template <typename T> 
	inline T clamp(T in, T min, T max) {
		return std::max(min, std::min(in, max));
	}
}