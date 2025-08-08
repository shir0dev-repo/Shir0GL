#pragma once

#include <stdint.h>

namespace sogl {
	enum class FaceDirection {
		down = 0,
		up = 1,
		left = 2,
		right = 3,
		forward = 4,
		back = 5
	};

	static bool ReverseOrder(const FaceDirection dir) {
		switch (dir) {
			case FaceDirection::up:
			case FaceDirection::right:
			case FaceDirection::forward:
				return true;
			default:
				return false;
		}
	}
	
	uint32_t NormalIndex(const FaceDirection dir) {
		switch (dir) {
			case FaceDirection::left: 
				return 0;
			case FaceDirection::right:
				return 1;
			case FaceDirection::down:
				return 2;
			case FaceDirection::up:
				return 3;
			case FaceDirection::forward:
				return 4;
			case FaceDirection::back:
				return 5;
		}
	}
}