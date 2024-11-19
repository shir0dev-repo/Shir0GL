#pragma once

#include <sogl/transform/vec3f.hpp>
#include <sogl/transform/vec4f.hpp>
#include <sogl/transform/quat.hpp>
#include <sogl/transform/matrix3f.hpp>

namespace sogl {
	/*	matrix4f is in COLUMN-MAJOR order.
	This means that my_matrix(1,2) is equivalent to the second column, third row.
	*/
	struct matrix4f {
		static matrix4f IDENTITY;

	private:
		static const int length = 16;
		float m_values[length];
		inline static unsigned to1D(const unsigned& column, const unsigned& row) {
			return (column * 4) + row;
		}
	public:

		inline matrix4f(const char*) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m_values[to1D(i, j)] = to1D(i, j);
				}
			}
		}

		// Constructs an identity matrix.
		inline matrix4f() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m_values[to1D(i, j)] = (i == j) ? 1.0f : 0.0f;
				}
			}
		}

		// Copies the given matrix.
		inline matrix4f(matrix4f const& mat) {
			*this = mat;
		}

		// Assigns the given matrix's values to this matrix.
		inline matrix4f& operator=(const matrix4f& mat) {
			for (int i = 0; i < 16; i++) {
				m_values[i] = mat.m_values[i];
			}

			return *this;
		}

		// Returns a pointer to the matrix's values (should only be used for OpenGL).
		inline const float* getPointer() const {
			return &m_values[0];
		}

		// Returns a 4D vector representing the given column.
		inline vec4f getColumn(const unsigned& column) const {
			vec4f v;
			v.x = m_values[to1D(column, 0)];
			v.y = m_values[to1D(column, 1)];
			v.z = m_values[to1D(column, 2)];
			v.w = m_values[to1D(column, 3)];

			return v;
		}

		inline void setColumn(const unsigned& col, const vec4f& v) {
			m_values[to1D(col, 0)] = v.x;
			m_values[to1D(col, 1)] = v.y;
			m_values[to1D(col, 2)] = v.z;
			m_values[to1D(col, 3)] = v.w;
		}

		// Returns a 4D vector representing the given row.
		inline vec4f getRow(const unsigned& row) const {
			vec4f v;
			v.x = m_values[to1D(0, row)];
			v.y = m_values[to1D(1, row)];
			v.z = m_values[to1D(2, row)];
			v.w = m_values[to1D(3, row)];

			return v;
		}

		inline void setRow(const unsigned& row, const vec4f& v) {
			m_values[to1D(0, row)] = v.x;
			m_values[to1D(1, row)] = v.y;
			m_values[to1D(2, row)] = v.z;
			m_values[to1D(3, row)] = v.w;
		}

		// Returns the determinant of this matrix.
		inline float determinant() const {
			int laplace = 1;
			float det = 0;

			for (int i = 0; i < 4; i++) {

				det += laplace * m_values[i] * submatrix(0, i).determinant();
				laplace *= -1;
			}

			return det;
		}

		// Returns a given submatrix defined by eliminating the given column and row.
		inline matrix3f submatrix(const unsigned& deletedColumn, const unsigned& deletedRow) const {
			unsigned matrixIndex = 0;
			matrix3f mat;
			for (int i = 0; i < 4; i++) {
				if (i == deletedColumn) continue;

				for (int j = 0; j < 4; j++) {
					if (j == deletedRow) continue;

					mat(matrixIndex / 3, matrixIndex % 3) = m_values[to1D(i, j)];
					matrixIndex++;
				}
			}

			return mat;
		}

		// Returns the cofactor of this matrix.
		inline matrix4f cofactor() const {
			matrix4f cofactor;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					matrix3f sub = submatrix(i, j);
					cofactor.m_values[to1D(i, j)] = pow(-1.0f, i + j) * sub.determinant();
				}
			}

			return cofactor;
		}

		// Returns a copy of this matrix inverted by using Gauss-Jordan Elimination.
		inline matrix4f inverted() const {
			matrix4f mat(*this);
			mat.invert();

			return mat;
		}

		// Inverts this matrix using Gauss-Jordan Elimination.
		inline void invert() {
			matrix4f augmented(*this);
			matrix4f inverse;

			// row iterator
			for (int i = 0; i < 4; i++) {
				int pivotRow = i;

				// column iterator
				for (int j = i + 1; j < 4; j++) {
					if (abs(augmented.m_values[to1D(i, j)]) > abs(augmented.m_values[to1D(i, pivotRow)])) {
						pivotRow = j;
					}
				}

				if (pivotRow != i) {
					// column iterator
					for (int col = 0; col < 4; col++) {
						std::swap(augmented.m_values[to1D(col, i)], augmented.m_values[to1D(col, pivotRow)]);
						std::swap(inverse.m_values[to1D(col, i)], inverse.m_values[to1D(col, pivotRow)]);
					}
				}

				float pivotValue = augmented.m_values[to1D(i, i)];
				if (pivotValue == 0) return;

				float invPivotValue = 1.0f / pivotValue;

				// column iterator
				for (int col = 0; col < 4; col++) {
					augmented.m_values[to1D(col, i)] *= invPivotValue;
					inverse.m_values[to1D(col, i)] *= invPivotValue;
				}


				//row iterator
				for (int j = 0; j < 4; j++) {
					if (j == i) continue;
					float factor = augmented.m_values[to1D(i, j)];

					// column iterator
					for (int col = 0; col < 4; col++) {
						augmented.m_values[to1D(col, j)] -= factor * augmented.m_values[to1D(col, i)];
						inverse.m_values[to1D(col, j)] -= factor * inverse.m_values[to1D(col, i)];
					}
				}
			}

			*this = inverse;
		}

		// Returns a copy of this matrix which is transposed.
		inline matrix4f transposed() const {
			matrix4f mat(*this);
			mat.transpose();

			return mat;
		}

		// Transposes this matrix.
		inline void transpose() {
			matrix4f temp;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					temp.m_values[to1D(i, j)] = m_values[to1D(j, i)];
				}
			}

			*this = temp;
		}

		
		
		// Returns a 3D vector representing this matrix's translation.
		inline vec3f getPosition() const {
			return (vec3f)getColumn(3);
		}

		// Sets this matrix's position.
		inline void setPosition(const vec3f& position) {
			vec4f pos(position);
			setColumn(3, pos);
		}

		// Applies the translation to this matrix.
		inline void translate(const vec3f& translation) {
			m_values[to1D(3, 0)] += translation.x;
			m_values[to1D(3, 1)] += translation.y;
			m_values[to1D(3, 2)] += translation.z;
		}

		// Returns the extracted rotation matrix of this matrix.
		inline matrix3f getRotationMatrix(const bool& removeScale = false) const {
			matrix3f rm = submatrix(3, 3);
			if (!removeScale)
				return rm;
			
			vec3f scale;
			scale.x = rm.getRow(0).length();
			scale.y = rm.getRow(1).length();
			scale.z = rm.getRow(2).length();

			rm(0, 0) /= scale.x; rm(1, 0) /= scale.y; rm(2, 0) /= scale.z;
			rm(0, 1) /= scale.x; rm(1, 1) /= scale.y; rm(2, 1) /= scale.z;
			rm(0, 2) /= scale.x; rm(1, 2) /= scale.y; rm(2, 2) /= scale.z;

			return rm;
		}
		
		// Returns the quaternion representation of this matrix's rotation.
		inline quat getRotation() const {
			return quat(getRotationMatrix());
		}

		// Sets this matrix's rotation.
		inline void setRotation(const quat& rotation) {
			setRotation(rotation.rotationMatrix());
		}

		// Sets this matrix's rotation.
		inline void setRotation(const matrix3f& rotationMatrix) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					m_values[to1D(i, j)] = rotationMatrix(i, j);
				}
			}

			setRow(3, vec4f(0, 0, 0, 1));
			orthonormalize();
		}

		// Applies a rotation to this matrix.
		inline void rotate(const quat& rotation) {
			rotate(rotation.rotationMatrix());
		}

		// Applies a rotation to this matrix.
		inline void rotate(const matrix3f& rotationMatrix) {
			setRotation(rotationMatrix * getRotationMatrix());
			orthonormalize();
		}

		inline void orthonormalize() {
			vec3f p = getPosition();
			matrix3f rm = getRotationMatrix(true);

			vec3f basisX = rm.getColumn(0);
			vec3f basisY = rm.getColumn(1);
			vec3f basisZ = rm.getColumn(2);

			vec3f u1 = basisX.normalized();

			vec3f w2 = basisY - vec3f::project(basisY, basisX);
			vec3f u2 = w2.normalized();

			vec3f w3 = basisZ - vec3f::project(basisZ, basisX);
			w3 = w3 - vec3f::project(basisZ, w2);
			vec3f u3 = w3.normalized();

			m_values[to1D(0, 0)] = u1.x; m_values[to1D(1, 0)] = u2.x; m_values[to1D(2, 0)] = u3.x; m_values[to1D(3, 0)] = p.x;
			m_values[to1D(0, 1)] = u1.y; m_values[to1D(1, 1)] = u2.y; m_values[to1D(2, 1)] = u3.y; m_values[to1D(3, 1)] = p.y;
			m_values[to1D(0, 2)] = u1.z; m_values[to1D(1, 2)] = u2.z; m_values[to1D(2, 2)] = u3.z; m_values[to1D(3, 2)] = p.z;
			m_values[to1D(0, 3)] = 0; m_values[to1D(1, 3)] = 0;	m_values[to1D(2, 3)] = 0; m_values[to1D(3, 3)] = 1;

		}

		// Returns the value at specified column and row.
		inline float operator() (const unsigned& column, const unsigned& row) const {
			float v = m_values[to1D(column, row)];
			return v;
		}

		// Returns the modifiable l-value at the specified column and row.
		inline float& operator() (const unsigned& column, const unsigned& row) {
			return m_values[to1D(column, row)];
		}

		// Returns a copy of v multiplied by this matrix.
		inline vec3f operator*(const vec3f& v) const {
			vec4f v4(v);
			return (vec3f)(*this * v4);
		}

		// Returns a copy of v multiplied by this matrix.
		inline vec4f operator*(const vec4f& v) const {
			vec4f result;
			result.x = vec4f::dot(v, getColumn(0));
			result.y = vec4f::dot(v, getColumn(1));
			result.z = vec4f::dot(v, getColumn(2));
			result.w = vec4f::dot(v, getColumn(3));

			return result;
		}

		// Returns a matrix whose elements are this matrix + mat.
		inline matrix4f operator+(const matrix4f& mat) const {
			matrix4f m(*this);
			for (int i = 0; i < 16; i++) {
				m.m_values[i] += mat.m_values[i];
			}

			return m;
		}

		// Adds the elements of mat to this matrix.
		inline matrix4f& operator+=(const matrix4f& mat) {
			for (int i = 0; i < 16; i++) {
				m_values[i] += mat.m_values[i];
			}

			return *this;
		}

		// Returns a matrix whose elements are this matrix - mat.
		inline matrix4f operator-(const matrix4f& mat) const {
			matrix4f m(*this);
			for (int i = 0; i < 16; i++) {
				m.m_values[i] -= mat.m_values[i];
			}

			return m;
		}

		// Subtracts the elements of mat from this matrix.
		inline matrix4f& operator-=(const matrix4f& mat) {
			for (int i = 0; i < 16; i++) {
				m_values[i] -= mat.m_values[i];
			}
			
			return *this;
		}

		// Returns a copy of this matrix scaled by scalar.
		inline matrix4f operator*(const float& scalar) const {
			matrix4f m(*this);
			for (int i = 0; i < 16; i++) {
				m.m_values[i] *= scalar;
			}

			return m;
		}
		
		// Scales this matrix's elements by scalar.
		inline matrix4f& operator*=(const float& scalar) {
			for (int i = 0; i < 16; i++) {
				m_values[i] *= scalar;
			}

			return *this;
		}

		// Returns a matrix whose elements are mat * this matrix (right multiplication).
		inline matrix4f operator*(const matrix4f& mat) const {
			matrix4f m;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m.m_values[to1D(j, i)] = vec4f::dot(this->getRow(j), mat.getColumn(i));
				}
			}

			return m;
		}

		// Multiplies this matrix's elements by mat (right multiplication).
		inline matrix4f& operator*=(const matrix4f& mat) {
			matrix4f m;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m.m_values[to1D(j, i)] = vec4f::dot(this->getRow(j), mat.getColumn(i));
				}
			}

			*this = m;
			return *this;
		}

		// Checks if both matrices contain the same values.
		inline bool operator==(const matrix4f& other) const {
			for (int i = 0; i < 16; i++) {
				if (m_values[i] != other.m_values[i])
					return false;
			}
			return true;
		}

		// Checks if both matrices contain differing values.
		inline bool operator!=(const matrix4f& other) const {
			return !(*this == other);
		}
	};
}