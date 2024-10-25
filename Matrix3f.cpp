#include "pch.h"
#include "Matrix3f.h"
#include "vec3f.h"

namespace shir0GL {
	matrix3f::matrix3f() {
		m_values = new float[9];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m_values[to1D(i, j)] = i == j ? 1.0f : 0.0f;
			}
		}
	}

	matrix3f::matrix3f(const matrix3f& mat) : matrix3f() {
		*this = mat;
	}

	matrix3f& matrix3f::operator=(const matrix3f& mat) {
		for (int i = 0; i < 9; i++) {
			m_values[i] = mat.m_values[i];
		}

		return *this;
	}

	matrix3f::~matrix3f() {
		delete[] m_values;
		m_values = nullptr;
	}

	float* const matrix3f::getPointer() {
		return &m_values[0];
	}

	float matrix3f::determinant() const {
		float det = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {

			}
		}
	}
	
	matrix3f matrix3f::transposed() const {
		matrix3f m(*this);
		m.transpose();

		return m;
	}
	void matrix3f::transpose() {
		matrix3f temp;

		for (int n = 0; n < 9; n++) {
			int i = n / 3;
			int j = n % 3;
			temp.m_values[n] = m_values[j * 3 + i];
		}

		*this = temp;
	}

	matrix3f matrix3f::inverted() const {

	}
	void matrix3f::invert() {

	}

	vec3f matrix3f::getColumn(const unsigned& col) const {
		vec3f v;
		v.x = m_values[to1D(col, 0)];
		v.y = m_values[to1D(col, 1)];
		v.z = m_values[to1D(col, 2)];

		return v;
	}
	vec3f matrix3f::getRow(const unsigned& row) const {
		vec3f v;
		v.x = m_values[to1D(0, row)];
		v.y = m_values[to1D(1, row)];
		v.z = m_values[to1D(2, row)];

		return v;
	}

	vec3f matrix3f::operator*(const vec3f& v) const {
		vec3f temp;
		temp.x = v.x * m_values[to1D(0, 0)] + v.y * m_values[to1D(0, 1)] + v.z * m_values[to1D(0, 2)];
		temp.y = v.x * m_values[to1D(1, 0)] + v.y * m_values[to1D(1, 1)] + v.z * m_values[to1D(1, 2)];
		temp.z = v.x * m_values[to1D(2, 0)] + v.y * m_values[to1D(2, 1)] + v.z * m_values[to1D(2, 2)];

		return temp;
	}

	matrix3f matrix3f::operator+(const matrix3f& mat) const {
		matrix3f m(*this);
		for (int i = 0; i < 9; i++) {
			m.m_values[i] += mat.m_values[i];
		}

		return m;
	}
	matrix3f& matrix3f::operator+=(const matrix3f& mat) {		
		for (int i = 0; i < 9; i++) {
			m_values[i] += mat.m_values[i];
		}

		return *this;
	}

	matrix3f matrix3f::operator-(const matrix3f& mat) const {
		matrix3f m(*this);
		for (int i = 0; i < 9; i++) {
			m.m_values[i] -= mat.m_values[i];
		}

		return m;
	}
	matrix3f& matrix3f::operator-=(const matrix3f& mat) {
		for (int i = 0; i < 9; i++) {
			m_values[i] -= mat.m_values[i];
		}

		return *this;
	}

	matrix3f matrix3f::operator*(const matrix3f& mat) const {

	}
	matrix3f& matrix3f::operator*=(const matrix3f& mat) {

	}

	matrix3f matrix3f::operator*(const float& scalar) const {
	
	}
	matrix3f& matrix3f::operator*=(const float& scalar) {

	}

	bool matrix3f::operator==(const matrix3f& other) const {
		for (int i = 0; i < 9; i++) {
			if (m_values[i] != other.m_values[i])
				return false;
		}

		return true;
	}
	bool matrix3f::operator!=(const matrix3f& other) const {
		return !(*this == other);
	}

	float matrix3f::operator() (const unsigned& column, const unsigned& row) const {
		float v = m_values[(column * 3) + row];
		return v;
	}
	float& matrix3f::operator() (const unsigned& column, const unsigned& row) {
		return m_values[(column * 3) + row];
	}

	unsigned matrix3f::to1D(const unsigned& col, const unsigned& row) {
		return (col * 3) + row;
	}
}