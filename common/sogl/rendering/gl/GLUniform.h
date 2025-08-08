#pragma once

#include <stdint.h>

namespace sogl {
	typedef void (*uniformUploadMethod)(struct GLUniform* uniform, const void* value);

	typedef struct GLUniform {
		char* name;
		char* nameEnd;
		uint32_t type;
		int32_t size;
		int32_t location;

		void FindUploadMethod();

		static void UploadFloat(GLUniform* u, const void* data);
		static void UploadBool(GLUniform* u, const void* data);
		static void UploadVec3f(GLUniform* u, const void* data);
		static void UploadVec4f(GLUniform* u, const void* data);
		static void UploadMatrix3f(GLUniform* u, const void* data);
		static void UploadMatrix4f(GLUniform* u, const void* data);

		uniformUploadMethod uploadMethod;
	} Uniform;
}