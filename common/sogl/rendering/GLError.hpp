#include <GLEW/glew.h>

#include <string>
#include <vector>

namespace sogl {

	typedef struct GLError {
	private:
		GLenum m_code;
		std::string m_what;
	public:
		inline GLError(const GLenum& code) {
			m_code = code;
			switch (code) {
				case GL_INVALID_ENUM:
					m_what = "INVALID ENUM";
					break;
				case GL_INVALID_VALUE:
					m_what = "INVALID VALUE";
					break;
				case GL_INVALID_OPERATION:
					m_what = "INVALID OPERATION";
					break;
				case GL_STACK_OVERFLOW:
					m_what = "STACK OVERFLOW";
					break;
				case GL_STACK_UNDERFLOW:
					m_what = "STACK UNDERFLOW";
					break;
				case GL_OUT_OF_MEMORY:
					m_what = "OUT OF MEMORY";
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					m_what = "INVALID FRAME BUFFER OPERATION";
				default:
					m_what = "UNDEFINED ERROR";
			}
		}

		bool good() const { return m_code == 0; }

		GLuint code() const { return m_code; }
		const char* what() const { return m_what.c_str(); }
	} GLError;
}