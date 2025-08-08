#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include <sogl/rendering/glUtilities.h>
#include <sogl/rendering/factories/ShaderFactory.h>
#include <sogl/rendering/factories/uniformBufferFactory.hpp>

namespace sogl {
	hashTable<unsigned int> ShaderFactory::m_vertexShaders(32);
	hashTable<unsigned int> ShaderFactory::m_fragmentShaders(32);
	hashTable<ShaderProgram> ShaderFactory::m_loadedShaders(32);

	ShaderProgram* ShaderFactory::createNew(const char* vertexFilePath, const char* fragmentFilePath, const char* alias) {
		char* aliasUsed;
		ShaderProgram* shader = nullptr;

		if (strcmp(alias, "") == 0) {
			aliasUsed = const_cast<char*>(vertexFilePath);
		}
		else {
			aliasUsed = const_cast<char*>(alias);
		}

		if (m_loadedShaders.find(aliasUsed, shader)) {
			return shader;
		}

		unsigned int vertexSourceID = createShaderSource(vertexFilePath, GL_VERTEX_SHADER);
		unsigned int fragmentSourceID = createShaderSource(fragmentFilePath, GL_FRAGMENT_SHADER);

		if (vertexSourceID == 0 || fragmentSourceID == 0) {			
			std::cout <<
				"[Shader Manager]: Failed to create shader " << aliasUsed << '\n' <<
				"|-- One or both source files failed to compile!\n";
			return nullptr;
		}

		// both shaders compiled, create program and attach shaders
		shader = new ShaderProgram();
		shader->programID = glCreateProgram();
		shader->vertexShaderID = vertexSourceID;
		shader->fragmentShaderID = fragmentSourceID;
		
		glAttachShader(shader->programID, shader->vertexShaderID);		
		glAttachShader(shader->programID, shader->fragmentShaderID);
		glLinkProgram(shader->programID);
		// always detach shaders after linking, even if successful
		// when the program ends, if openGL thinks any programs are still attached, calling glDeleteShader(vertex/frag) will only queue the deletion.
		glDetachShader(shader->programID, shader->vertexShaderID);
		glDetachShader(shader->programID, shader->fragmentShaderID);

		// check if program is linked correctly
		int isLinked = 0;
		glGetProgramiv(shader->programID, GL_LINK_STATUS, &isLinked);

		if (isLinked == GL_FALSE) {
			std::cout <<
				"[Shader Manager]: Failed to create shader program " << aliasUsed << "!\n" <<
				"|-- Shader program did not successfully link! See details below:\n";

			int logLength = 0;
			glGetProgramiv(shader->programID, GL_INFO_LOG_LENGTH, &logLength);

			char* programOutput = new char[logLength];
			glGetProgramInfoLog(shader->programID, logLength, &logLength, programOutput);
			std::cout << programOutput << '\n';
			delete[] programOutput;

			glDeleteProgram(shader->programID);
			shader->programID = 0;
			shader->vertexShaderID = 0;
			shader->fragmentShaderID = 0;
			
			delete shader;
			return nullptr;
		}
		
		// program successfully linked
		m_loadedShaders.insert(aliasUsed, shader);
		
		// collect all unique uniform blocks within shader
		int numUniformBlocks = 0;
		glGetProgramiv(shader->programID, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
		if (numUniformBlocks <= 0) {
			// this program has no uniform blocks
			return shader;
		}

		char* currentBlockName = nullptr;
		for (int currentBlockIndex = 0; currentBlockIndex < numUniformBlocks; currentBlockIndex++) {
			int nameLength = 0;
			glGetActiveUniformBlockiv(shader->programID, currentBlockIndex, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLength);
			currentBlockName = new char[nameLength];
			glGetActiveUniformBlockName(shader->programID, currentBlockIndex, nameLength, NULL, currentBlockName);
			
			UniformBuffer* ubo = nullptr;
			//this ubo has previously been named, bind it to program
			if (uniformBufferFactory::find(currentBlockName, ubo)) {
				glUniformBlockBinding(shader->programID, currentBlockIndex, ubo->bindingIndex);
				// we only delete if the buffer already exists, since deleting it after creating it makes the UBO's name in the table null.
				delete[] currentBlockName;
			}
			// haven't encountered this buffer yet, add it to the list
			else {
				int32_t bufferSize = 0;
				glGetActiveUniformBlockiv(shader->programID, currentBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &bufferSize);
				ubo = uniformBufferFactory::createNew(currentBlockName, bufferSize);
				glUniformBlockBinding(shader->programID, currentBlockIndex, ubo->bindingIndex);
			}
		}

		return shader;
	}

	bool ShaderFactory::find(const char* alias, ShaderProgram*& outShader) {
		return m_loadedShaders.find(alias, outShader);
	}

	const char* ShaderFactory::GetShaderVertexName(const uint32_t vertexID) {
		uint32_t* ID = nullptr;
		for (uint32_t i = 0; i < m_vertexShaders.size; i++) {
			if ((ID = m_vertexShaders.data[i].value) != nullptr && *ID == vertexID) {
				
				return m_vertexShaders.data[i].key;
			}
		}

		return "";
	}

	const char* ShaderFactory::GetShaderFragmentName(const uint32_t fragmentID) {
		uint32_t* ID = nullptr;
		for (uint32_t i = 0; i < m_fragmentShaders.size; i++) {
			if ((ID = m_fragmentShaders.data[i].value) != nullptr && *ID == fragmentID) {
				return m_fragmentShaders.data[i].key;
			}
		}

		return "";
	}

	const char* ShaderFactory::GetShaderName(const ShaderProgram* shader) {
		char* alias = nullptr;
		for (uint32_t i = 0; i < m_loadedShaders.size; i++) {
			if ((alias = m_loadedShaders.data[i].key) == nullptr)
				continue;

			ShaderProgram* program = m_loadedShaders.data[i].value;
			if (program == shader) {
				return alias;
			}
		}

		return "";
	}

	unsigned int ShaderFactory::createShaderSource(const char* filePath, const unsigned int sourceType) {
		if (sourceType != GL_VERTEX_SHADER && sourceType != GL_FRAGMENT_SHADER) {
			std::cout <<
				"[Shader Manager]: Could not create shader from source file!\n" <<
				"|-- Specified type " << sourceType << "is not a vertex or fragment shader identifier!\n";
			return 0;
		}

		unsigned int sourceID = 0;
		unsigned int* srcPtr = &sourceID;

		if (ShaderFactory::findSource(filePath, sourceType, srcPtr)) {
			// the specified vertex shader already exists
			return *srcPtr;
		}
		// create new GL shader
		sourceID = glCreateShader(sourceType);

		// try to read shader contents
		char* shaderSource = nullptr;
		if (!readShader(filePath, shaderSource)) {
			std::cout <<
				"[Shader Manager]: Could not create shader from source file!\n" <<
				"|-- Specified file could not be read.\n";
			glDeleteShader(sourceID);
			
			return 0;
		}

		const char* src = const_cast<const char*>(shaderSource);
		// assuming source file was successfully read, try to compile to the shader
		glShaderSource(sourceID, 1, &src, NULL);
		glCompileShader(sourceID);

		// extremely bad practice, but this is actually allocated in readShader(), need to find a better way to do it
		delete[] shaderSource;

		// finally, check for errors to ensure we're okay to link this to a shader program
		unsigned int err = glGetError();
		if (err != GL_FALSE) {
			glErrorCB(err);
		}

		int logSize = 0;
		glGetShaderiv(sourceID, GL_INFO_LOG_LENGTH, &logSize);
		// log size > 0 when error for shader compilation occurs
		if (logSize > 0) {
			char* shaderOutput = new char[logSize];
			glGetShaderInfoLog(sourceID, logSize, &logSize, shaderOutput);
			std::cout <<
				"[Shader Manager]: Could not create shader from source file!\n" <<
				"|-- Shader failed to compile. See details below:\n" <<
				shaderOutput << '\n';
			delete[] shaderOutput;
			glDeleteShader(sourceID);
			return 0;
		}

		// successfully created shader
		// add to list of existing shaders to be used again later
		//  return source ID
		switch (sourceType) {
		case GL_VERTEX_SHADER:
			m_vertexShaders.insert(filePath, new unsigned int(sourceID));
			return sourceID;
		case GL_FRAGMENT_SHADER:
			m_fragmentShaders.insert(filePath, new unsigned int(sourceID));
			return sourceID;
		default:
			std::cout << "How did you even get here????\n";
			assert(false);
			return 0;
		}
	}

	bool ShaderFactory::findSource(const char* alias, const unsigned int sourceType, unsigned int*& outSourceID) {
		if (sourceType == GL_VERTEX_SHADER) {
			if (m_vertexShaders.find(alias, outSourceID))
				return true;
			else
				return false;
		}
		else if (sourceType == GL_FRAGMENT_SHADER) {
			if (m_fragmentShaders.find(alias, outSourceID))
				return true;
			else
				return false;
		}

		std::cout <<
			"[Shader Manager]: Could not find shader source!\n" <<
			"|-- Specified type was not GL_VERTEX_SHADER or GL_FRAGMENT_SHADER!\n";
		return false;
	}

	void ShaderFactory::terminate() {
		for (uint16_t i = 0; i < m_loadedShaders.size; i++) {
			ShaderProgram* shader;
			if ((shader = m_loadedShaders.data[i].value) == nullptr)
				continue;

			glDeleteProgram(shader->programID);
			shader->programID = 0;
			shader->vertexShaderID = 0;
			shader->fragmentShaderID = 0;
			m_loadedShaders.remove(m_loadedShaders.data[i].key);
		}
		for (uint16_t i = 0; i < m_vertexShaders.size; i++) {
			unsigned int* vert;
			if ((vert = m_vertexShaders.data[i].value) == nullptr)
				continue;

			glDeleteShader(*vert);
			*vert = 0;
			m_vertexShaders.remove(m_vertexShaders.data[i].key);
		}
		for (uint16_t i = 0; i < m_fragmentShaders.size; i++) {
			unsigned int* frag;
			if ((frag = m_fragmentShaders.data[i].value) == nullptr)
				continue;

			glDeleteShader(*frag);
			*frag = 0;
			m_fragmentShaders.remove(m_fragmentShaders.data[i].key);
		}
	}

	bool ShaderFactory::readShader(const char* filePath, char*& outSource) {
		std::string content = "";
		std::ifstream fileStream(filePath, std::ios::in);

		if (!fileStream.is_open()) {
			std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
			return false;
		}

		std::string line = "";
		while (!fileStream.eof()) {
			std::getline(fileStream, line);
			content.append(line);
			content.push_back('\n');
		}

		fileStream.close();
		//content.append("\0");

		size_t size = content.size();
		outSource = new char[size + 1] {0};
		outSource[size] = '\0';
		content.copy(outSource, size, 0);

		return true;
	}
}