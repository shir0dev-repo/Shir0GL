#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace shir0GL {
	struct matrix3f;
	struct matrix4f;
	struct vec3f;

	// 4D vector that handles rotations.
	struct SOGL_API quat {
		static quat identity;

		float x, y, z, w;

		// Identity quaternion (no rotation).
		quat();
		
		// Constructs a quaternion from the euler angles xDegrees, yDegrees, and zDegrees.
		quat(const float&, const float&, const float&);
		
		// Constructs a quaternion from the euler angles v.
		quat(const vec3f&);
		
		// Constructs a quaternion from the rotation matrix mat.
		quat(const matrix3f&);
		
		// Copies values of quaternion other.
		quat(const quat&);
		
		// Assigns this quaternion's components from rhs.
		quat& operator=(const quat&);

		vec3f toEulerAngles() const;

		// Returns the length of this quaternion.
		float length() const;
		// Returns the squared length of this quaternion.
		float lengthSquared() const;

		// Returns a normalized version of this quaternion.
		quat normalized() const;
		// Normalizes this quaternion.
		void normalize();

		// Returns the conjugate of this quaternion (-x, -y, -z, w).
		quat conjugate() const;
		// Returns the inverse of this quaternion (a rotation that "undoes" this quaternion's rotation).
		quat inverse() const;
		// Returns a matrix3f specifying the rotation of this quaternion.
		matrix3f toMatrix3f() const;

		// Returns the dot product between quaternions a and b.
		static float dot(const quat&, const quat&);

		// Returns a linearly interpolated quaternion between a and b by a factor of t.
		static quat lerp(const quat&, const quat&, const float&);
		// Returns a spherically linearly interpolated quaternion between a and b by a factor of t.
		static quat slerp(const quat&, const quat&, const float&);

		// Constructs a quaternion from the given euler angles.
		static quat euler(const vec3f&);
		// Constructs a quaternion from the given euler angles.
		static quat euler(const float&, const float&, const float&);
		// Constructs a quaternion that rotates angle degrees around axis.
		static quat axisAngle(const vec3f&, const float&);

		// Returns a quaternion that rotation from vector a to vector b
		static quat fromToRotation(const vec3f&, const vec3f&);
		
		quat operator+(const quat&) const;
		quat& operator+=(const quat&);

		quat operator-(const quat&) const;
		quat& operator-=(const quat&);

		quat operator*(const float&) const;
		quat& operator*=(const float&);

		quat operator*(const quat&) const;
		quat& operator*=(const quat&);
		
		/*NOT A BINARY OPERATOR!!
		Returns a copied version of this quaternion whose components are raised to a power of exp.*/
		quat operator^(const float&) const;
		/*NOT A BINARY OPERATOR!!
		Returns a copied version of this quaternion whose components are raised to a power of exp.*/
		quat& operator^=(const float&);

		bool operator==(const quat&) const;
		bool operator!=(const quat&) const;
	};

	
}