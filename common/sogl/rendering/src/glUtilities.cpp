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

static void GLFWDefaultErrorCallback(int error, const char* msg) {
	fprintf(stderr,
		"GLFW Error %s: %s\n",
		std::to_string(error).c_str(), msg);
}

 

namespace sogl {
	GLFWwindow* glInitialize(const int& windowHeight, const int& windowWidth) {
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

		std::cout << "Setting up callbacks...";
		glfwSetKeyCallback(CurrentInstance.window, glfwKeyCB);
		glfwSetCursorPosCallback(CurrentInstance.window, glfwMouseCB);
		std::cout << "\nGLFW callbacks initialized.\n";

		CurrentInstance.windowWidth = windowWidth;
		CurrentInstance.windowHeight = windowHeight;
		CurrentInstance.aspectRatio = windowWidth / windowHeight;
		
		CurrentInstance.renderCamera = new camera();
		CurrentInstance.renderCamera->screenWidth = windowWidth;
		CurrentInstance.renderCamera->screenHeight = windowHeight;
		CurrentInstance.renderCamera->aspectRatio = CurrentInstance.aspectRatio;
		CurrentInstance.renderCamera->init();

		glfwSetWindowPos(CurrentInstance.window, 100, 100);
		glfwMakeContextCurrent(CurrentInstance.window);
		
		std::cout << "Initializing GLEW...";
		glewInit();
		std::cout << "\nGLEW initialized.\n";

		int err = glGetError();
		if (err != GL_FALSE) {
			std::cout << "ERROR: " << err << '\n';
			glfwTerminate();
			std::cout << "GLFW has been terminated.\n";
			assert(false);
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		return CurrentInstance.window;
	}

	void glPollEvents() {
		memcpy(CurrentInstance.gKeysPrev.data(), CurrentInstance.gKeysCurr.data(), GLFW_KEY_LAST * sizeof(int));
		glfwPollEvents();

		CurrentInstance.lastTime = CurrentInstance.time;
	}

	vec3f glConsumeInput() {
		vec3f input;

		if (CurrentInstance.gKeysCurr[GLFW_KEY_W] == GLFW_PRESS) {
			input.z = 1;
		}
		else if (CurrentInstance.gKeysCurr[GLFW_KEY_S] == GLFW_PRESS) {
			input.z = -1;
		}

		if (CurrentInstance.gKeysCurr[GLFW_KEY_D] == GLFW_PRESS) {
			input.x = 1;
		}
		else if (CurrentInstance.gKeysCurr[GLFW_KEY_A] == GLFW_PRESS) {
			input.x = -1;
		}

		if (CurrentInstance.gKeysCurr[GLFW_KEY_SPACE] == GLFW_PRESS) {
			input.y = 1;
		}
		else if (CurrentInstance.gKeysCurr[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS) {
			input.y = -1;
		}

		return input;
	}

	void glTerminate() {
		if (!CurrentInstance.GLTerminationFunctions.empty()) {
			for (GLTerminateFunction func : CurrentInstance.GLTerminationFunctions) {
				func();
			}
		}

		glfwDestroyWindow(CurrentInstance.window);
		CurrentInstance.window = nullptr;
		glfwTerminate();
	}

	void glAddTerminationFunction(GLTerminateFunction func) {
		CurrentInstance.GLTerminationFunctions.push_back(func);
	}

	GLFWwindow* getWindow() {
		return CurrentInstance.window;
	}

	camera* getRenderCamera() {
		return CurrentInstance.renderCamera;
	}

	int windowWidth() {
		return CurrentInstance.windowWidth;
	}

	int windowHeight() {
		return CurrentInstance.windowHeight;
	}

	float aspectRatio() {
		return CurrentInstance.aspectRatio;
	}

	float deltaTime() {
		return CurrentInstance.deltaTime;
	}

	void setCaptureCursor(const bool& capture) {
		CurrentInstance.captureCursor = capture;
	}

	void getCursorPosition(double* xPos, double* yPos) {
		*xPos = CurrentInstance.mousePosX / (double)CurrentInstance.windowWidth;
		*yPos = CurrentInstance.mousePosY / (double)CurrentInstance.windowHeight;
	}

	void getCursorDelta(double* xDelta, double* yDelta) {
		*xDelta = CurrentInstance.mouseDeltaX / (double)CurrentInstance.windowWidth;
		*yDelta = CurrentInstance.mouseDeltaY / (double)CurrentInstance.windowHeight;
	}

	void glStartFrame() {
		CurrentInstance.time = glfwGetTime();
		CurrentInstance.deltaTime = CurrentInstance.time - CurrentInstance.lastTime;

		GLFWwindow* window = CurrentInstance.window;
		glfwGetFramebufferSize(window, &CurrentInstance.windowWidth, &CurrentInstance.windowHeight); 
		CurrentInstance.aspectRatio = (float)CurrentInstance.windowWidth / CurrentInstance.windowHeight;
		
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		std::cout << "[ERROR " << std::to_string(code) << "]\n";
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
		CurrentInstance.mouseDeltaX = CurrentInstance.mousePosX - xPos;
		CurrentInstance.mouseDeltaY = CurrentInstance.mousePosY - yPos;

		CurrentInstance.mousePosX = xPos;
		CurrentInstance.mousePosY = yPos;

		if (CurrentInstance.captureCursor)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}