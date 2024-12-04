#include <GLEW/glew.h>

#include <iostream>
#include <cstring>

#include <sogl/rendering/factories/uniformBufferFactory.hpp>
#include <sogl/rendering/factories/lightFactory.hpp>

namespace sogl {
	hashTable<light> lightFactory::LoadedLights(32);

	static int dirLightCount = 0;
	static int pointLightCount = 0;
	static int spotLightCount = 0;

	light* lightFactory::createDirectionalLight(const vec3f& direction, const vec4f& intensityDiffuse, const vec4f& intensitySpecular, const char* alias) {
		
		if (dirLightCount >= MAX_DIR_LIGHTS) {
			std::cout <<
				"[Light Manager]: Failed to create directional light!\n" <<
				"|-- Maximum number of directional lights reached! (" << MAX_DIR_LIGHTS << ")\n";
			return nullptr;
		}

		char* aliasUsed = nullptr;
		if (strcmp(alias, "") == 0) {
			static const int DNAME_SIZE = 17;

			if (dirLightCount > 9) {
				char name[DNAME_SIZE + 2];
				sprintf(name, "DirectionalLight%d", dirLightCount);
			}
			else if (dirLightCount > 0) {
				char name[DNAME_SIZE + 1];
				sprintf(name, "DirectionalLight%d", dirLightCount);
				aliasUsed = name;
			}
			else {
				aliasUsed = const_cast<char*>("DirectionalLight");
			}
		}
		

		light* dirLight = new light(direction, intensityDiffuse, intensitySpecular);
		dirLight->type = lightType::directional;
		dirLight->lightIndex = dirLightCount++;

		tryInsertIntoBuffer(dirLight);
		LoadedLights.insert(aliasUsed, dirLight);

		std::cout << "[Light Manager]: Successfully created directional light \"" << aliasUsed << "\"\n";
		return dirLight;
	}

	light* lightFactory::createPointLight(const vec3f& position, const vec4f& diffuse, const vec4f& specular, const vec3f& attenuation,
		const float& radius, const char* alias) {
		if (pointLightCount >= MAX_POINT_LIGHTS) {
			std::cout <<
				"[Light Manager]: Failed to create point light!\n" <<
				"|-- Maximum number of point lights reached! (" << MAX_POINT_LIGHTS << ")\n";
			return nullptr;
		}

		char* aliasUsed = nullptr;
		if (strcmp(alias, "") == 0) {
			static const int DNAME_SIZE = 11;

			if (pointLightCount > 9) {
				char name[DNAME_SIZE + 2];
				sprintf(name, "PointLight%d", pointLightCount);
			}
			else if (pointLightCount > 0) {
				char name[DNAME_SIZE + 1];
				sprintf(name, "PointLight%d", pointLightCount);
				aliasUsed = name;
			}
			else {
				aliasUsed = const_cast<char*>("PointLight");
			}
		}
		

		light* pointLight = new light(position, diffuse, specular, attenuation, radius);
		pointLight->type = lightType::point;
		pointLight->lightIndex = pointLightCount++;

		tryInsertIntoBuffer(pointLight);

		LoadedLights.insert(aliasUsed, pointLight);

		std::cout << "[Light Manager]: Successfully created point light \"" << aliasUsed << "\"\n";
		return pointLight;
	}

