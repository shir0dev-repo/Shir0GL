#pragma once

// REQUIREMENTS:
#include <sogl/rendering/gl/VertexArray.h>
#include <sogl/transform/transform.hpp>

namespace sogl {
	struct Texture;
	struct Mesh;
	struct camera;
	struct Material;

	struct renderable {
		VertexArray vertexAttributes;
		transform transform;
		Material* currentMaterial;
		Texture* boundTexture;

		renderable(const Mesh& Mesh, Material* mat);
		~renderable();
		void render() const;
		void addTexture(const std::string& filePath);

		static void renderAll();
	};

	namespace renderableManager {
		
	}
}