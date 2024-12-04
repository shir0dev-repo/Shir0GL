#pragma once

#include <algorithm>
#include <math.h>
#include <sogl/transform/matrix3f.hpp>
#include <sogl/mathutil.hpp>
#include <sogl/transform/vec3f.hpp>

namespace sogl {
	// 4D vector that handles rotations.
	struct quatd {
		static quatd IDENTITY;

		double x;
		double y;
		double z;
		double w;

		// Identity quatdernion (no rotation).
		inline quatd() {
			x = 0;
			y = 0;
			z = 0;
			w = 1;
		}

		// Constructs a quatdernion from the euler angles xDegrees, yDegrees, and zDegrees.
		inline quatd(const double& xDegrees, const double& yDegrees, const double& zDegrees) {
			double cx = cos(DEG2RAD * xDegrees / 2.0f);
			double cy = cos(DEG2RAD * yDegrees / 2.0f);
			double cz = cos(DEG2RAD * zDegrees / 2.0f);

			double sx = sin(DEG2RAD * xDegrees / 2.0f);
			double sy = sin(DEG2RAD * yDegrees / 2.0f);
			double sz = sin(DEG2RAD * zDegrees / 2.0f);

			x = sx * cy * cz + cx * sy * sz;
			y = cx * sy * cz - sx * cy * sz;
			z = cx * cy * sz + sx * sy * cz;
			w = cx * cy * cz - sx * sy * sz;

			normalize();
		}

		// Constructs a quatdernion from the euler angles v.
		inline quatd(const vec3f& eulers) : quatd(eulers.x, eulers.y, eulers.z) { }

		// Constructs a quatdernion that rotates angle degrees around axis.
		inline quatd(const double& angle, const vec3f& axis) {
			double rad = DEG2RAD * angle;
			x = axis.x * sin(rad / 2.0f);
			y = axis.y * sin(rad / 2.0f);
			z = axis.z * sin(rad / 2.0f);
			w = cos(rad / 2.0f);

			normalize();
		}

		// Constructs a quatdernion from the given rotation matrix.
		inline quatd(const matrix3f& mat) {
			//https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quatd.pdf
		// matrix on website is row-major, values must be flipped
			double t;

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

		// Copies values of quatdernion q.
		inline quatd(const quatd& q) {
			*this = q;
			normalize();
		}

		// Assigns this quatdernion's components from the components of q.
		inline quatd& operator=(const quatd& q) {
			this->x = q.x;
			this->y = q.y;
			this->z = q.z;
			this->w = q.w;

			normalize();
			return *this;
		}

		inline vec3f toEulerAngles() const {
			vec3f eulers;

			eulers.z = RAD2DEG * atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * z * z + x * x);
			eulers.x = RAD2DEG * asinf(2.0f * w * x - z * y);
			eulers.y = RAD2DEG * atan2f(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y));

			return eulers;
		}

		// Returns the length of this quatdernion.
		inline double length() const {
			return sqrtf(x * x + y * y + z * z + w * w);
		}

		// Returns the squared length of this quatdernion.
		inline double lengthSquared() const {
			return x * x + y * y + z * z + w * w;
		}

		// Normalizes this quatdernion.
		inline void normalize() {
			double l = length();
			if (l == 0)
				return;

			double inv_l = 1.0f / l;
			x = x / l;
			y = y / l;
			z = z / l;
			w = w / l;
		}

		// Returns a normalized version of this quatdernion.
		inline quatd normalized() const {
			quatd q(*this);
			q.normalize();

			return q;
		}

		// Returns the conjugate of this quatdernion (-x, -y, -z, w).
		inline quatd conjugate() const {
			quatd q(*this);

			q.normalize();

			q.x *= -1;
			q.y *= -1;
			q.z *= -1;

			return q;
		}

		// Returns the inverse of this quatdernion (a rotation that "undoes" this quatdernion's rotation).
		inline quatd inverse() const {
			quatd q = conjugate();

			double l = q.lengthSquared();
			if (l <= 0)
				return q;

			double inv_lSqr = 1.0f / l;
			q.x *= inv_lSqr;
			q.y *= inv_lSqr;
			q.z *= inv_lSqr;
			q.w *= inv_lSqr;

			return q;
		}

		// Returns a matrix3f specifying the rotation of this quatdernion.
		inline matrix3f rotationMatrix() const {
			quatd q = this->normalized();

			double xx = q.x * q.x;
			double xy = q.x * q.y;
			double xz = q.x * q.z;
			double xw = q.x * q.w;
			double yy = q.y * q.y;
			double yz = q.y * q.z;
			double yw = q.y * q.w;
			double zz = q.z * q.z;
			double zw = q.z * q.w;
			double ww = q.w * q.w;

			matrix3f m;
			m(0, 0) = 2 * (ww + xx) - 1;
			m(0, 1) = 2 * (xy + zw);
			m(0, 2) = 2 * (xz - yw);

			m(1, 0) = 2 * (xy - zw);
			m(1, 1) = 2 * (ww + yy) - 1;
			m(1, 2) = 2 * (yz + xw);

			m(2, 0) = 2 * (xz + yw);
			m(2, 1) = 2 * (yz - xw);
			m(2, 2) = 2 * (ww + zz) - 1;

			//m(0, 0) = 1.0f - 2.0f * (yy -/**/ zz);
			//m(0, 1) =		 2.0f * (xy + zw);
			//m(0, 2) =		 2.0f * (xz -/**/ yw);

			//m(1, 0) =		 2.0f * (xy -/**/ zw);
			//m(1, 1) = 1.0f - 2.0f * (xx -/**/ zz);
			//m(1, 2) =		 2.0f * (yz + xw);

			//m(2, 0) =		 2.0f * (xz + yw);
			//m(2, 1) =		 2.0f * (yz -/**/ xw);
			//m(2, 2) = 1.0f - 2.0f * (xx -/**/ yy);

			return m;
		}

