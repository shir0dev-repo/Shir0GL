#pragma once

// REQUIREMENTS:
#include <sogl/rendering/gl/vertexArrayObject.hpp>
#include <sogl/transform/transform.hpp>

namespace sogl {
	struct texture;
	struct mesh;
	struct camera;
	struct material;

	struct renderable {
		vertexArrayObject vertexAttributes;
		transform transform;
		material* currentMaterial;
		texture* boundTexture;

		renderable(const mesh& mesh, material* mat);
		~renderable();
		void render() const;
		void addTexture(const std::string& filePath);

		static void renderAll();
	};

	namespace renderableManager {
		
	}
}