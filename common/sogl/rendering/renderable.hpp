#pragma once

// REQUIREMENTS:
#include <sogl/rendering/vertexArrayObject.hpp>
#include <sogl/transform/transform.hpp>

namespace sogl {
	struct texture;
	struct mesh;
	struct shaderProgram;
	struct camera;

	struct renderable {
		vertexArrayObject vertexAttributes;
		transform transform;

		shaderProgram* shader;
		texture* boundTexture;

		renderable(const mesh& mesh, const char* shaderID);
		~renderable();
		
		void render();
		void addTexture(const std::string& filePath);

		static void renderAll();		
	};
}