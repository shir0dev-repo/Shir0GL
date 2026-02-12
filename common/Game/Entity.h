#pragma once

#include "sogl/transform/transform.hpp"

struct Entity {
	Entity();
	~Entity();

	inline sogl::transform const* getTransform() const { return Transform; }

	virtual void Update(const float& deltaTime);

protected:
	sogl::transform* Transform;
};