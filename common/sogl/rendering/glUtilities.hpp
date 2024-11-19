#pragma once

#include <array>
#include<vector>

struct GLFWwindow;

namespace sogl {
	struct camera; 
	struct vec3f; 
	typedef void(*GLTerminateFunction)();
	
	typedef struct InstanceData {
		GLFWwindow* window = nullptr;
		camera* renderCamera = nullptr;

		float lastTime = 0.0f;
		float time = 0.0f;
		float deltaTime = 0.0f;
		float aspectRatio = 0.0f;

		double mouseDeltaX = 0.0;
		double mouseDeltaY = 0.0;
		double lastMousePosX = 0.0;
		double lastMousePosY = 0.0;
		double mousePosX = 0.0f;
		double mousePosY = 0.0f;

		int windowWidth = 0;
		int windowHeight = 0;

		bool captureCursor = false;

		std::array<int, GLFW_KEY_LAST> gKeysPrev{};
		std::array<int, GLFW_KEY_LAST> gKeysCurr{};

		std::vector<GLTerminateFunction> GLTerminationFunctions;
	} InstanceData;
	
	static InstanceData CurrentInstance;

	GLFWwindow* glInitialize(const int& windowWidth, const int& windowHeight);

	void glTerminate();
	void glAddTerminationFunction(GLTerminateFunction func);

	void glErrorCB(const unsigned int& code);
	static void glfwKeyCB(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwMouseCB(GLFWwindow* window, double xPos, double yPos);

	void glStartFrame();
	void glPollEvents();
	vec3f glConsumeInput();

	void setCaptureCursor(const bool& captureCursor);
	void getCursorPosition(double* xPos, double* yPos);
	void getCursorDelta(double* xDelta, double* yDelta);

	GLFWwindow* getWindow();
	camera* getRenderCamera();

	float deltaTime();
	float aspectRatio();
	int windowWidth();
	int windowHeight();

	bool getKeyPressed(int key);
	bool getKeyDown(int key);
	bool getKeyUp(int key);
}

