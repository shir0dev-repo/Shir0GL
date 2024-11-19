#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <sogl/database.hpp>
#include <sogl/transform/transform.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/shaderUtilities.hpp>
#include <sogl/rendering/texture.hpp>
#include <sogl/rendering/camera.hpp>
#include <sogl/rendering/vertexArrayObject.hpp>
#include <sogl/rendering/renderable.hpp>
#include <sogl/rendering/mesh.hpp>
#include <sogl/rendering/shaderProgram.hpp>

namespace sogl {
	static std::vector<renderable> Renderables;

	renderable::renderable(const mesh& mesh, const char* shaderName) : vertexAttributes(mesh) {
		this->boundTexture = nullptr;
		this->shader = findShader(shaderName);
		this->transform = sogl::transform();
	}

	void renderable::render() {
		camera* cam = getRenderCamera();
		shader->uploadUniformMatrix4f(cam->projectionMatrix, false, "u_projectionMatrix");
		shader->uploadUniformMatrix4f(cam->viewMatrix, false, "u_viewMatrix");
		shader->uploadUniformMatrix4f(transform.getTransformationMatrix(), false, "u_transformationMatrix");

		if (boundTexture) {
			boundTexture->bind();
		}
		
		vertexAttributes.bind();
		int count = vertexAttributes.pointCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
		vertexAttributes.unbind();

		if (boundTexture) {
			boundTexture->unbind();
		}

		shader->stop();
	}

	void renderable::addTexture(const std::string& filePath) {
		if (boundTexture) {
			delete boundTexture;
			boundTexture = nullptr;
		}
		boundTexture = new texture(filePath.c_str());
	}

	void renderable::renderAll() {
		for (renderable r : Renderables)
			r.render();
	}
}