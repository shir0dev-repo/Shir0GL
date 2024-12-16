// GL:
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// STD:
#include <cstdlib>
#include <iostream>
#include <string>

// SOGL:
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/camera.hpp>
#include <sogl/transform/vec3f.hpp>
#include <sogl/debug/debug.h>
#include <sogl/world/data/chunk.h>

static void GLFWDefaultErrorCallback(int error, const char* msg) {
	fprintf(stderr,
		"GLFW Error %s: %s\n",
		std::to_string(error).c_str(), msg);
}

namespace sogl {
	static InstanceData CurrentInstance;

	GLFWwindow* glInitialize(const int& windowHeight, const int& windowWidth) {
		srand(static_cast<unsigned>(time(0)));

		std::cout << "Initializing GLFW...";
		if (glfwInit() == GLFW_FALSE) {
			glfwTerminate();
			return nullptr;
		}
		std::cout << '\n' << "GLFW initialized.\n";

		std::cout << "Setting GLFW error callback...";
		glfwSetErrorCallback(GLFWDefaultErrorCallback);
		std::cout << "\nError callback set.\n";

		std::cout << "Setting GLFW window hints...";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		std::cout << "\nGLFW Context: 4.6";


		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		std::cout << "\nOpenGL profile: Compat.";

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		std::cout << "\nDouble buffering enabled.";
		std::cout << "\nGLFW window hints set.\n";

		std::cout << "Creating GLFW window handle...";
		CurrentInstance.window = glfwCreateWindow(windowWidth, windowHeight, "Shir0 Open Game Library", NULL, NULL);

		if (CurrentInstance.window == nullptr) {
			std::cout << "\nGLFW could not create window.\n";

			glfwTerminate();
			assert(false);
		}
		std::cout << "\nGLFW window handle created.\n";

		std::cout << "Setting up GLFW callbacks...";
		glfwSetKeyCallback(CurrentInstance.window, glfwKeyCB);
		glfwSetCursorPosCallback(CurrentInstance.window, glfwMouseCB);
		glfwSetWindowSizeCallback(CurrentInstance.window, glfwResizeCB);
		std::cout << "\nGLFW callbacks initialized.\n";

		glfwSetWindowPos(CurrentInstance.window, 100, 100);
		glfwMakeContextCurrent(CurrentInstance.window);

		std::cout << "Initializing GLEW...";
		glewInit();
		std::cout << "\nGLEW initialized.\n";

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(glDebugErrorCallback, nullptr);

		int err = glGetError();
		if (err != GL_FALSE) {
			std::cout << "ERROR: " << err << '\n';
			glfwTerminate();
			std::cout << "GLFW has been terminated.\n";
			exit(-1);
		}

		CurrentInstance.windowWidth = windowWidth;
		CurrentInstance.windowHeight = windowHeight;
		CurrentInstance.aspectRatio = windowWidth / windowHeight;
		
		CurrentInstance.moveSpeed = 1.0f;

		CurrentInstance.mouseSensitivityX = 20.0f;
		CurrentInstance.mouseSensitivityY = 20.0f;

		CurrentInstance.renderCamera = new camera();
		CurrentInstance.renderCamera->screenWidth = windowWidth;
		CurrentInstance.renderCamera->screenHeight = windowHeight;
		CurrentInstance.renderCamera->aspectRatio = CurrentInstance.aspectRatio;
		CurrentInstance.renderCamera->nearPlane = 0.03f;
		CurrentInstance.renderCamera->farPlane = 100.0f;
		CurrentInstance.renderCamera->fov = 60.0f;
		CurrentInstance.renderCamera->init();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glEnable(GL_POINT_SIZE);
		glfwMaximizeWindow(CurrentInstance.window);
		
		debug::setup();
		//chunk::initialize();
		return CurrentInstance.window;
	}