	light* lightFactory::createSpotLight(const vec3f& position, const vec3f& direction, const vec4f& diffuse, const vec4f& specular, const vec3f& attenuation,
		const float& innerAngleDegrees, const float& outerAngleDegrees, const char* alias)
	{
		if (spotLightCount >= MAX_SPOT_LIGHTS) {
			std::cout <<
				"[Light Manager]: Failed to create spot light!\n" <<
				"|-- Maximum number of spot lights reached! (" << MAX_DIR_LIGHTS << ")\n";
			return nullptr;
		}

		char* aliasUsed = nullptr;
		if (strcmp(alias, "") == 0) {
			static const int DNAME_SIZE = 17;
			
			if (spotLightCount > 9) {
				char name[DNAME_SIZE + 2];
				sprintf(name, "SpotLight%d", spotLightCount);
				aliasUsed = name;
			}
			else if (spotLightCount > 0) {
				char name[DNAME_SIZE + 1];
				sprintf(name, "SpotLight%d", spotLightCount);
				aliasUsed = name;
			}
			else {
				aliasUsed = const_cast<char*>("SpotLight");
			}
		}
		

		light* spotLight = new light(position, direction, diffuse, specular, attenuation, innerAngleDegrees, outerAngleDegrees);

		spotLight->type = lightType::spot;
		spotLight->lightIndex = spotLightCount++;

		tryInsertIntoBuffer(spotLight);

		LoadedLights.insert(aliasUsed, spotLight);

		std::cout << "[Light Manager]: Successfully created spot light \"" << aliasUsed << "\".\n";
		return spotLight;
	}

	void lightFactory::tryInsertIntoBuffer(light*& light) {
		static uniformBufferObject* lightBuffer = nullptr;
		static bool foundBuffer = false;
		
		if (lightBuffer == nullptr) {
			foundBuffer = uniformBufferFactory::find("Lights", lightBuffer);
		}

		if (foundBuffer == false) {
			std::cout <<
				"[Light Manager]: Failed to load light into buffer!\n" <<
				"|-- Failed to locate uniform light buffer object!\n";
			return;
		}

		const void* lightPtr = light;
		uint64_t size = 0;
		uint64_t offset = 0;

		if (light->type == lightType::directional) {
			size = DIR_LIGHT_SIZE;
			offset = 0;
		}
		else if (light->type == lightType::point) {
			size = POINT_LIGHT_SIZE;
			offset = (DIR_LIGHT_SIZE * MAX_DIR_LIGHTS) + (POINT_LIGHT_SIZE * light->lightIndex);
		}
		else if (light->type == lightType::spot) {
			size = SPOT_LIGHT_SIZE;
			offset = DIR_LIGHT_SIZE + (POINT_LIGHT_SIZE * MAX_POINT_LIGHTS) + (SPOT_LIGHT_SIZE * light->lightIndex);
		}
		else {
			throw;
		}

		lightBuffer->bufferData(light, size, offset);
	}
	
	void lightFactory::updateLightBuffer(light* l) {
		if (l->type == lightType::NONE) return;
		
		uniformBufferObject* lightBuffer = nullptr;
		if (!uniformBufferFactory::find("Lights", lightBuffer)) {
			std::cout << "falfdkawlda\n";
		}

		void* lightPtr = l;
		uint64_t size = 0;
		uint64_t offset = 0;
		if (l->type == lightType::directional) {
			size = DIR_LIGHT_SIZE;
			offset = DIR_LIGHT_SIZE * l->lightIndex;
			lightBuffer->bufferData(lightPtr, size, offset);
		}
		else if (l->type == lightType::point) {
			size = POINT_LIGHT_SIZE;
			offset = (DIR_LIGHT_SIZE * MAX_DIR_LIGHTS) + (POINT_LIGHT_SIZE * l->lightIndex);
			lightBuffer->bufferData(lightPtr, size, offset);
		}
		else if (l->type == lightType::spot) {
			size = SPOT_LIGHT_SIZE;
			offset = (DIR_LIGHT_SIZE * MAX_DIR_LIGHTS) + (POINT_LIGHT_SIZE * MAX_POINT_LIGHTS) + (SPOT_LIGHT_SIZE * l->lightIndex);
			lightBuffer->bufferData(lightPtr, size, offset);
		}
	}

	void lightFactory::terminate() {
		for (uint64_t i = 0; i < LoadedLights.size; i++) {
			light* l = nullptr;
			if ((l = LoadedLights.data[i].value) == nullptr) {
				continue;
			}

			LoadedLights.remove(LoadedLights.data[i].key);
		}
	}
}