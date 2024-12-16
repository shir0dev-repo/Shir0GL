#pragma once

namespace sogl {
	typedef class chunkMesh {
		typedef enum blockFaceType : uint8_t {
			LEFT,
			RIGHT,
			TOP,
			BOTTOM,
			FRONT,
			BACK
		};

		struct mesh* meshData;

		void addFace(chunk* chunkData, const blockFaceType faceType, const struct vec3f& position, )
	public:
		chunkMesh(struct chunk& chunkData);
	};
}