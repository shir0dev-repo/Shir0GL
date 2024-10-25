#include "pch.h"
#include "Matrix3f.h"
#include "vec3f.h"

namespace shir0GL {
	matrix3f::matrix3f() {
		m_values = new float[9];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				operator()(i, j) = i == j ? 1.0f : 0.0f;
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