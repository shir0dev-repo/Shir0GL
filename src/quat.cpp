#include <math.h>
#include <algorithm>

#include <sogl/math/common>


namespace sogl {
#ifdef SOGL_EXPORT
	quat quat::IDENTITY = quat();
#endif

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

	quat::quat(const float& xDegrees, const float& yDegrees, const float& zDegrees) {
		float cx = cos(DEG2RAD * xDegrees / 2.0f);
		float cy = cos(DEG2RAD * yDegrees / 2.0f);
		float cz = cos(DEG2RAD * zDegrees / 2.0f);

		float sx = sin(DEG2RAD * xDegrees / 2.0f);
		float sy = sin(DEG2RAD * yDegrees / 2.0f);
		float sz = sin(DEG2RAD * zDegrees / 2.0f);

		x = sx * cy * cz + cx * sy * sz;
		y = cx * sy * cz - sx * cy * sz;
		z = cx * cy * sz + sx * sy * cz;
		w = cx * cy * cz - sx * sy * sz;

		normalize();
	}

	quat::quat(const vec3f& v) : quat(v.x, v.y, v.z) { }

	quat::quat(const matrix3f& mat) {
		//https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
		// matrix on website is row-major, values must be flipped
		float t;

		if (mat(2, 2) < 0) {
			if (mat(0, 0) > mat(1, 1)) {
				t = 1 + mat(0, 0) - mat(1, 1) - mat(2, 2);

				x = t;
				y = mat(1, 0) + mat(0, 1);
				z = mat(0, 2) + mat(2, 0);
				w = mat(2, 1) - mat(1, 2);
			}
			else {
				t = 1 - mat(0, 0) + mat(1, 1) - mat(2, 2);

				x = mat(1, 0) + mat(0, 1);
				y = t;
				z = mat(2, 1) + mat(1, 2);
				w = mat(0, 2) - mat(2, 0);
			}
		}
		else {
			if (mat(0, 0) < -mat(1, 1)) {
				t = 1 - mat(0, 0) - mat(1, 1) + mat(2, 2);

				x = mat(0, 2) + mat(2, 0);
				y = mat(2, 1) + mat(1, 2);
				z = t;
				w = mat(1, 0) - mat(0, 1);
			}
			else {
				t = 1 + mat(0, 0) + mat(1, 1) + mat(2, 2);

				x = mat(2, 1) - mat(1, 2);
				y = mat(0, 2) - mat(2, 0);
				z = mat(1, 0) - mat(0, 1);
				w = t;
			}
		}

		*this *= 0.5f / sqrt(t);
		normalize();
	}

	quat& quat::operator=(const quat& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		this->w = rhs.w;

		return *this;
	}

	

	vec3f quat::toEulerAngles() const {
		vec3f eulers;

		eulers.z = RAD2DEG * atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * z * z + x * x);
		eulers.x = RAD2DEG * asin(2.0f * w * x - z * y);
		eulers.y = RAD2DEG * atan2(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y));

		return eulers;
	}

	float quat::lengthSquared() const {
		return x * x + y * y + z * z + w * w;
	}
	float quat::length() const {
		return sqrt(lengthSquared());
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
		
		q.normalize();

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

	matrix3f quat::rotationMatrix() const {
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
		m(0, 0) = 1.0f - 2.0f * (yy - zz);
		m(0, 1) = 2.0f * (xy + zw);
		m(0, 2) = 2.0f * (xz - yw);

		m(1, 0) = 2.0f * (xy - zw);
		m(1, 1) = 1.0f - 2.0f * (xx - zz);
		m(1, 2) = 2.0f * (yz + xw);

		m(2, 0) = 2.0f * (xz + yw);
		m(2, 1) = 2.0f * (yz - xw);
		m(2, 2) = 1.0f - 2.0f * (xx - yy);

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
		quat q1;
		
		q1.x = x * q.w + w * q.x + y * q.z - z * q.y;
		q1.y = y * q.w + w * q.y + z * q.x - x * q.z;
		q1.z = z * q.w + w * q.z + x * q.y - y * q.x;
		q1.w = w * q.w - x * q.x - y * q.y - z * q.z;

		return q1;
	}
	quat& quat::operator*=(const quat& q) {
		*this = *this * q;

		return *this;
	}

	vec3f quat::operator*(const vec3f& v) const {
		quat pure;
		pure.x = v.x;
		pure.y = v.y;
		pure.z = v.z;
		pure.w = 0;
		quat result = this->conjugate() * pure * *this;

		return vec3f(result.x, result.y, result.z);
	}

	quat quat::operator^(const float& exp) const {
		quat q(*this);
		q.x = pow(q.x, exp);
		q.y = pow(q.y, exp);
		q.z = pow(q.z, exp);
		q.w = pow(q.w, exp);

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