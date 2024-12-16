#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/material.hpp>

namespace sogl {
	typedef class materialFactory {
	static hashTable<material> LoadedMaterials;
	
	public:
		static material* createNew(const char* alias, shaderProgram* shader, cullingMode cullMode);
		static bool find(const char* alias, material*& outMaterial);
	} materialFactory;
}