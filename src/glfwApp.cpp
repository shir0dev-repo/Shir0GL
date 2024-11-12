// GL:
#include <glew/glew.h>
#include <glfw/glfw3.h>

// STD:
#include <iostream>
#include <string>
#include <cassert>

// SOGL:
#include "../rendering/camera.hpp"
#include "../rendering/glfwApp.hpp"

namespace sogl {

	void glfwDefaultErrorCallback(int error, const char* msg) {
		fprintf(stderr,
			"GLFW Error %s: %s\n",
			std::to_string(error).c_str(), msg);
	}

	glfwApp::glfwApp() {
		m_renderCamera = new camera();
		bool result = false;
		m_windPtr = create(m_renderCamera->m_cameraData.screenWidth, m_renderCamera->m_cameraData.screenHeight,
			"New GLFW Project", result);

		assert(result);
	}

	glfwApp::glfwApp(const cameraData& cam) {
		m_renderCamera = new camera(cam);
		bool result = false;
		m_windPtr = create(m_renderCamera->m_cameraData.screenWidth, m_renderCamera->m_cameraData.screenHeight,
			"New GLFW Project", result);

		assert(result);
	}

	glfwApp::~glfwApp() {
		delete m_renderCamera;
		m_renderCamera = nullptr;
	}

	GLFWwindow* glfwApp::create(const unsigned int& screenWidth, const unsigned int& screenHeight, const char* windowName,
		bool& result) const {

		if (glfwInit() == GLFW_FALSE) {
			glfwTerminate();
			result = false;
			return nullptr;
		}

		if (m_errorCallback == nullptr)
			glfwSetErrorCallback(glfwDefaultErrorCallback);
		else
			glfwSetErrorCallback(m_errorCallback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

		GLFWwindow* wPtr = glfwCreateWindow(screenWidth, screenHeight, windowName, NULL, NULL);

		if (wPtr == nullptr) {
			std::cout << "GLFW could not create window.\n";

			glfwTerminate();
			result = false;
			return nullptr;
		}

		glfwSetWindowPos(wPtr, 100, 100);
		glfwMakeContextCurrent(wPtr);

		glewInit();

		result = true;
		return wPtr;
	}

	camera const* glfwApp::getRenderCamera() {
		return m_renderCamera;
	}

	void glfwApp::enter() {
		while (!glfwWindowShouldClose(m_windPtr)) {
			glfwPollEvents();

			glfwSwapBuffers(m_windPtr);
		}

		quit();
	}

	int glfwApp::quit() {
		glfwDestroyWindow(m_windPtr);
		m_windPtr = nullptr;

		glfwTerminate();

		return 0;
	}
}