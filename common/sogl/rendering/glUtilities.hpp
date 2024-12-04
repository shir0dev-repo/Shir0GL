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

		float mouseSensitivityX = 0.0f;
		float mouseSensitivityY = 0.0f;

		float moveSpeed = 0.0f;

		double mouseDeltaX = 0.0;
		double mouseDeltaY = 0.0;
		double lastMousePosX = 0.0;
		double lastMousePosY = 0.0;
		double mousePosX = 0.0f;
		double mousePosY = 0.0f;

		int windowWidth = 0;
		int windowHeight = 0;

		bool captureCursor = false;

		std::array<int, 348> gKeysPrev{};
		std::array<int, 348> gKeysCurr{};

		std::vector<GLTerminateFunction> GLTerminationFunctions;
	} InstanceData;
	
	GLFWwindow* glInitialize(const int& windowWidth, const int& windowHeight);
	void glStartFrame();
	void glPollEvents();
	vec3f glConsumeInput();

	void glAddTerminationFunction(GLTerminateFunction func);
	void glTerminate();

	GLFWwindow* getWindow();
	camera* getRenderCamera();

	void getMoveSpeed(float* moveSpeed);
	void setMoveSpeed(const float moveSpeed);

	void getMouseSensitivity(float* sensitivityX, float* sensitivityY);
	void setMouseSensitivity(const float sensitivityX, const float sensitivityY);

	bool isCursorLocked();
	void setCaptureCursor(const bool& captureCursor);
	void toggleCaptureCursor();

	void getCursorPosition(double* xPos, double* yPos);
	void consumeCursorDelta(float* xDelta, float* yDelta);

	
	float getTime();
	float getDeltaTime();
	float getAspectRatio();
	int getWindowWidth();
	int getWindowHeight();

	bool getKeyPressed(int key);
	bool getKeyDown(int key);
	bool getKeyUp(int key);

	void glErrorCB(const unsigned int& code);
	void glDebugErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
	const char* glGetNamedType(const uint32_t type);

	static void glfwKeyCB(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwMouseCB(GLFWwindow* window, double xPos, double yPos);
	static void glfwResizeCB(GLFWwindow* window, int width, int height);
}

