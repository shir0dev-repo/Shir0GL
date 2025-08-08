#include <GLEW/glew.h>

#include <sogl/transform/vec3f.hpp>
#include <sogl/transform/vec4f.hpp>
#include <sogl/transform/matrix3f.hpp>
#include <sogl/transform/matrix4f.hpp>
#include <sogl/rendering/glUtilities.h>
#include <sogl/rendering/gl/GLUniform.h>

namespace sogl {
	void GLUniform::FindUploadMethod() {
		switch (type) {
			case GL_FLOAT:
				uploadMethod = GLUniform::UploadFloat;
				break;
			case GL_INT:
				uploadMethod = &GLUniform::UploadBool;
				break;
			case GL_FLOAT_VEC3:
				uploadMethod = &GLUniform::UploadVec3f;
				break;
			case GL_FLOAT_VEC4:
				uploadMethod = &GLUniform::UploadVec4f;
				break;
			case GL_FLOAT_MAT3:
				uploadMethod = &GLUniform::UploadMatrix3f;
				break;
			case GL_FLOAT_MAT4:
				uploadMethod = &GLUniform::UploadMatrix4f;
				break;
			default:
				
				printf("[%s]: Failed to assign uniform \"%s\" an upload method.\n", glGetNamedType(type), name);
				printf("|-- Uniform type \"%lu\" is unsupported!\n", type);
				uploadMethod = nullptr;
				break;
		}
	}
	void GLUniform::UploadFloat(GLUniform* u, const void* data) {
		float* f = (float*)data;
		if (f) glUniform1f(u->location, *f);
	}
	void GLUniform::UploadBool(GLUniform* u, const void* data) {
		int* i = (int*)data;
		if (i) glUniform1i(u->location, *i);
	}
	void GLUniform::UploadVec3f(GLUniform* u, const void* data) {
		const vec3f* v = (vec3f*)data;
		if (v) glUniform3f(u->location, v->x, v->y, v->z);
	}
	void GLUniform::UploadVec4f(GLUniform* u, const void* data) {
		const vec4f* v = (vec4f*)data;
		if (v) glUniform4f(u->location, v->x, v->y, v->z, v->w);
	}
	void GLUniform::UploadMatrix3f(GLUniform* u, const void* data) {
		const matrix3f* m = (matrix3f*)data;
		if (m) glUniformMatrix3fv(u->location, 1, GL_FALSE, m->getPointer());
	}
	void GLUniform::UploadMatrix4f(GLUniform* u, const void* data) {
		const matrix4f* m = (matrix4f*)data;
		if (m) glUniformMatrix4fv(u->location, 1, GL_FALSE, m->getPointer());
	}
}