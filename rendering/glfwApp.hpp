#pragma once

struct GLFWwindow;
struct camera;
struct cameraData;

namespace sogl {

	typedef void (*glfwErrorCallback)(int error, const char* msg);
	void glfwDefaultErrorCallback(int, const char*);

	typedef struct glfwWindowData {
		GLFWwindow* pointer;

		unsigned int screenWidth;
		unsigned int screenHeight;
	};

	struct glfwApp {
	private:
		GLFWwindow* m_windPtr;
		glfwErrorCallback m_errorCallback;
		glfwWindowData m_windowData;

		camera* m_renderCamera;
	public:
		/// <summary>
		///		<para>Constructs a new glfwApp with default GLFW/OpenGL contexts/caps.</para>
		///		<para>Creates a new, default projection camera.</para>
		///		<para>If a camera already exists, use the other constructor instead.</para>
		/// </summary>
		glfwApp();

		/// <summary>
		/// Constructs a new glfwApp with default GLFW/OpenGL contexts/caps.
		/// </summary>
		/// <param name="camera">- The camera used for rendering.</param>
		glfwApp(const cameraData& cameraData);

		~glfwApp();

		camera const* getRenderCamera();

	private:
		/// <summary>
		///		<para>Attempts to create a window using standard GLFW/OpenGL context.</para>
		///		<para>On success, sets the current OpenGL context to window, and returns the pointer.</para>
		///		<para>On fail, result will be set to false, and the method returns a null pointer.</para>		
		/// </summary>
		/// 
		/// <param name="screenWidth">- Target width of the screen, in pixels.</param>
		/// <param name="screenHeight">- Target height of the screen, in pixels.</param>
		/// <param name="windowName">- Title displayed on the window's title bar.</param>
		/// <param name="result">- Set to true if success, false if failed.</param>
		/// 
		/// <returns>Pointer to window if successfully created, otherwise nullptr.</returns>
		GLFWwindow* create(const unsigned int& screenWidth, const unsigned int& screenHeight, const char* windowName,
			bool& result) const;
		
	public:
		void enter();
		int quit();
	};
}