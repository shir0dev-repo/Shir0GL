#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace shir0GL {
	struct matrix3f;
	struct quat;

	struct SOGL_API vec3f {
		static vec3f ZERO;
		static vec3f LEFT;
		static vec3f RIGHT;
		static vec3f DOWN;
		static vec3f UP;
		static vec3f BACK;
		static vec3f FORWARD;
		static vec3f ONE;

		float x, y, z;

		vec3f(const float& = 0.0f, const float& = 0.0f, const float& = 0.0f);
		vec3f(const vec3f& v);
		vec3f& operator=(const vec3f& v);

		float length() const;
		float lengthSquared() const;

		vec3f normalized() const;
		void normalize();

		static float dot(const vec3f&, const vec3f&);
		static vec3f cross(const vec3f&, const vec3f&);

		vec3f operator+(const vec3f&) const;
		vec3f& operator+=(const vec3f&);

		vec3f operator-(const vec3f&) const;
		vec3f& operator-=(const vec3f&);

		vec3f operator*(const matrix3f&) const;
		vec3f& operator*=(const matrix3f&);

		vec3f operator*(const quat&) const;
		vec3f& operator*=(const quat&);

		bool operator==(const vec3f&) const;
		bool operator!=(const vec3f&) const;
	};
}