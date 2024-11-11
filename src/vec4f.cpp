#include <sogl/matrix>

namespace sogl {
	vec4f::vec4f(const float& x, const float& y, const float& z, const float& w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	vec4f::vec4f(const vec3f& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = 1.0f;
	}

	vec4f::vec4f(const vec4f& v) {
		*this = v;
	}

	vec4f& vec4f::operator=(const vec4f& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	float vec4f::dot(const vec4f& a, const vec4f& b) {
		return
			a.x * b.x +
			a.y * b.y +
			a.z * b.z +
			a.w * b.w;
	}

	vec4f vec4f::operator+(const vec4f& v) const {
		vec4f v1(*this);
		v1.x += v.x;
		v1.y += v.y;
		v1.z += v.z;
		v1.w += v.w;

		return v1;
	}

	vec4f& vec4f::operator+=(const vec4f& v) {
		*this = *this + v;

		return *this;
	}

	vec4f vec4f::operator-(const vec4f& v) const {
		vec4f v1(*this);
		v1.x -= v.x;
		v1.y -= v.y;
		v1.z -= v.z;
		v1.w -= v.w;

		return v1;
	}

	vec4f& vec4f::operator-=(const vec4f& v) {
		*this = *this - v;

		return *this;
	}


	bool vec4f::operator==(const vec4f& other) const {
		return
			x == other.x &&
			y == other.y &&
			z == other.z &&
			w == other.w;
	}

	bool vec4f::operator!=(const vec4f& other) const {
		return !(*this == other);
	}

	vec4f::operator vec3f() const {
		return vec3f(x, y, z);
	}
}