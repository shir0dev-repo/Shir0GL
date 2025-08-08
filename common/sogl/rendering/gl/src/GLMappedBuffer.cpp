#include <GLEW/glew.h>

#include <sogl/rendering/gl/GLMappedBuffer.h>

namespace sogl {
	GLMappedBuffer::GLMappedBuffer(const uint32_t size, const uint32_t flags, const void* data) : syncObj() {
		this->flags = flags | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		this->size = size;
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferStorage(GL_ARRAY_BUFFER, size, data, flags);
		this->pointer = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, flags);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLMappedBuffer::update() {
		GLenum waitReturn = GL_UNSIGNALED;
		while (waitReturn != GL_ALREADY_SIGNALED && waitReturn != GL_CONDITION_SATISFIED) {
			waitReturn = glClientWaitSync(syncObj, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
		}

		glDeleteSync(syncObj);
		syncObj = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
}