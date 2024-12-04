#pragma once

#include <sogl/hashTable.hpp>
#include <sogl/transform/vec3f.hpp>
#include <sogl/rendering/light.hpp>

namespace sogl {
	typedef class lightFactory {
	private:
		static hashTable<light> LoadedLights;

		static void tryInsertIntoBuffer(light*& light);
	public:
		/// <summary>
		/// <para>Creates a directional light and uploads it to the buffer.</para>
		/// <para>Defaulted name will be "DirectionalLight" with the current count of directional lights appended:</para>
		/// <para>"DirectionalLight", "DirectionalLight0", etc.</para>
		/// </summary>		
		static light* createDirectionalLight(
			const vec3f& posOrDir, 
			const vec4f& diffuse,
			const vec4f& specular = vec3f::ONE * 0.5f,
			const char* alias = ""
		);
		
		/// <summary>
		/// <para>Creates a point light and uploads it to the buffer.</para>
		/// <para>Defaulted name will be "PointLight" with the current count of point lights - 1 appended:</para>
		/// <para>"PointLight", "PointLight0", etc.</para>
		/// </summary>
		static light* createPointLight(
			const vec3f& position,
			const vec4f& diffuse,
			const vec4f& specular,
			const vec3f& attenuation,
			const float& radius,
			const char* alias = ""
		);

		static light* createSpotLight(
			const vec3f& position,
			const vec3f& direction,
			const vec4f& diffuse,
			const vec4f& specular,
			const vec3f& attenuation ,
			const float& innerAngleDegrees,
			const float& outerAngleDegrees,
			const char* alias = ""
		);

		static void updateLightBuffer(light* l);
		static bool findLight(const char* alias, light*& outLight);
		static void terminate();
	} lightFactory;
}