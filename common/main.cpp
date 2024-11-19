#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/shaderUtilities.hpp>
#include <sogl/rendering/renderable.hpp>
#include <sogl/rendering/camera.hpp>
#include <sogl/rendering/shaderProgram.hpp>
#include <sogl/rendering/mesh.hpp>
#include <sogl/rendering/GLError.hpp>
using namespace sogl;

const int W_WIDTH = 800;
const int W_HEIGHT = 800;

int main() {
	glInitialize(W_WIDTH, W_HEIGHT);
	glAddTerminationFunction(terminateShaders);

	addShader("normals", "default", "normal_color");
	addShader("texCoords", "default", "tcoord_color");
	addShader("textured", "default", "texture");
	shaderProgram shader("default", "default", "normal_color");
	mesh head("assets/mesh/head.obj");
	renderable headRenderable(head, "texCoords");

	mesh ct4("assets/mesh/ct4.obj");
	renderable ct4Renderable(ct4, "textured");
	ct4Renderable.addTexture("assets/tex/ct4_grey.png");
	ct4Renderable.transform->translate(vec3f(10, 0, 0));

	vec3f input;
	vec3f cursorDelta;

	while (!glfwWindowShouldClose(getWindow())) {
		glStartFrame();
		glPollEvents();

		// get input values
		input = glConsumeInput();
		getCursorDelta((double*)&cursorDelta.x, (double*)&cursorDelta.y);
		
		//update camera
		getRenderCamera()->update(input, cursorDelta, aspectRatio());
		headRenderable.render();

		glfwSwapBuffers(getWindow());
	}
}