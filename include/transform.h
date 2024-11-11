#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace sogl {
	struct vec3f;
	struct quat;
	struct matrix3f;
	struct matrix4f;

	struct SOGL_API transform {
	private:
		matrix4f* m_transformationMatrix;
	public:

		// Constructs a transform which represents no translation, rotation, or scaling.
		transform();
		
		// Constructs a transform which has a translation of position, no rotation, and no scaling.
		transform(const vec3f& position);

		// Constructs a transform which has a translation of position, a rotation of rotation, and no scaling.
		transform(const vec3f& position, const quat& rotation);

		// Constructs a transform which is a copy of the given transform other.
		transform(const transform& other);

		// Copies the transformation matrix
		transform& operator=(const transform& other);

		// Destructor.
		~transform();

		// Returns this transform's transformation matrix.
		matrix4f getTransformationMatrix() const;

		// Returns this transform's rotation matrix.
		matrix3f getRotationMatrix() const;

		// Returns this transform's position in world space.
		vec3f getPosition() const;

		// Sets this transform's position.
		void setPosition(const vec3f& position);

		// Translates this transform by the delta position.
		void translate(const vec3f& deltaPosition);

		// Returns this transform's rotation.
		quat getRotation() const;

		// Returns this transform's euler rotation.
		vec3f getEulerRotation() const;

		// Sets this transform's rotation.
		void setRotation(const quat& rotation);

		// Sets this transform's euler rotation.
		void setRotation(const vec3f& eulerAngles);

		// Sets this transform's rotation matrix.
		void setRotation(const matrix3f& rotationMatrix);

		// Rotates this transform by the delta rotation quaternion.
		void rotate(const quat& deltaRotation);

		// Rotates this transform by the delta euler angles.
		void rotate(const vec3f& deltaEuler);

		bool operator==(const transform& other) const;
		bool operator!=(const transform& other) const;
	};
}