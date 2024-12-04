#include <GLEW/glew.h>


#include <sogl/transform/transform.hpp>
#include <sogl/rendering/light.hpp>

namespace sogl {
		light::light() {
		this->positionOrDirection = vec3f::ZERO;
		this->positionOrDirection.w = 1;
		this->diffuse = vec4f(1, 1, 1, 0.5f);
		this->specular = vec4f(1, 1, 1, 0.5f);
		this->isActive = 1;
		this->attenuation = vec4f(1, 0, 0, 1);
		this->innerAngle = 0;
		this->outerAngle = 0;
	}

	light::light(const vec3f& posOrDir, const vec4f& diffuse, const vec4f& specular) {
		this->positionOrDirection = posOrDir;
		this->positionOrDirection.w = 0;
		this->diffuse = diffuse;
		this->specular = specular;
		this->isActive = 1;
		this->attenuation = vec3f(1, 0, 0);
		this->innerAngle = 0;
		this->outerAngle = 0;
	}

	light::light(const vec3f& posOrDir, const vec4f& diffuse, const vec4f& specular, const vec3f& attenuation, const float& radius) {
		this->positionOrDirection = posOrDir;
		this->positionOrDirection.w = 0;
		this->diffuse = diffuse;
		this->specular = specular;
		this->isActive = 1;
		this->attenuation = attenuation;
		this->attenuation.w = radius;
		this->innerAngle = 0;
		this->outerAngle = 0;
	}

	light::light(const vec3f& position, const vec3f& spotlightDirection, const vec4f& diffuse, const vec4f& specular, const vec3f& attenuation,
		const float& innerAngle, const float& outerAngle) {
		this->positionOrDirection = position;
		this->positionOrDirection.w = 0;
		this->diffuse = diffuse;
		this->specular = specular;
		this->isActive = 1;
		this->attenuation = attenuation;
		this->innerAngle = cosf(DEG2RAD * innerAngle);
		this->outerAngle = cosf(DEG2RAD * outerAngle);
		this->spotlightDirection = spotlightDirection;
	}
}