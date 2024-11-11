#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace sogl {
	struct vec3f;
	struct matrix4f;

	struct SOGL_API vec4f {		
		float x, y, z, w;

		vec4f(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f, const float& w = 1.0f);
		vec4f(const vec3f& v);
		vec4f(const vec4f& v);
		vec4f& operator=(const vec4f& v);

		static float dot(const vec4f& a, const vec4f& b);

		vec4f operator+(const vec4f&) const;
		vec4f& operator+=(const vec4f&);

		vec4f operator-(const vec4f&) const;
		vec4f& operator-=(const vec4f&);

		bool operator==(const vec4f&) const;
		bool operator!=(const vec4f&) const;

		// Returns a truncated (x, y, z, no w) version of this vec4f.
		explicit operator vec3f() const;
	};
}