#include "../transform/vec3f.hpp"
#include "../transform/quat.hpp"
#include "../transform/matrix3f.hpp"
#include "../transform/matrix4f.hpp"

namespace sogl {
	vec3f vec3f::ZERO = vec3f(0, 0, 0);
	vec3f vec3f::LEFT = vec3f(-1, 0, 0);
	vec3f vec3f::RIGHT = vec3f(1, 0, 0);
	vec3f vec3f::DOWN = vec3f(0, -1, 0);
	vec3f vec3f::UP = vec3f(0, 1, 0);
	vec3f vec3f::BACK = vec3f(0, 0, -1);
	vec3f vec3f::FORWARD = vec3f(0, 0, 1);
	vec3f vec3f::ONE = vec3f(1, 1, 1);

	quat quat::IDENTITY = quat();

	matrix3f matrix3f::IDENTITY = matrix3f();

	matrix4f matrix4f::IDENTITY = matrix4f();
}