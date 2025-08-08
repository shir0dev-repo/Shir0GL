#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <sogl/rendering/gl/mesh/Mesh.h>
#include <sogl/rendering/gl/VertexArray.h>
#include <sogl/rendering/gl/GLBuffer.h>

namespace sogl {
	VertexArray::VertexArray(const Mesh& Mesh) {
		glGenVertexArrays(1, &this->ID);
		glBindVertexArray(this->ID);

		const unsigned int target = GL_ARRAY_BUFFER;

		positions = GLBuffer(Mesh.VerticesSize(), Mesh.Vertices(), target);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		positions.unbind();

		texCoords = GLBuffer(Mesh.VerticesSize(), Mesh.TexCoords(), target);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		texCoords.unbind();

		normals = GLBuffer(Mesh.VerticesSize(), Mesh.Normals(), target);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		normals.unbind();

		this->pointCount = Mesh.IndexCount();
		indices = GLBuffer(Mesh.IndicesSize(), Mesh.Indices(), GL_ELEMENT_ARRAY_BUFFER);
		indices.unbind();

		glBindVertexArray(0);
	}

	void VertexArray::bind() const {
		glBindVertexArray(this->ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.ID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

	void VertexArray::unbind() const {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}