#pragma once

namespace shir0GL {
	struct matrix4f;
	struct quat;

	struct vec3f {
		static vec3f ZERO;
		static vec3f LEFT;
		static vec3f RIGHT;
		static vec3f DOWN;
		static vec3f UP;
		static vec3f BACK;
		static vec3f FORWARD;
		static vec3f ONE;

		float x, y, z;

		vec3f(float = 0.0f, float = 0.0f, float = 0.0f);
		vec3f(const vec3f& v);
		vec3f& operator=(const vec3f& v);

		float length() const;
		float lengthSquared() const;

		vec3f normalized();
		void normalize();

		static float dot(const vec3f& a, const vec3f& b);
		static vec3f cross(const vec3f& a, const vec3f& b);

		vec3f operator+(const vec3f& v);
		vec3f operator+=(const vec3f& v);

		vec3f operator-(const vec3f& v);
		vec3f operator-=(const vec3f& v);

		bool operator==(const vec3f& v);
		bool operator!=(const vec3f& v);
		bool referenceEquals(const vec3f& other);

		vec3f operator*(const matrix4f& m);
		vec3f operator*(const quat& q);
	};
}