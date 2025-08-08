#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stbi/stb_image.h>

#include <sogl/structure/linkedList.h>
#include <sogl/structure/octree.h>

#include <sogl/rendering/glUtilities.h>
#include <sogl/structure/runLengthEncoding.h>
#include <sogl/rendering/renderable.hpp>
#include <sogl/rendering/camera.hpp>
#include <sogl/rendering/factories/MaterialFactory.h>
#include <sogl/rendering/color.hpp>
// factories:
#include <sogl/rendering/factories/MeshFactory.h>
#include <sogl/rendering/factories/TextureFactory.h>
#include <sogl/rendering/factories/ShaderFactory.h>
#include <sogl/rendering/factories/uniformBufferFactory.hpp>
#include <sogl/rendering/factories/lightFactory.hpp>
#include <sogl/rendering/factories/ModelFactory.h>

#include <sogl/world/data/chunk.h>
#include <sogl/world/data/chunkMesh.h>
using namespace sogl;

const int W_WIDTH = 800;
const int W_HEIGHT = 800;

int main() {
	GLFWwindow* windPtr = glInitialize(W_WIDTH, W_HEIGHT);
	glAddTerminationFunction(MeshFactory::Terminate);
	MaterialFactory::SetSaveOnTerminate(true);
	glAddTerminationFunction(MaterialFactory::Terminate);
	glAddTerminationFunction(uniformBufferFactory::terminate);
	glAddTerminationFunction(lightFactory::terminate);
	glAddTerminationFunction(TextureFactory::terminate);
	glAddTerminationFunction(ShaderFactory::terminate);

	Texture* defaultTexture = TextureFactory::getDefaultTexture();
	Texture* viviTexture = TextureFactory::loadTexture("assets/tex/vivi-col.png");
	Texture* viviWandTexture = TextureFactory::loadTexture("assets/tex/vivi-wand-col.png");
	
	ShaderProgram* litShader = ShaderFactory::createNew("assets/shader/lit.vert", "assets/shader/lit.frag", "lit");
	//shaderProgram* defaultShader = shaderFactory::createNew("assets/shader/default.vert", "assets/shader/default.frag", "default");
	//shaderProgram* normalColorShader = shaderFactory::createNew("assets/shader/default.vert", "assets/shader/normal_color.frag", "normalColor");
	//shaderProgram* texCoordColorShader = shaderFactory::createNew("assets/shader/default.vert", "assets/shader/tcoord_color.frag", "texCoordColor");
	ShaderProgram* texturedShader = ShaderFactory::createNew("assets/shader/default.vert", "assets/shader/texture.frag", "textured");
	ShaderProgram* wireFrameShader = ShaderFactory::createNew("assets/shader/wireframe.vert", "assets/shader/wireframe.frag", "wireframe");
	//material* defaultMaterial = createMaterial(defaultShader, "defaultMat");
	//material* normalMaterial = createMaterial(normalColorShader, "normalMat");
	Material* litMaterial = MaterialFactory::CreateNew(litShader, "litMat");
	Material* viviMaterial = MaterialFactory::CreateNew(litShader, "viviMat");
	Material* viviWandMaterial = MaterialFactory::CreateNew(litShader, "viviWandMat");
	Material* wireframeMaterial = MaterialFactory::CreateNew(wireFrameShader, "wireframeMat");

	litMaterial->addTexture(defaultTexture);
	viviMaterial->addTexture(viviTexture);
	viviWandMaterial->addTexture(viviWandTexture);

	litMaterial->listAllUniforms();
	
	float spec = 0.5f;
	float amb = 0.15f;
	float diffuse = 0.6;
	float shine = 1.5;

	litMaterial->bind();
	litMaterial->uploadUniformData("specularReflection", &spec);
	litMaterial->uploadUniformData("ambientReflection", &amb);
	litMaterial->uploadUniformData("diffuseReflection", &diffuse);
	litMaterial->uploadUniformData("shininess", &shine);
	litMaterial->unbind();

	viviMaterial->bind();
	viviMaterial->uploadUniformData("specularReflection", &spec);
	viviMaterial->uploadUniformData("ambientReflection", &amb);
	viviMaterial->uploadUniformData("diffuseReflection", &diffuse);
	viviMaterial->uploadUniformData("shininess", &shine);
	viviMaterial->unbind();

	viviWandMaterial->bind();
	viviWandMaterial->uploadUniformData("specularReflection", &spec);
	viviWandMaterial->uploadUniformData("ambientReflection", &amb);
	viviWandMaterial->uploadUniformData("diffuseReflection", &diffuse);
	viviWandMaterial->uploadUniformData("shininess", &shine);
	viviWandMaterial->unbind();

	light* dirLight = lightFactory::createDirectionalLight(
		vec3f(-1, -1, -1).normalized(),
		vec4f(0.1, 0.24, 0.4, 1),
		vec4f(1, 1, 1, 0.5));
	
	//light* pointLight1 = lightFactory::createPointLight(
	//	vec3f(0, 0, 0), 
	//	vec4f(1, 0.3, 0.1, 2), 
	//	vec4f(0.7, 0.1, 0.1, 1), 
	//	vec3f(20, 0, 0),
	//	5);
	light* pointLight2 = lightFactory::createPointLight(
		vec3f(0, 0, 0),
		vec4f(1, 0.3, 0.1, 2),
		vec4f(0.7, 0.1, 0.1, 1),
		vec3f(20, 0, 0),
		5);
	
	light* spotLight = lightFactory::createSpotLight(
		vec3f(0, 20, -5), 
		vec3f(0, -1, 0), 
		vec4f(1, 1, 1, 0.3),
		vec4f(1,1,1,0.3),
		vec3f(0.033, 0.09, 0.0),
		7,
		11);
	
	
	const Mesh* cube = MeshFactory::CreateNew("assets/mesh/cube.obj", "cube");
	const Mesh* head = MeshFactory::CreateNew("assets/mesh/head.obj", "head");
	const Mesh* vivi = MeshFactory::CreateNew("assets/mesh/vivi.obj", "vivi");
	const Mesh* viviWand = MeshFactory::CreateNew("assets/mesh/vivi-wand.obj", "viviWand");
	const Mesh* plane = MeshFactory::CreateNew("assets/mesh/plane.obj", "plane");

	renderable viviRenderable(*vivi, viviMaterial);
	viviRenderable.transform.setPosition(vec3f(0, -1, -5));
	renderable viviWandRenderable(*viviWand, viviWandMaterial);
	viviWandRenderable.transform.setPosition(vec3f(0, -1, -5));

	renderable planeRenderable(*plane, litMaterial);
	planeRenderable.transform.setPosition(vec3f(0, -1, 0));
	Model* vvModel = new Model(vivi, viviMaterial);

	setMoveSpeed(10);
	setMouseSensitivity(2, 2);
	float moveSpeed;
	getMoveSpeed(&moveSpeed);

	vec3f input;
	vec3f cursorDelta;
	camera* const renderCamera = getRenderCamera();
	debug::setPointSize(5);
	
	Chunk ch(vec3f(0, 0, 0));
	ChunkMesh chMesh(ch);
	while (!glfwWindowShouldClose(windPtr)) {
		glStartFrame();
		glPollEvents();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(renderCamera->projectionMatrix.getPointer());
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf(renderCamera->viewMatrix.getPointer());

		float deltaTime = getDeltaTime();
		if (getKeyPressed(GLFW_KEY_LEFT_ALT)) {
			toggleCaptureCursor();
		}
		if (isCursorLocked()) {
			// get input values
			float sensX, sensY;
			getMouseSensitivity(&sensX, &sensY);
			consumeCursorDelta(&cursorDelta.y, &cursorDelta.x);
			cursorDelta.x *= sensX * deltaTime;
			cursorDelta.y *= sensY * deltaTime;
			getMoveSpeed(&moveSpeed);
			input = glConsumeInput() * moveSpeed;
			renderCamera->update(input, cursorDelta, getAspectRatio());
		}
		else {
			renderCamera->update(vec3f::ZERO, vec3f::ZERO, getAspectRatio());
		}

		//pointLight1->positionOrDirection = vec4f(2 * sinf(getTime()), 1, (2 * cosf(getTime())) - 5, 0);
		pointLight2->positionOrDirection = vec4f(2 * -sinf(getTime()), 1, (2 * -cosf(getTime())) - 5, 0);
		//lightFactory::updateLightBuffer(pointLight1);
		lightFactory::updateLightBuffer(pointLight2);
		viviRenderable.render();
		viviWandRenderable.render();
		planeRenderable.render();
		//ch.draw();
		//tree.drawOutline();
		
		//glBegin(GL_TRIANGLES);
		//for (int i = 0; i < chMesh.meshData->VertexCount(); i+= 3) {
		//	glVertex3f(chMesh.meshData->Vertices()[i], chMesh.meshData->Vertices()[i + 1], chMesh.meshData->Vertices()[i + 2]);
		//}
		//glEnd();
		debug::finalize();

		glfwSwapBuffers(windPtr);
	}

	glTerminate();
}