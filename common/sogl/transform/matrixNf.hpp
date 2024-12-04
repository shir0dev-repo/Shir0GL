/*
#pragma once

#include <algorithm>
#include <sogl/transform/vec3f.hpp>

namespace sogl {
	/*	matrix3f is in COLUMN-MAJOR order.
	This means that my_matrix(1,2) is equivalent to the second column, third row.
	
struct matrix3f {
	static matrix3f IDENTITY;

private:
	static const int length = 9;
	float m_values[length];
	inline static unsigned to1D(const unsigned& col, const unsigned& row) {
		return (col * 3) + row;
	}
public:

	// Creates an identity Matrix3f.
	inline matrix3f() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m_values[to1D(i, j)] = i == j ? 1.0f : 0.0f;
			}
		}
	}

	// Creates a new Matrix3f with values copied from mat.
	inline matrix3f(const matrix3f& mat) {
		*this = mat;
	}

	// Creates a new Matrix3f with values copied from mat.
	inline matrix3f& operator=(const matrix3f& mat) {
		for (int i = 0; i < 9; i++) {
			m_values[i] = mat.m_values[i];
		}

		return *this;
	}

	// Returns a const pointer to the matrix's values.
	inline const float* getPointer() const {
		return &m_values[0];
	}

	// Returns a vec3f for the specified column.
	inline vec3f getColumn(const unsigned& col) const {
		vec3f v;
		v.x = m_values[to1D(col, 0)];
		v.y = m_values[to1D(col, 1)];
		v.z = m_values[to1D(col, 2)];

		return v;
	}

	// Returns a vec3f for the specified row.
	inline vec3f getRow(const unsigned& row) const {
		vec3f v;
		v.x = m_values[to1D(0, row)];
		v.y = m_values[to1D(1, row)];
		v.z = m_values[to1D(2, row)];

		return v;
	}

	// Returns the determinant of this matrix.
	inline float determinant() const {
		matrix3f m(*this);
		float det = 0;
		float sign = 1;
		for (int col = 0; col < 3; col++) {
			if (getRow(col).lengthSquared() == 0) {
				return 0.0f;
			}

			unsigned left = (col + 1) % 3;
			unsigned right = (col + 2) % 3;

			float d = m(col, 0) * (m(left, 1) * m(right, 2)) - (m(right, 1), m(left, 2));
			det += d * sign;
			sign *= -1;
		}

		return det;
	}

	// Returns an inverted version of this matrix.
	inline matrix3f inverted() const {
		matrix3f mat(*this);
		mat.invert();

		return mat;
	}

	// Inverts this matrix.
	inline void invert() {
		float det = this->determinant();
		if (det == 0) return;

		matrix3f augmented(*this);
		matrix3f inverse;

		// row iterator
		for (int i = 0; i < 3; i++) {
			int pivotRow = i;

			// column iterator
			for (int j = i + 1; j < 3; j++) {
				if (abs(augmented.m_values[to1D(i, j)]) > abs(augmented.m_values[to1D(i, pivotRow)])) {
					pivotRow = j;
				}
			}

			if (pivotRow != i) {
				// column iterator
				for (int k = 0; k < 3; k++) {
					std::swap(augmented.m_values[k, i], augmented.m_values[to1D(k, pivotRow)]);
					std::swap(inverse.m_values[k, i], inverse.m_values[to1D(k, pivotRow)]);
				}
			}

			float pivotValue = augmented.m_values[to1D(i, i)];
			if (pivotValue == 0) return;

			float invPivotValue = 1.0f / pivotValue;

			// column iterator
			for (int k = 0; k < 3; k++) {
				augmented.m_values[to1D(k, i)] *= invPivotValue;
				inverse.m_values[to1D(k, i)] *= invPivotValue;
			}


			//row iterator
			for (int j = 0; j < 3; j++) {
				if (j == i) continue;
				float factor = augmented.m_values[to1D(i, j)];

				// column iterator
				for (int k = 0; k < 3; k++) {
					augmented.m_values[to1D(k, j)] -= factor * augmented.m_values[to1D(k, i)];
					inverse.m_values[to1D(k, j)] -= factor * inverse.m_values[to1D(k, i)];
				}
			}
		}

		*this = inverse;
	}

	// Returns a copy of this matrix which is transposed.
	inline matrix3f transposed() const {
		matrix3f mat(*this);
		mat.transpose();

		return mat;
	}

	// Transposes this matrix.
	inline void transpose() {
		matrix3f temp;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp(i, j) = this->operator()(j, i);
			}
		}

		*this = temp;
	}

	// Returns the value at specified column and row.
	inline float operator() (const unsigned& column, const unsigned& row) const {
		return m_values[to1D(column, row)];
	}

	// Returns the address at the specified column and row.
	inline float& operator() (const unsigned& column, const unsigned& row) {
		return m_values[to1D(column, row)];
	}

	inline vec3f operator*(const vec3f& v) const {
		vec3f temp;
		temp.x = v.x * m_values[to1D(0, 0)] + v.y * m_values[to1D(1, 0)] + v.z * m_values[to1D(2, 0)];
		temp.y = v.x * m_values[to1D(0, 1)] + v.y * m_values[to1D(1, 1)] + v.z * m_values[to1D(2, 1)];
		temp.z = v.x * m_values[to1D(0, 2)] + v.y * m_values[to1D(1, 2)] + v.z * m_values[to1D(2, 2)];

		return temp;
	}

	inline matrix3f operator+(const matrix3f& mat) const {
		matrix3f m(*this);
		for (int i = 0; i < 9; i++) {
			m.m_values[i] += mat.m_values[i];
		}

		return m;
	}

	inline matrix3f& operator+=(const matrix3f& mat) {
		for (int i = 0; i < 9; i++) {
			m_values[i] += mat.m_values[i];
		}

		return *this;
	}

	inline matrix3f operator-(const matrix3f& mat) const {
		matrix3f m(*this);
		for (int i = 0; i < 9; i++) {
			m.m_values[i] -= mat.m_values[i];
		}

		return m;
	}

	inline matrix3f& operator-=(const matrix3f& mat) {
		for (int i = 0; i < 9; i++) {
			m_values[i] -= mat.m_values[i];
		}

		return *this;
	}

	inline matrix3f operator*(const float& scalar) const {
		matrix3f m(*this);
		for (int i = 0; i < 9; i++) {
			m.m_values[i] *= scalar;
		}

		return m;
	}

	inline matrix3f& operator*=(const float& scalar) {
		for (int i = 0; i < 9; i++) {
			m_values[i] *= scalar;
		}

		return *this;
	}

	inline matrix3f operator*(const matrix3f& mat) const {
		matrix3f m;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m(j, i) = vec3f::dot(this->getRow(i), mat.getColumn(j));
			}
		}
		return m;
	}

	inline matrix3f& operator*=(const matrix3f& mat) {
		*this = *this * mat;
		return *this;
	}

	inline bool operator==(const matrix3f& other) const {
		for (int i = 0; i < 9; i++) {
			if (m_values[i] != other.m_values[i])
				return false;
		}

		return true;
	}

	inline bool operator!=(const matrix3f& other) const {
		return !(*this == other);
	}
};
}
*/