#include <math.h>

#include <sogl/math/common>

namespace sogl {
#ifdef SOGL_EXPORT
	vec3f vec3f::ZERO = vec3f(0, 0, 0);
	vec3f vec3f::LEFT = vec3f(-1, 0 ,0);
	vec3f vec3f::RIGHT = vec3f(1, 0, 0);
	vec3f vec3f::DOWN = vec3f(0, -1, 0);
	vec3f vec3f::UP = vec3f(0, 1, 0);
	vec3f vec3f::BACK = vec3f(0, 0, -1);
	vec3f vec3f::FORWARD = vec3f(0, 0, 1);
	vec3f vec3f::ONE = vec3f(1, 1, 1);
#endif
	vec3f::vec3f(const float& x, const float& y, const float& z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vec3f::vec3f(const vec3f& v) {
		*this = v;
	}
	vec3f& vec3f::operator=(const vec3f& v) {
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	float vec3f::length() const {
		return sqrt(lengthSquared());
	}
	float vec3f::lengthSquared() const {
		return x * x + y * y + z * z;
	}

	vec3f vec3f::normalized() const {
		vec3f v(*this);
		v.normalize();
		return v;
	}
	void vec3f::normalize() {
		float inv_l = 1.0f / length();
		x *= inv_l;
		y *= inv_l;
		z *= inv_l;
	}

	float vec3f::dot(const vec3f& a, const vec3f& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	vec3f vec3f::cross(const vec3f& a, const vec3f& b) {
		vec3f c;
		c.x = (a.y * b.z) - (a.z * b.y);
		c.y = (a.z * b.x) - (a.x * b.z);
		c.z = (a.x * b.y) - (a.y * b.x);

		return c;
	}

	vec3f vec3f::operator+(const vec3f& v) const {
		vec3f v1(*this);
		v1.x += v.x;
		v1.y += v.y;
		v1.z += v.z;

		return v1;
	}
	vec3f& vec3f::operator+=(const vec3f& v) {
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3f vec3f::operator-(const vec3f& v) const {
		vec3f v1(*this);
		v1.x -= v.x;
		v1.y -= v.y;
		v1.z -= v.z;

		return v1;
	}
	vec3f& vec3f::operator-=(const vec3f& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3f vec3f::operator*(const matrix3f& mat) const {
		vec3f v(*this);
		return mat * v;
	}
	vec3f& vec3f::operator*=(const matrix3f& mat) {
		*this = mat * *this;
		return *this;
	}

	vec3f vec3f::operator*(const quat& q) const {
		vec3f v(*this);
		v = q.toMatrix3f() * v;

		return v;
	}
	vec3f& vec3f::operator*=(const quat& q) {
		*this = *this * q;

		return *this;
	}

	vec3f vec3f::operator*(const float& scalar) const {
		vec3f v(*this);

		v.x *= scalar;
		v.y *= scalar;
		v.z *= scalar;

		return v;
	}

	vec3f& vec3f::operator*=(const float& scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	bool vec3f::operator==(const vec3f& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
	bool vec3f::operator!=(const vec3f& other) const {
		return !(*this == other);
	}
}