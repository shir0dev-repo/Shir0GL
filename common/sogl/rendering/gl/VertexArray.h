#pragma once

#include <stdint.h>
#include <sogl/rendering/gl/GLBuffer.h>

namespace sogl {
	struct VertexArray {
		GLBuffer positions;
		GLBuffer texCoords;
		GLBuffer normals;
		GLBuffer indices;

		uint32_t ID;
		uint32_t pointCount;
		
		VertexArray() = default;
		VertexArray(const struct Mesh& Mesh);

		void bind() const;
		void unbind() const;
	};
}