		// Returns the dot product between quatdernions a and b.
		inline static double dot(const quatd& a, const quatd& b) {
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		// Returns a linearly interpolated quatdernion between a and b by a factor of t.
		inline static quatd lerp(const quatd& a, const quatd& b, const double& t) {
			if (0 > t || t > 1) {
				double l = std::max(0.0, std::min(t, 1.0));
				return a * (1.0f - l) + b * l;
			}

			return a * (1.0f - t) + b * t;
		}

		// Returns a spherically linearly interpolated quatdernion between a and b by a factor of t.
		inline static quatd slerp(const quatd& a, const quatd& b, const double& t) {
			if (0 > t || t > 1) {
				double l = std::max(0.0, std::min(t, 1.0));
				return qPow(a * (a.conjugate() * b), l);
			}

			return qPow(a * (a.conjugate() * b), t);
		}

		// Returns a quatdernion that rotation from vector a to vector b
		inline static quatd fromToRotation(const vec3f& a, const vec3f& b) {
			double aDotB = vec3f::dot(a, b);
			vec3f axis;
			double angle;

			if (aDotB < -0.999f) {
				axis = vec3f::cross(a, vec3f::RIGHT);
				if (axis.length() < 0.0001f)
					axis = vec3f::cross(a, vec3f::UP).normalized();

				angle = 180.0f;
			}
			else {
				axis = vec3f::cross(a, b).normalized();
				angle = RAD2DEG * acos(aDotB);
			}

			return quatd(angle, axis);
		}

		inline static quatd qPow(const quatd& q, const double& exp) {
			quatd q1;
			q1.x = pow(q.x, exp);
			q1.y = pow(q.y, exp);
			q1.z = pow(q.z, exp);
			q1.w = pow(q.w, exp);

			return q1;
		}

		// Returns a rotation such that result = this * other (post-multiplied).
		inline quatd operator*(const quatd& other) const {
			quatd q1;

			q1.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
			q1.x = (other.w * x) + (other.x * w) - (other.y * z) + (other.z * y);
			q1.y = (other.w * y) + (other.x * z) + (other.y * w) - (other.z * x);
			q1.z = (other.w * z) - (other.x * y) + (other.y * x) + (other.z * w);

			q1.normalize();
			return q1;
		}

		// Post-multiplies the rotation other onto this quatdernion.
		inline quatd& operator*=(const quatd& other) {
			*this = *this * other;
			return *this;
		}

		// Returns a copy of v rotated by this quatdernion.
		inline vec3f operator*(const vec3f& v) const {
			// create a "pure" quatdernion (copy v and set w to 0)
			quatd pure;
			pure.x = v.x;
			pure.y = v.y;
			pure.z = v.z;
			pure.w = 0;

			// apply the rotation in the order: conjugate * pure * this
			// see https://math.stackexchange.com/q/40169 for mathematical explanation
			quatd result = this->conjugate() * pure * *this;

			return vec3f(result.x, result.y, result.z);
		}

		// Returns true if both quatdernions result in the same rotation.
		inline bool operator==(const quatd& other) const {
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

		// Returns true if the quatdernions result in different rotations.
		inline bool operator!=(const quatd& other) const {
			return !(*this == other);
		}

	private:
		/* given quatdernions are used to represent rotations, they should always be "unit" quatdernions.
		these are hidden because freely scaling a quatdernion disregarding this rule can result in
		weird, unexpected results. */

		// Hidden scalar operator to allow usage of quatd::lerp function.
		inline quatd operator+(const quatd& q) const {
			quatd q1(*this);

			q1.x += q.x;
			q1.y += q.y;
			q1.z += q.z;
			q1.w += q.w;

			return q1;
		}

		inline quatd& operator+=(const quatd& q) {
			x += q.x;
			y += q.y;
			z += q.z;
			w += q.w;

			return *this;
		}

		// Hidden scalar operator to allow usage of quatd::lerp function.
		inline quatd operator*(const double& scalar) const {
			quatd q1(*this);

			q1.x *= scalar;
			q1.y *= scalar;
			q1.z *= scalar;
			q1.w *= scalar;

			return q1;
		}

		// Hidden scalar operator to allow usage of quatd::lerp function.
		inline quatd& operator*=(const double& scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;

			return *this;
		}
	};
}