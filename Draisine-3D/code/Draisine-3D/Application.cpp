#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "Camera.h"
#include "Utilities.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "TexturesMapper.h"
#include "Renderer.h"
#include "OrtogonalProjection.h"
#include "PerspectiveProjection.h"
#include "PositionFrontUpView.h"
#include "primitives/Trapeze.h"
#include "composites/Floor.h"
#include "composites/railway.h"
#include "composites/Cart.h"

using namespace std;
const GLuint WIDTH = 1280, HEIGHT = 720;

const glm::vec3 cameraOffset = glm::vec3(0.0f, 1.0f, 3.0f);
const glm::vec3 defaultFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraPos = cameraOffset;
glm::vec3 cameraFront = defaultFront;
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 prevPos(0.0f, 0.0f, 0.0f);
Camera cam = Camera(cameraPos, cameraFront, cameraUp);

glm::vec3 cartPos;
double fps = 0.0;
const float FPS_CONST = 0.05f * 60.0f;
float step = 0.0f;
double prev_X, prev_Y;

glm::vec3 skybox_offset;

bool skybox_should_move = false;
bool camera_attached = true;
void mouse_callback(GLFWwindow* window, double new_X, double new_Y)
{
	cam.Rotate_Mouse(prev_X - new_X, prev_Y - new_Y);
	glfwSetCursorPos(window, prev_X, prev_Y);
}
//for keys pressed once
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// cout << key << endl;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (camera_attached == false) {
			prevPos = cam.getPos();
			cam.setPos(cartPos + cameraOffset);
			cam.setFront(defaultFront);
			cam.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
			camera_attached = true;
			skybox_offset = cartPos - prevPos;
			skybox_should_move = true;
		}
		else camera_attached = false;
	}
}
//for "sticky keys"
void process_sticky_keys(GLFWwindow* window, Cube& skybox, Cart& cart)
{
	prevPos = cam.getPos();
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)//FASTER
		step = (FPS_CONST / fps) * 5;
	else step = FPS_CONST / fps;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//STEP_FORWARD
		cam.Step_Longtitudal(step);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//STEP_BACK
		cam.Step_Longtitudal(-step);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)//STEP_LEFT
		cam.Step_Lateral(-step);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //STEP_RIGHT
		cam.Step_Lateral(step);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //STEP_UP
		cam.Step_Vertical(step);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //STEP_DOWN
		cam.Step_Vertical(-step);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //SPEED_UP
		cart.addSpeed(0.1);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //SPEED_DOWN
		cart.addSpeed(-0.1);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)//STOP
		cart.stop();
	//should be in key_callback but we need cart handle

	if (!camera_attached)
		skybox.move(cam.getPos() - prevPos);
}
void render(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) {
	shader.bind();
	vertexArray.bind();
	indexBuffer.bind();
	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

void renderQuad();

void clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 04", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(window, &prev_X, &prev_Y);
		glewExperimental = GL_TRUE;

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");
		std::shared_ptr<PositionFrontUpView> view = std::make_shared<PositionFrontUpView>(cameraPos, cameraFront, cameraUp);
		//std::shared_ptr<OrtogonalProjection> projection = std::make_shared<OrtogonalProjection>(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
		float fov = 45.0f;
		std::shared_ptr<PerspectiveProjection> projection = std::make_shared<PerspectiveProjection>(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.25f, 100.0f);

		// configure depth map FBO
		// -----------------------
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture
		unsigned int depthMap;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		auto skybox_tex = std::make_shared<Texture>("textures/skybox.png");
		Cube skybox(skybox_tex, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 40.0, 40.0, 40.0 });
		Cart cart;

		Floor floor(4);
		auto shader = std::make_shared<Shader>("shaders/shader.vert", "shaders/fullLighting.frag");
		auto depthShader = std::make_shared<Shader>("shaders/depthMap.vert", "shaders/depthMap.frag");

		shader->bind();
		shader->setUniformInt("shadowMap", 1);

		double lastTime = glfwGetTime(), deltaT = 0.0, spf = 0.0;
		float currentFrame;
		int nbFrames = 0;
		glm::vec3 lightPos = { -3.0f, 4.2f, 5.0f };

		// main event loop
		while (!glfwWindowShouldClose(window)) {
			currentFrame = glfwGetTime();
			deltaT = currentFrame - lastTime;
			nbFrames++;
			if (deltaT >= 0.25) {
				fps = 4 * nbFrames;
				nbFrames = 0;
				lastTime += 0.25f;
			}
			//cout << "Current Frame: " << currentFrame << "\nFPS: " << fps << endl;

			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			if (camera_attached)cam.setPos(cart.getPos() + cameraOffset);
			cameraPos = cam.getPos();
			cameraFront = cam.getFront();
			//cameraUp = cam.getUp();
			view->setPosition(cameraPos);
			view->setFront(cameraFront);
			view->setUp(cameraUp);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// 1. render depth of scene to texture (from light's perspective)
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			float near_plane = 5.0f, far_plane = 40.0f;
			lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
			//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			lightView = glm::lookAt(cart.getPos() + glm::vec3(-10.0, 10.0, 10.0), cart.getPos() + glm::vec3(0.0, 0.0, -10.0), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;
			// render scene from light's point of view
			depthShader->bind();
			depthShader->setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			cart.render(depthShader, false);
			floor.render(depthShader, false);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// reset viewport
			glViewport(0, 0, WIDTH, HEIGHT);
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader->bind();
			glViewport(0, 0, WIDTH, HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			// set uniforms
			glm::mat4 viewMatrix = view->getMatrix();
			shader->setUniformMat4("VIEW", viewMatrix);

			glm::mat4 projectionMatrix = projection->getMatrix();
			shader->setUniformMat4("PROJECTION", projectionMatrix);
			shader->setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

			// uniforms for fragment shader
			glm::vec3 lightColor = { 1.0, 1.0, 1.0 };
			shader->setUniformVec3("LIGHT_COLOR", lightColor);
			shader->setUniformVec3("LIGHT_POS", lightPos);
			glm::vec3 cameraPos = cam.getPos();
			shader->setUniformVec3("CAMERA_POS", cameraPos);
			float ambientLightStrenght = 0.3f;
			shader->setUniformFloat("AMBIENT_LIGHT_STRENGHT", ambientLightStrenght);
			float diffuseLightStrenght = 0.8f;
			shader->setUniformFloat("DIFFUSE_LIGHT_STRENGHT", diffuseLightStrenght);
			float specularLightStrenght = 0.5f;
			shader->setUniformFloat("SPECULAR_LIGHT_STRENGHT", specularLightStrenght);
			cart.render(shader);
			ambientLightStrenght = 0.6f;
			shader->setUniformFloat("AMBIENT_LIGHT_STRENGHT", ambientLightStrenght);
			diffuseLightStrenght = 0.8f;
			shader->setUniformFloat("DIFFUSE_LIGHT_STRENGHT", diffuseLightStrenght);
			specularLightStrenght = 0.0f;
			shader->setUniformFloat("SPECULAR_LIGHT_STRENGHT", specularLightStrenght);
			floor.render(shader);

			ambientLightStrenght = 1.0f;
			shader->setUniformFloat("AMBIENT_LIGHT_STRENGHT", ambientLightStrenght);
			diffuseLightStrenght = 0.0f;
			shader->setUniformFloat("DIFFUSE_LIGHT_STRENGHT", diffuseLightStrenght);
			specularLightStrenght = 0.0f;
			shader->setUniformFloat("SPECULAR_LIGHT_STRENGHT", specularLightStrenght);
			skybox.render(shader);

			if (camera_attached)prevPos = cart.getPos();
			cart.moveAuto();
			if (camera_attached)skybox.move(cart.getPos() - prevPos);
			lightPos = cart.getPos() + glm::vec3(-3.0f, 4.2f, 3.0f);

			if ((cart.getPos() - floor.getPos()).z > 5) floor.reposition(1);
			if ((cart.getPos() - floor.getPos()).z < -5) floor.reposition(-1);

			mouse_callback(window, prev_X, prev_Y);
			glfwSwapBuffers(window);
			process_sticky_keys(window, skybox, cart);
			cartPos = cart.getPos();
			glfwPollEvents();
			if (skybox_should_move) {
				skybox_should_move = false;
				skybox.move(skybox_offset);
			}
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();
	return 0;
}