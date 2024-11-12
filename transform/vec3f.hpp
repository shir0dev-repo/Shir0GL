#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

#include <math.h>

namespace sogl {
	struct SOGL_API vec3f {	
		static vec3f ZERO;
		static vec3f LEFT;
		static vec3f RIGHT;
		static vec3f DOWN;
		static vec3f UP;
		static vec3f BACK;
		static vec3f FORWARD;
		static vec3f ONE;

		float x;
		float y;
		float z;

		inline vec3f() : x(0.0f), y(0.0f), z(0.0f) { }

		inline vec3f(const float& x, const float& y, const float& z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		inline vec3f(const vec3f& v) {
			*this = v;
		}

		inline vec3f& operator=(const vec3f& v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;

			return *this;
		}

		inline float length() const {
			return sqrt(x * x + y * y + z * z);
		}
		inline float lengthSquared() const {
			return x * x + y * y + z * z;
		}

		inline vec3f normalized() const {
			vec3f v(*this);
			
			float inv_l = 1.0f / v.length();
			v.x *= inv_l;
			v.y *= inv_l;
			v.z *= inv_l;

			return v;
		}

		inline void normalize() {
			float inv_l = 1.0f / length();
			x *= inv_l;
			y *= inv_l;
			z *= inv_l;
		}

		inline static float dot(const vec3f& a, const vec3f& b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		inline static vec3f cross(const vec3f& a, const vec3f& b) {
			vec3f c;
			c.x = (a.y * b.z) - (a.z * b.y);
			c.y = (a.z * b.x) - (a.x * b.z);
			c.z = (a.x * b.y) - (a.y * b.x);

			return c;
		}

		inline vec3f operator+(const vec3f& v) const {
			vec3f v1(*this);
			v1.x += v.x;
			v1.y += v.y;
			v1.z += v.z;

			return v1;
		}

		inline vec3f& operator+=(const vec3f& v) {
			x += v.x;
			y += v.y;
			z += v.z;

			return *this;
		}

		inline vec3f operator-(const vec3f& v) const {
			vec3f v1(*this);
			v1.x -= v.x;
			v1.y -= v.y;
			v1.z -= v.z;

			return v1;
		}

		inline vec3f& operator-=(const vec3f& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		inline vec3f operator*(const float& scalar) const {
			vec3f v(*this);

			v.x *= scalar;
			v.y *= scalar;
			v.z *= scalar;

			return v;
		}
		
		inline vec3f& operator*=(const float& scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		inline vec3f operator*(const vec3f& v) const {
			vec3f v1(*this);

			v1.x *= v.x;
			v1.y *= v.y;
			v1.z *= v.z;

			return v1;
		}

		inline vec3f& operator*=(const vec3f& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;

			return *this;
		}

		inline bool operator==(const vec3f& other) const {
			return x == other.x && y == other.y && z == other.z;
		}

		inline bool operator!=(const vec3f& other) const {
			return !(*this == other);
		}
	};


}