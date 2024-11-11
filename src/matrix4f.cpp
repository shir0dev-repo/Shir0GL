#include <algorithm>

#include "matrix4f.h"

#include "math.h"
#include "matrix3f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "quat.h"


namespace sogl {
#ifdef SOGL_EXPORT
	matrix4f matrix4f::IDENTITY = matrix4f();
#endif

	matrix4f::matrix4f() {
		m_values = new float[16];
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m_values[to1D(i, j)] = (i == j) ? 1.0f : 0.0f;
			}
		}
	}

	matrix4f::matrix4f(const matrix4f& mat) {
		*this = mat;
	}

	matrix4f matrix4f::operator=(const matrix4f& mat) {
		for (int i = 0; i < 16; i++) {
			m_values[i] = mat.m_values[i];
		}

		return *this;
	}

	matrix4f::~matrix4f() {
		delete[] m_values;
		m_values = nullptr;
	}

	float* const matrix4f::getPointer() const {
		return &m_values[0];
	}

	float matrix4f::determinant() const {
		int laplace = 1;
		matrix3f subMatrix;
		float det = 0;

		for (int i = 0; i < 4; i++) {
			
			det += laplace * m_values[i] * submatrix(0, i).determinant();
			laplace *= -1;
		}

		return det;
	}

	matrix3f matrix4f::submatrix(const unsigned& deletedRow, const unsigned& deletedColumn) const {
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

	matrix4f matrix4f::cofactor() const {
		matrix4f cofactor;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				matrix3f sub = submatrix(i, j);
				cofactor.m_values[to1D(i, j)] = pow(-1.0f, i + j) * sub.determinant();
			}
		}

		return cofactor;
	}

	matrix4f matrix4f::inverted() const {
		matrix4f mat(*this);
		mat.invert();

		return mat;
	}

	void matrix4f::invert() {
		float det = this->determinant();
		if (det == 0) return;

		matrix4f augmented = *this;
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
				for (int k = 0; k < 4; k++) {
					std::swap(augmented.m_values[k, i], augmented.m_values[to1D(k, pivotRow)]);
					std::swap(inverse.m_values[k, i], inverse.m_values[to1D(k, pivotRow)]);

				}
			}

			float pivotValue = augmented.m_values[to1D(i, i)];
			if (pivotValue == 0) return;

			float invPivotValue = 1.0f / pivotValue;
			
			// column iterator
			for (int k = 0; k < 4; k++) {
				augmented.m_values[to1D(k, i)] *= invPivotValue;
				inverse.m_values[to1D(k, i)] *= invPivotValue;
			}

			
			//row iterator
			for (int j = 0; j < 4; j++) {
				if (j == i) continue;
				float factor = augmented.m_values[to1D(i, j)];
				
				// column iterator
				for (int k = 0; k < 4; k++) {
					augmented.m_values[to1D(k, j)] -= factor * augmented.m_values[to1D(k, i)];
					inverse.m_values[to1D(k, j)] -= factor * inverse.m_values[to1D(k, i)];
				}
			}
		}

		*this = inverse;
	}

	matrix4f matrix4f::transposed() const {
		matrix4f mat(*this);
		mat.transpose();

		return mat;
	}

	void matrix4f::transpose() {
		matrix4f temp;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp.m_values[to1D(i, j)] = m_values[to1D(j, i)];
			}
		}

		*this = temp;
	}

	vec4f matrix4f::getColumn(const unsigned& column) const {
		vec4f v;
		v.x = m_values[to1D(column, 0)];
		v.y = m_values[to1D(column, 1)];
		v.z = m_values[to1D(column, 2)];
		v.w = m_values[to1D(column, 3)];

		return v;
	}

	vec4f matrix4f::getRow(const unsigned& row) const {
		vec4f v;
		v.x = m_values[to1D(0, row)];
		v.y = m_values[to1D(1, row)];
		v.z = m_values[to1D(2, row)];
		v.w = m_values[to1D(3, row)];

		return v;
	}

	vec3f matrix4f::getTranslation() const {
		return (vec3f)getColumn(3);
	}

	void matrix4f::setTranslation(const vec3f& position) {
		m_values[3] = position.x;
		m_values[7] = position.y;
		m_values[11] = position.z;
	}

	void matrix4f::translate(const vec3f& translation) {
		m_values[3] += translation.x;
		m_values[7] += translation.y;
		m_values[11] += translation.z;
	}

	matrix3f matrix4f::getRotationMatrix() const {
		return submatrix(3, 3);
	}

	quat matrix4f::getRotation() const {
		return quat(getRotationMatrix());
	}

	void matrix4f::setRotation(const quat& rotation) {
		setRotation(rotation.toMatrix3f());
	}

	void matrix4f::setRotation(const matrix3f& rotationMatrix) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m_values[to1D(i, j)] = rotationMatrix(i, j);
			}
		}
	}

	void matrix4f::rotate(const quat& rotation) {
		rotate(rotation.toMatrix3f());
	}

	void matrix4f::rotate(const matrix3f& rotationMatrix) {
		setRotation(rotationMatrix * getRotationMatrix());
	}

	
	float matrix4f::operator()(const unsigned& column, const unsigned& row) const {
		float v = m_values[to1D(column, row)];
		return v;
	}
	float& matrix4f::operator()(const unsigned& column, const unsigned& row) {
		return m_values[to1D(column, row)];
	}

	vec3f matrix4f::operator*(const vec3f& v) const {
		vec4f v4(v);
		return (vec3f) (*this * v4);
	}

	vec4f matrix4f::operator*(const vec4f& v) const {
		vec4f result;
		result.x = vec4f::dot(v, getColumn(0));
		result.y = vec4f::dot(v, getColumn(1));
		result.z = vec4f::dot(v, getColumn(2));
		result.w = vec4f::dot(v, getColumn(3));

		return result;
	}

	matrix4f matrix4f::operator+(const matrix4f& mat) const {
		matrix4f m(*this);
		for (int i = 0; i < 16; i++) {
			m.m_values[i] += mat.m_values[i];
		}

		return m;
	}

	matrix4f& matrix4f::operator+=(const matrix4f& mat) {
		for (int i = 0; i < 16; i++) {
			m_values[i] += mat.m_values[i];
		}

		return *this;
	}

	matrix4f matrix4f::operator-(const matrix4f& mat) const {
		matrix4f m(*this);
		for (int i = 0; i < 16; i++) {
			m.m_values[i] -= mat.m_values[i];
		}

		return m;
	}

	matrix4f& matrix4f::operator-=(const matrix4f& mat) {
		for (int i = 0; i < 16; i++) {
			m_values[i] -= mat.m_values[i];
		}
		matrix4f mul = *this * mat;
		return *this;
	}

	matrix4f matrix4f::operator*(const float& scalar) const {
		matrix4f m(*this);
		for (int i = 0; i < 16; i++) {
			m.m_values[i] *= scalar;
		}

		return m;
	}

	matrix4f& matrix4f::operator*=(const float& scalar) {
		for (int i = 0; i < 16; i++) {
			m_values[i] *= scalar;
		}

		return *this;
	}

	matrix4f matrix4f::operator*(const matrix4f& mat) const {
		matrix4f m;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m.m_values[to1D(i, j)] = vec4f::dot(this->getRow(j), mat.getColumn(i));
			}
		}

		return m;
	}

	matrix4f& matrix4f::operator*=(const matrix4f& mat) {
		matrix4f m;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m.m_values[to1D(i, j)] = vec4f::dot(this->getRow(j), mat.getColumn(i));
			}
		}

		*this = m;
		return *this;
	}

	bool matrix4f::operator==(const matrix4f& other) const {
		for (int i = 0; i < 16; i++) {
			if (m_values[i] != other.m_values[i])
				return false;
		}
		return true;
	}

	bool matrix4f::operator!=(const matrix4f& other) const {
		return !(*this == other);
	}

	unsigned matrix4f::to1D(const unsigned& column, const unsigned& row) {
		return (column * 4) + row;
	}
}