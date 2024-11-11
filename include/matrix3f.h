#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace sogl {
	struct vec3f;

	/*	matrix3f is in COLUMN-MAJOR order.
	This means that my_matrix(1,2) is equivalent to the second column, third row.
	*/
	struct SOGL_API matrix3f {
		static matrix3f IDENTITY;

		// Creates an identity Matrix3f.
		matrix3f();

		// Creates a new Matrix3f with values copied from m.
		matrix3f(const matrix3f&);

		// Creates a new Matrix3f with values copied from m.
		matrix3f& operator=(const matrix3f&);

		// Destructor.
		~matrix3f();

		// Returns the determinant of this matrix.
		float determinant() const;

		// Returns an inverted version of this matrix.
		matrix3f inverted() const;

		// Inverts this matrix.
		void invert();
		
		// Returns a copy of this matrix which is transposed.
		matrix3f transposed() const;

		// Transposes this matrix.
		void transpose();

		// Returns a vec3f for the specified row.
		vec3f getRow(const unsigned&) const;
		// Returns a vec3f for the specified column.
		vec3f getColumn(const unsigned&) const;
		// Returns a const pointer to the matrix's values.
		float* const getPointer();

		// Returns the value at specified column and row.
		float operator() (const unsigned& column, const unsigned& row) const;

		// Returns the address at the specified column and row.
		float& operator() (const unsigned& column, const unsigned& row);
		
		vec3f operator*(const vec3f&) const;

		matrix3f operator+(const matrix3f&) const;
		matrix3f& operator+=(const matrix3f&);

		matrix3f operator-(const matrix3f&) const;
		matrix3f& operator-=(const matrix3f&);

		matrix3f operator*(const float&) const;
		matrix3f& operator*=(const float&);

		matrix3f operator*(const matrix3f&) const;
		matrix3f& operator*=(const matrix3f&);

		bool operator==(const matrix3f&) const;
		bool operator!=(const matrix3f&) const;
	private:
		float* m_values;

		static unsigned to1D(const unsigned&, const unsigned&);
	};
}