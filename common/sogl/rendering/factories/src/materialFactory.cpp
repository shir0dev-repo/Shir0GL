#include <GLEW/glew.h>

#include <sogl/rendering/gl/shaderProgram.hpp>
#include <sogl/rendering/factories/materialFactory.hpp>

namespace sogl {
	hashTable<material> materialFactory::LoadedMaterials(64);

	material* materialFactory::createNew(const char* alias, shaderProgram* shader, cullingMode cullMode) {
		return nullptr;
	}
}