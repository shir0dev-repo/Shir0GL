#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace sogl {
	struct vec3f;
	struct vec4f;
	struct quat;
	struct matrix3f;
	/*	matrix4f is in COLUMN-MAJOR order.
	This means that my_matrix(1,2) is equivalent to the second column, third row.
	*/
	struct SOGL_API matrix4f {
		static matrix4f IDENTITY;

		// Constructs an identity matrix.
		matrix4f();

		// Copies the given matrix.
		matrix4f(const matrix4f&);

		// Assigns the given matrix's values to this matrix.
		matrix4f operator=(const matrix4f&);

		// delete[]'s this matrix's values and set's the value pointer to nullptr.
		~matrix4f();

		// Returns a pointer to the matrix's values (should only be used for OpenGL).
		float* const getPointer() const;

		// Returns the determinant of this matrix.
		float determinant() const;

		// Returns a given submatrix defined by eliminating the given column and row.
		matrix3f submatrix(const unsigned&, const unsigned&) const;

		// Returns the cofactor of this matrix.
		matrix4f cofactor() const;

		// Returns a copy of this matrix inverted by using Gauss-Jordan Elimination.
		matrix4f inverted() const;

		// Inverts this matrix using Gauss-Jordan Elimination.
		void invert();

		// Returns a copy of this matrix which is transposed.
		matrix4f transposed() const;

		// Transposes this matrix.
		void transpose();

		// Returns a 4D vector representing the given row.
		vec4f getRow(const unsigned&) const;

		// Returns a 4D vector representing the given column.
		vec4f getColumn(const unsigned&) const;
		
		// Returns a 3D vector representing this matrix's translation.
		vec3f getTranslation() const;

		// Sets this matrix's translation.
		void setTranslation(const vec3f&);

		// Applies a translation to this matrix.
		void translate(const vec3f&);

		// Returns the extracted rotation matrix of this matrix.
		matrix3f getRotationMatrix() const;

		// Returns the quaternion representation of this matrix's rotation.
		quat getRotation() const;

		// Sets this matrix's rotation.
		void setRotation(const quat&);

		// Sets this matrix's rotation.
		void setRotation(const matrix3f&);

		// Applies a rotation to this matrix.
		void rotate(const quat&);

		// Applies a rotation to this matrix.
		void rotate(const matrix3f&);

		// Returns the value at specified column and row.
		float  operator() (const unsigned& column, const unsigned& row) const;

		// Returns the modifiable l-value at the specified column and row.
		float& operator() (const unsigned& column, const unsigned& row);

		// Returns a copy of v multiplied by this matrix.
		vec3f operator*(const vec3f&) const;

		// Returns a copy of v multiplied by this matrix.
		vec4f operator*(const vec4f&) const;

		// Returns a matrix whose elements are this matrix + mat.
		matrix4f operator+(const matrix4f& mat) const;

		// Adds the elements of mat to this matrix.
		matrix4f& operator+=(const matrix4f& mat);

		// Returns a matrix whose elements are this matrix - mat.
		matrix4f operator-(const matrix4f&) const;

		// Subtracts the elements of mat from this matrix.
		matrix4f& operator-=(const matrix4f&);

		// Returns a copy of this matrix scaled by scalar.
		matrix4f operator*(const float& scalar) const;
		
		// Scales this matrix's elements by scalar.
		matrix4f& operator*=(const float& scalar);

		// Returns a matrix whose elements are mat * this matrix (right multiplication).
		matrix4f operator*(const matrix4f& mat) const;

		// Multiplies this matrix's elements by mat (right multiplication).
		matrix4f& operator*=(const matrix4f&);

		// Checks if both matrices contain the same values.
		bool operator==(const matrix4f&) const;

		// Checks if both matrices contain differing values.
		bool operator!=(const matrix4f&) const;

	private:
		float* m_values;

		static unsigned to1D(const unsigned&, const unsigned&);
	};
}