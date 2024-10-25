#include "pch.h"
#include "quat.h"
#include "vec3f.h"
#include "Matrix3f.h"

namespace shir0GL {
	quat::quat() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 1;
	}
	quat::quat(const quat& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}
	quat& quat::operator=(const quat& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		this->w = rhs.w;
	}

	quat::quat(const float& xDegrees, const float& yDegrees, const float& zDegrees) {
		float cx = cosf(deg2rad * xDegrees / 2.0f);
		float cy = cosf(deg2rad * yDegrees / 2.0f);
		float cz = cosf(deg2rad * zDegrees / 2.0f);

		float sx = sinf(deg2rad * xDegrees / 2.0f);
		float sy = sinf(deg2rad * yDegrees / 2.0f);
		float sz = sinf(deg2rad * zDegrees / 2.0f);

		x = sx * cy * cz + cx * sy * sz;
		y = cx * sy * cz - sx * cy * sz;
		z = cx * cy * sz + sx * sy * cz;
		w = cx * cy * cz - sx * sy * sz;

		normalize();
	}

	quat::quat(const vec3f& v) : quat(v.x, v.y, v.z) { }

	vec3f quat::toEulerAngles() const {
		vec3f eulers;

		eulers.z = rad2deg * atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * z * z + x * x);
		eulers.x = rad2deg * asinf(2.0f * w * x - z * y);
		eulers.y = rad2deg * atan2f(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y));

		return eulers;
	}

	float quat::lengthSquared() const {
		return x * x + y * y + z * z + w * w;
	}
	float quat::length() const {
		return sqrtf(lengthSquared());
	}

	void quat::normalize() {
		float l = 1.0f / length();
		x *= l;
		y *= l;
		z *= l;
		w *= l;
	}
	quat quat::normalized() const {
		quat q(*this);
		q.normalize();

		return q;
	}

	quat quat::conjugate() const {
		quat q(*this);
		q.x *= -1;
		q.y *= -1;
		q.z *= -1;

		return q;
	}
	quat quat::inverse() const {
		quat q = conjugate();
		float inv_lSqr = 1.0f / lengthSquared();
		q.x *= inv_lSqr;
		q.y *= inv_lSqr;
		q.z *= inv_lSqr;
		q.w *= inv_lSqr;

		return q;
	}

	matrix3f quat::toMatrix3f() const {
		float xx = x * x;
		float xy = x * y;
		float xz = x * z;
		float xw = x * w;
		float yy = y * y;
		float yz = y * z;
		float yw = y * w;
		float zz = z * z;
		float zw = z * w;

		matrix3f m;
		m(0, 0) = 1.0f - 2.0f * (yy + zz);
		m(0, 1) = 2.0f * (xy - zw);
		m(0, 2) = 2.0f * (xz + yw);

		m(1, 0) = 2.0f * (xy + zw);
		m(1, 1) = 1.0f - 2.0f * (xx + zz);
		m(1, 2) = 2.0f * (yz - xw);

		m(2, 0) = 2.0f * (xz - yw);
		m(2, 1) = 2.0f * (yz + xw);
		m(2, 2) = 1.0f - 2.0f * (xx + yy);

		return m;
	}

	float quat::dot(const quat& a, const quat& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	quat quat::lerp(const quat& a, const quat& b, const float& t) {
		if (0 > t || t > 1) {
			float l = std::max(0.0f, std::min(t, 1.0f));
			return a * (1.0f - l) + b * l;
		}

		return a * (1.0f - t) + b * t;
	}
	quat quat::slerp(const quat& a, const quat& b, const float& t) {
		if (0 > t || t > 1) {
			float l = std::max(0.0f, std::min(t, 1.0f));
			return a * (a.conjugate() * b) ^ l;
		}

		return a * (a.conjugate() * b) ^ t;
	}

	quat quat::euler(const vec3f& v) {
		return quat(v);
	}

	quat quat::euler(const float& xDegrees, const float& yDegrees, const float& zDegrees) {
		return quat(xDegrees, yDegrees, zDegrees);
	}

	quat quat::axisAngle(const vec3f& axis, const float& angle) {
		quat q;

		float rad = deg2rad * angle;
		q.x = axis.x * sinf(rad / 2.0f);
		q.y = axis.y * sinf(rad / 2.0f);
		q.z = axis.z * sinf(rad / 2.0f);
		q.w = cosf(rad / 2.0f);

		return q;
	}

	quat quat::fromToRotation(const vec3f& a, const vec3f& b) {
		float aDotB = vec3f::dot(a, b);
		vec3f axis;

		if (aDotB < -0.9999f) {
			axis = vec3f::cross(a, vec3f::RIGHT);
			if (axis.length() < 0.0001f)
				axis = vec3f::cross(a, vec3f::UP).normalized();


			return axisAngle(axis, 180.0f);
		}

		axis = vec3f::cross(a, b).normalized();
		float angle = rad2deg * acosf(aDotB);
		return axisAngle(axis, angle);
	}

	quat quat::operator+(const quat& q) const {
		quat q1(*this);
		q1.x += q.x;
		q1.y += q.y;
		q1.z += q.z;
		q1.w += q.w;

		return q1;
	}
	quat& quat::operator+=(const quat& q) {
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	quat quat::operator-(const quat& q) const {
		quat q1(*this);
		q1.x -= q.x;
		q1.y -= q.y;
		q1.z -= q.z;
		q1.w -= q.w;

		return q1;
	}
	quat& quat::operator-=(const quat& q) {
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}

	quat quat::operator*(const float& val) const {
		quat q(*this);
		q.x *= val;
		q.y *= val;
		q.z *= val;
		q.w *= val;

		return q;
	}
	quat& quat::operator*=(const float& val) {
		*this = (*this) * val;
		return *this;
	}

	quat quat::operator*(const quat& q) const {
		quat q1(*this);
		q1.x *= q.x;
		q1.y *= q.y;
		q1.z *= q.z;
		q1.w *= q.w;

		return q1;
	}
	quat& quat::operator*=(const quat& q) {
		x *= q.x;
		y *= q.y;
		z *= q.z;
		w *= q.w;

		return *this;
	}

	quat quat::operator^(const float& exp) const {
		quat q(*this);
		q.x = powf(q.x, exp);
		q.y = powf(q.y, exp);
		q.z = powf(q.z, exp);
		q.w = powf(q.w, exp);

		return q;
	}
	quat& quat::operator^=(const float& exp) {
		*this = (*this) ^ exp;
		return *this;
	}

	bool quat::operator==(const quat& other) const {
		if (x == other.x && y == other.y && z == other.z && w == other.w) {
			return true;
		}
		else if (-x == other.x && -y == other.y && -z == other.z && -w == other.w) {
			return true;
		}
		else if (x == -other.x && y == -other.y && z == -other.z && w == -other.w) {
			return true;
		}

		return false;
	}
	bool quat::operator!=(const quat& other) const {
		return !(*this == other);
	}
}