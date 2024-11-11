#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

#include "vec3f.inl"
#include "vec4f.inl"
#include "quat.inl"
#include "matrix3f.h"
#include "matrix4f.h"

namespace sogl {
	// The transform struct currently does not support scaling.
	struct SOGL_API transform {
	private:
		matrix4f m_transformationMatrix;
	public:

		// Constructs a transform which represents no translation or rotation.
		inline transform() : m_transformationMatrix() { }
		
		// Constructs a transform which has a translation of position, and no rotation.
		inline transform(const vec3f& position) : transform() {
			m_transformationMatrix.setTranslation(position);
		}

		// Constructs a transform which has a translation of position, and a rotation of rotation.
		inline transform(const vec3f& position, const quat& rotation) : transform(position) {
			m_transformationMatrix.setRotation(rotation);
		}

		// Constructs a transform which is a copy of the given transform other.
		inline transform(const transform& other) {
			*this = other;
		}

		// Copies the transformation matrix
		inline transform& operator=(const transform& other) {
			this->m_transformationMatrix = other.m_transformationMatrix;
		}

		// Returns this transform's transformation matrix.
		inline matrix4f getTransformationMatrix() const {
			return m_transformationMatrix;
		}

		// Returns this transform's position in world space.
		inline vec3f getPosition() const {
			return (vec3f) m_transformationMatrix.getColumn(3);
		}

		// Sets this transform's position.
		inline void setPosition(const vec3f& position) {
			m_transformationMatrix.setTranslation(position);
		}

		// Translates this transform by the delta position.
		inline void translate(const vec3f& deltaPosition) {
			m_transformationMatrix.translate(deltaPosition);
		}

		// Returns this transform's rotation.
		inline quat getRotation() const {
			return m_transformationMatrix.getRotation();
		}

		// Returns this transform's euler rotation.
		inline vec3f getEulerRotation() const {
			return m_transformationMatrix.getRotation().toEulerAngles();
		}

		// Returns this transform's rotation matrix.
		inline matrix3f getRotationMatrix() const {
			return m_transformationMatrix.getRotationMatrix();
		}

		// Sets this transform's rotation.
		inline void setRotation(const quat& rotation) {
			m_transformationMatrix.setRotation(rotation);
		}

		// Sets this transform's euler rotation.
		inline void setRotation(const vec3f& eulerAngles) {
			quat q(eulerAngles);
			m_transformationMatrix.setRotation(q);
		}

		// Sets this transform's rotation matrix.
		inline void setRotation(const matrix3f& rotationMatrix) {
			m_transformationMatrix.setRotation(rotationMatrix);
		}

		// Rotates this transform by the delta rotation quaternion.
		inline void rotate(const quat& deltaRotation) {
			m_transformationMatrix.rotate(deltaRotation);
		}

		// Rotates this transform by the delta euler angles.
		inline void rotate(const vec3f& deltaEuler) {
			quat q(deltaEuler);
			m_transformationMatrix.rotate(q);
		}

		// Returns true if both transforms' position and rotation are equal.
		inline bool operator==(const transform& other) const {
			return m_transformationMatrix == other.m_transformationMatrix;
		}

		// Returns true if the given transform's position or rotation are different.
		inline bool operator!=(const transform& other) const {
			return !(*this == other);
		}
	};
}