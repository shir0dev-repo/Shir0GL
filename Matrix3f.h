#pragma once

namespace shir0GL {
	struct vec3f;

	/*	Matrix3f is in COLUMN-MAJOR order.
	This means that my_matrix(1,2) is equivalent to the second column, third row.
	*/
	struct matrix3f {
		// Creates an identity Matrix3f.
		matrix3f();
		// Creates a new Matrix3f with values copied from m.
		matrix3f(matrix3f const&);
		// Creates a new Matrix3f with values copied from m.
		matrix3f& operator=(const matrix3f&);
		// Destructor.
		~matrix3f();

		float determinant() const;

		matrix3f inverted() const;
		void invert();
		
		matrix3f transposed() const;
		void transpose();

		// Returns a vec3f for the specified row.
		vec3f getRow(const unsigned&) const;
		// Returns a vec3f for the specified column.
		vec3f getColumn(const unsigned&) const;
		// Returns a const pointer to the matrix's values.
		float* const getPointer();

		// Returns the value at specified column and row.
		float  operator() (const unsigned& column, const unsigned& row) const;
		// Returns the address at the specified column and row.
		float& operator() (const unsigned& column, const unsigned& row);
		
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