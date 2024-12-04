#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <sogl/rendering/mesh.hpp>
#include <sogl/rendering/gl/vertexArrayObject.hpp>

namespace sogl {
	vertexBufferObject::vertexBufferObject() : size(0), target(0), ID(0) {}
	vertexBufferObject::vertexBufferObject(const unsigned int& size, const void* data, const unsigned int& target) 
	{
		this->size = size;
		this->target = target;
		glGenBuffers(1, &ID);
		glBindBuffer(target, ID);
		glBufferData(target, size, data, GL_STATIC_DRAW);
	}

	void vertexBufferObject::bind() const {
		glBindBuffer(target, ID);
	}

	void vertexBufferObject::unbind() const {
		glBindBuffer(target, 0);
	}

	vertexArrayObject::vertexArrayObject(const mesh& mesh) {		
		glGenVertexArrays(1, &this->ID);
		glBindVertexArray(this->ID);

		const unsigned int target = GL_ARRAY_BUFFER;

		positions = vertexBufferObject(mesh.size, mesh.positions, target);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		positions.unbind();

		texCoords = vertexBufferObject(mesh.size, mesh.texCoords, target);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		texCoords.unbind();

		normals = vertexBufferObject(mesh.size, mesh.normals, target);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		normals.unbind();

		this->pointCount = mesh.indicesSize / sizeof(unsigned int);
		indices = vertexBufferObject(mesh.indicesSize, mesh.indices, GL_ELEMENT_ARRAY_BUFFER);
		indices.unbind();

		glBindVertexArray(0);
	}

	void vertexArrayObject::bind() const {
		glBindVertexArray(this->ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.ID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

	void vertexArrayObject::unbind() const {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}