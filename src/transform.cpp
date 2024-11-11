#include "matrix4f.h"
#include "vec3f.h"
#include "quat.h"
#include "matrix3f.h"

#include "transform.h"

namespace sogl {
	transform::transform() : m_transformationMatrix() { }
	
	transform::transform(const vec3f& position) : m_transformationMatrix() {
		m_transformationMatrix->setTranslation(position);
	}

	transform::transform(const vec3f& position, const quat& rotation) : transform(position) {
		m_transformationMatrix->setRotation(rotation);
	}

	transform::transform(const transform& other) {
		*m_transformationMatrix = matrix4f(*other.m_transformationMatrix);
	}

	transform& transform::operator=(const transform& other) {
		this->m_transformationMatrix = other.m_transformationMatrix;

		return *this;
	}

	transform::~transform() {
		delete m_transformationMatrix;
		m_transformationMatrix = nullptr;
	}

	matrix4f transform::getTransformationMatrix() const {
		return matrix4f(*m_transformationMatrix);
	}

	matrix3f transform::getRotationMatrix() const {
		return matrix3f(m_transformationMatrix->getRotationMatrix());
	}

	vec3f transform::getPosition() const {
		return m_transformationMatrix->getTranslation();
	}

	void transform::setPosition(const vec3f& position) {
		m_transformationMatrix->setTranslation(position);
	}

	void transform::translate(const vec3f& deltaPosition) {
		m_transformationMatrix->translate(deltaPosition);
	}

	quat transform::getRotation() const {
		return m_transformationMatrix->getRotation();
	}

	vec3f transform::getEulerRotation() const {
		return m_transformationMatrix->getRotation().toEulerAngles();
	}

	void transform::setRotation(const quat& rotation) {
		m_transformationMatrix->setRotation(rotation);
	}

	void transform::setRotation(const vec3f& eulerAngles) {
		m_transformationMatrix->setRotation(quat(eulerAngles));
	}

	void transform::setRotation(const matrix3f& rotationMatrix) {
		m_transformationMatrix->setRotation(rotationMatrix);
	}

	void transform::rotate(const quat& deltaRotation) {
		m_transformationMatrix->rotate(deltaRotation);
	}

	void transform::rotate(const vec3f& deltaEuler) {
		m_transformationMatrix->rotate(quat(deltaEuler));
	}

	bool transform::operator==(const transform& other) const {
		return m_transformationMatrix == other.m_transformationMatrix;
	}

	bool transform::operator!=(const transform& other) const {
		return !(*this == other);
	}
}