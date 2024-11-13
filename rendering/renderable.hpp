#pragma once

namespace sogl {
	struct vertexArrayObject;
	struct vertexBufferObject;

	struct renderable {
		vertexArrayObject* vertexAttributes;
		vertexBufferObject* vertexData;


	};
}