	void glStartFrame() {
		glfwGetFramebufferSize(CurrentInstance.window, &CurrentInstance.windowWidth, &CurrentInstance.windowHeight);
		CurrentInstance.aspectRatio = (float)CurrentInstance.windowWidth / CurrentInstance.windowHeight;
		glViewport(0, 0, CurrentInstance.windowWidth, CurrentInstance.windowHeight);
		glClearColor(0.07, 0.1, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void glPollEvents() {
		memcpy(CurrentInstance.gKeysPrev.data(), CurrentInstance.gKeysCurr.data(), GLFW_KEY_LAST * sizeof(int));
		glfwPollEvents();

		CurrentInstance.lastTime = CurrentInstance.time;
		CurrentInstance.time = glfwGetTime();
		CurrentInstance.deltaTime = CurrentInstance.time - CurrentInstance.lastTime;

		CurrentInstance.lastMousePosX = CurrentInstance.mousePosX;
		CurrentInstance.lastMousePosY = CurrentInstance.mousePosY;
		glfwGetCursorPos(CurrentInstance.window, &CurrentInstance.mousePosX, &CurrentInstance.mousePosY);
		CurrentInstance.mouseDeltaX = CurrentInstance.lastMousePosX - CurrentInstance.mousePosX;
		CurrentInstance.mouseDeltaY = CurrentInstance.lastMousePosY - CurrentInstance.mousePosY;

		if (CurrentInstance.captureCursor)
			glfwSetInputMode(CurrentInstance.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(CurrentInstance.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	vec3f glConsumeInput() {
		vec3f input;

		if (getKeyDown(GLFW_KEY_W)) {
			input.z = 1;
		}
		else if (getKeyDown(GLFW_KEY_S)) {
			input.z = -1;
		}

		if (getKeyDown(GLFW_KEY_D)) {
			input.x = 1;
		}
		else if (getKeyDown(GLFW_KEY_A)) {
			input.x = -1;
		}

		if (getKeyDown(GLFW_KEY_SPACE)) {
			input.y = 1;
		}
		else if (getKeyDown(GLFW_KEY_LEFT_SHIFT)) {
			input.y = -1;
		}

		return input;
	}

	void glAddTerminationFunction(GLTerminateFunction func) {
		CurrentInstance.GLTerminationFunctions.push_back(func);
	}

	void glTerminate() {
		std::cout << "Cleaning up resources...\n";
		if (!CurrentInstance.GLTerminationFunctions.empty()) {
			for (GLTerminateFunction func : CurrentInstance.GLTerminationFunctions) {
				func();
			}
		}

		glfwDestroyWindow(CurrentInstance.window);
		CurrentInstance.window = nullptr;
		glfwTerminate();
	}

	GLFWwindow* getWindow() {
		return CurrentInstance.window;
	}

	camera* getRenderCamera() {
		return CurrentInstance.renderCamera;
	}

	void getMoveSpeed(float* moveSpeed) {
		*moveSpeed = CurrentInstance.moveSpeed;
	}

	void setMoveSpeed(const float moveSpeed) {
		CurrentInstance.moveSpeed = moveSpeed;
	}

	void getMouseSensitivity(float* sensitivityX, float* sensitivityY) {
		*sensitivityX = CurrentInstance.mouseSensitivityX;
		*sensitivityY = CurrentInstance.mouseSensitivityY;
	}

	void setMouseSensitivity(const float sensitivityX, const float sensitivityY) {
		CurrentInstance.mouseSensitivityX = sensitivityX;
		CurrentInstance.mouseSensitivityY = sensitivityY;

	}

	int getWindowWidth() {
		return CurrentInstance.windowWidth;
	}

	int getWindowHeight() {
		return CurrentInstance.windowHeight;
	}

	float getAspectRatio() {
		return CurrentInstance.aspectRatio;
	}

	float getTime() {
		return CurrentInstance.time;
	}

	float getDeltaTime() {
		return CurrentInstance.deltaTime;
	}

	bool isCursorLocked() {
		return CurrentInstance.captureCursor;
	}

	void setCaptureCursor(const bool& capture) {
		CurrentInstance.captureCursor = capture;
	}

	void toggleCaptureCursor() {
		CurrentInstance.captureCursor = !CurrentInstance.captureCursor;
	}

	void getCursorPosition(double* xPos, double* yPos) {
		*xPos = CurrentInstance.mousePosX / (double)CurrentInstance.windowWidth;
		*yPos = CurrentInstance.mousePosY / (double)CurrentInstance.windowHeight;
	}

	void consumeCursorDelta(float* xDelta, float* yDelta) {
		*xDelta = CurrentInstance.mouseDeltaX; /// CurrentInstance.windowWidth;
		*yDelta = CurrentInstance.mouseDeltaY; /// CurrentInstance.windowHeight;

		/*CurrentInstance.mouseDeltaX = 0.0;
		CurrentInstance.mouseDeltaY = 0.0;*/
	}
	
	bool getKeyDown(int key) {
		return CurrentInstance.gKeysCurr[key] == GLFW_PRESS;
	}

	bool getKeyUp(int key) {
		return CurrentInstance.gKeysCurr[key] == GLFW_RELEASE;
	}

	bool getKeyPressed(int key) {
		return CurrentInstance.gKeysCurr[key] == GLFW_RELEASE && CurrentInstance.gKeysPrev[key] == GLFW_PRESS;
	}

	void glErrorCB(const unsigned int& code) {
		
		switch (code) {
			
		}

		
		const char* error_desc;
		switch (code) {
			// useless errors / warnings go here
			case GL_NONE:
				return;


			case GL_INVALID_ENUM:
				error_desc = "INVALID ENUM";
				break;
			case GL_INVALID_VALUE:
				error_desc = "INVALID VALUE";
				break;
			case GL_INVALID_OPERATION:
				error_desc = "INVALID OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				error_desc = "STACK OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				error_desc = "STACK UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				error_desc = "OUT OF MEMORY";
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				error_desc = "INVALID FRAME BUFFER OPERATION";
			default:
				error_desc = "UNDEFINED ERROR";
		}
		std::cout << "[ERROR " << std::to_string(code) << "]\n";
		std::cout << error_desc << '\n';
	}

	void glDebugErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) {
		switch (id) {
			case 131185:
				return;
			default:
				break;
		}

		std::cout << "[" << severity << " GLERROR (" << type << ")" << id << "]:\n";
		for (int i = 0; i < length; i++) {
			std::cout << message[i];
		}
	}

	const char* glGetNamedType(const uint32_t type) {
		switch (type) {
			case (GL_FLOAT):
				return "GL_FLOAT";
			case (GL_INT):
				return "GL_INT";
			case (GL_FLOAT_VEC3):
				return "GL_FLOAT_VEC3";
			case (GL_FLOAT_VEC4):
				return "GL_FLOAT_VEC4";
			case (GL_FLOAT_MAT3):
				return "GL_FLOAT_MAT3";
			case (GL_FLOAT_MAT4):
					return "GL_FLOAT_MAT4";
			default:
				return "SOGL_UNSUPPORTED_TYPE";
		}
	}

	static void glfwKeyCB(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_REPEAT) return;

		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			return;
		}

		CurrentInstance.gKeysCurr[key] = action;
	}

	static void glfwMouseCB(GLFWwindow* window, double xPos, double yPos) {
		
	}

	static void glfwResizeCB(GLFWwindow* window, int width, int height) {
		CurrentInstance.renderCamera->screenWidth = width;
		CurrentInstance.renderCamera->screenHeight = height;
		CurrentInstance.renderCamera->aspectRatio = width / (float)height;
		CurrentInstance.renderCamera->regenerateProjectionMatrix();
	}
}