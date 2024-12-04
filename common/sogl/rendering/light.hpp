#pragma once

#include <sogl/transform/matrix4f.hpp>
#include <sogl/rendering/color.hpp>
/*
This struct will be casted to void pointer and sent to the shader as one of the following:

struct PointLight {
	vec3 position;

	vec3 intensityDiffuse;
	vec3 intensitySpecular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight {
	vec4 direction;

	vec4 intensityDiffuse;
	vec4 intensitySpecular;
};

*/
namespace sogl {
	constexpr auto DIR_LIGHT_SIZE = sizeof(vec4f) * 4;
	constexpr auto MAX_DIR_LIGHTS = 1;

	constexpr auto POINT_LIGHT_SIZE = (sizeof(vec4f) * 5);
	constexpr auto MAX_POINT_LIGHTS = 4;
	
	constexpr auto SPOT_LIGHT_SIZE = (sizeof(vec4f) * 7);
	constexpr auto MAX_SPOT_LIGHTS = 1;

	typedef enum lightType { NONE, directional, point, spot };

	struct alignas(16) light {
		vec4f positionOrDirection;			// For point lights, this will be position. For directional lights: direction.
		vec4f diffuse;						// Intensity is stored in the w-component.
		vec4f specular;						// Intensity is stored in the w-component.
		int32_t isActive;					// Although not ideal, this is placed above attenuation to allow dir lights to use it as well.
	private:
		vec3f padding;						// Required for std140 layout.
		/// ---DIRECTIONAL_LIGHT END---
	public:
		vec4f attenuation;					// Constant, linear, and quadratic attenuation is stored in the xyz-components respectively. W-component is radius.
		/// ---POINT LIGHT END---

		float innerAngle;					// The cosine of the angle at which fragments will be illuminated.
		float outerAngle;					// The cosine of the angle at which light will start to dissipate.
		float padding2[2]{ 0 };
		vec4f spotlightDirection;			// The direction the spot light is pointing.
		
		// Anything specified after this point will not be inserted into the buffer.

		lightType type = lightType::NONE;
		uint32_t lightIndex = 0;
		light();
		/// <summary>Constructor for directional lights.</summary>
		light(const vec3f& direction, const vec4f& diffuse, const vec4f& specular);
		/// <summary>Constructor for point lights.</summary>
		light(const vec3f& position, const vec4f& diffuse, const vec4f& specular, const vec3f& attenuation, const float& radius);
		///<summary>Constructor for spot lights.</summary>
		light(const vec3f& position, const vec3f& spotlightDirection, const vec4f& diffuse, const vec4f& specular, const vec3f& attenuation,
			const float& innerAngle, const float& outerAngle);
	};
}