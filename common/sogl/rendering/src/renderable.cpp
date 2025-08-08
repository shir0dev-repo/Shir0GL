#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <sogl/structure/hashTable.hpp>
#include <sogl/transform/transform.hpp>
#include <sogl/rendering/glUtilities.h>
#include <sogl/rendering/Material.h>
#include <sogl/rendering/Texture.h>
#include <sogl/rendering/camera.hpp>
#include <sogl/rendering/gl/mesh/Mesh.h>
#include <sogl/rendering/gl/VertexArray.h>
#include <sogl/rendering/gl/shaderProgram.h>
#include <sogl/rendering/renderable.hpp>
#include <sogl/rendering/factories/uniformBufferFactory.hpp>

namespace sogl {
	renderable::renderable(const Mesh& Mesh, Material* mat) : vertexAttributes(Mesh) {
		this->boundTexture = nullptr;
		this->currentMaterial = mat;
		this->transform = sogl::transform();
	}

	renderable::~renderable() {
		if (boundTexture != nullptr) {
			delete boundTexture;
		}

		boundTexture = nullptr;
	}

	void renderable::render() const {
		currentMaterial->bind();
		currentMaterial->prepare();
		currentMaterial->uploadUniformData("u_transformationMatrix", transform.getTransformationMatrix().getPointer());

		vertexAttributes.bind();
		int count = vertexAttributes.pointCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
		vertexAttributes.unbind();

		if (boundTexture) {
			boundTexture->unbind();
		}

		currentMaterial->unbind();
	}

	void renderable::addTexture(const std::string& filePath) {
		if (boundTexture) {
			delete boundTexture;
			boundTexture = nullptr;
		}
		boundTexture = new Texture(filePath.c_str());
	}

	void renderable::renderAll() {

	}
}