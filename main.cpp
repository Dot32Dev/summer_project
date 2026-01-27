#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "obj_importer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;

const GLint WIDTH = 1280;
const GLint HEIGHT = 720;

// Globals 
Uniform* projection_uniform_pointer;

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	glm::mat4 projection = glm::mat4(1.0);
	projection = glm::perspective(
		glm::radians(90.0f), 
		(float)width/height, 
		0.1f, 
		100.0f
	);
	projection_uniform_pointer->send(projection);
}

int main() {
	if (!glfwInit()) {
		glfwTerminate();
		return 1;
	}	
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile dissables OpenGL's immediate mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(
		WIDTH, 
		HEIGHT, 
		"Summer project", 
		NULL, 
		NULL
	);
	if (!window) {
		glfwTerminate();
		return 2;
	}	
	
	int bufferWidth;
	int bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; // Should probably switch to glad some day
	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwSetFramebufferSizeCallback(window, resize);
	glEnable(GL_DEPTH_TEST);

	// Camera
	Camera camera(glm::vec3(0.0f, 1.8f, 3.0f));

	// Character
	vector<Mesh> character = obj_importer("res/Player/model.obj");
	glm::mat4 character_trans = glm::mat4(1.0);

	Shader shader("res/vert.glsl", "res/frag.glsl");
	shader.use(); 

	Texture player = Texture::from_image("res/Player/texture.png");
	Uniform texture_uniform = shader.get_uniform("our_texture");

	Uniform colour_uniform = shader.get_uniform("our_colour");
	Uniform model_uniform = shader.get_uniform("model");
	Uniform view_uniform = shader.get_uniform("view");
	Uniform projection_uniform = shader.get_uniform("projection");
	projection_uniform_pointer = &projection_uniform;

	glm::vec3 rotation_axis = glm::vec3(0.0f, 1.0f, 1.0f);

	glm::mat4 projection = glm::mat4(1.0);
	projection = glm::perspective(
		glm::radians(90.0f), 
		(float)WIDTH/HEIGHT, 
		0.1f, 
		100.0f
	);

	projection_uniform.send(projection);

	double delta_time = 0.0;
	double last_frame = 0.0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		float movement_speed = 1.5 * delta_time;
		float rot_speed = 1.5 * delta_time;

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			movement_speed *= 2.0;
		}

		glm::vec3 pos_input = glm::vec3(0.0f, 0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			pos_input += glm::vec3(0.0f, 0.0f, -movement_speed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			pos_input += glm::vec3(0.0f, 0.0f, movement_speed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			pos_input += glm::vec3(-movement_speed, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			pos_input += glm::vec3(movement_speed, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			pos_input += glm::vec3(0.0f, movement_speed, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			pos_input += glm::vec3(0.0f, -movement_speed, 0.0f);
		}
		glm::vec2 dir_input = glm::vec2(0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			dir_input += glm::vec2(0.0f, -rot_speed);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			dir_input += glm::vec2(0.0f, rot_speed);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			dir_input += glm::vec2(-rot_speed, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			dir_input += glm::vec2(rot_speed, 0.0f);
		}
		camera.pos_input(pos_input);
		camera.dir_input(dir_input);
		view_uniform.send(camera.get_view_matrix());

		colour_uniform.send(1.0f, 1.0f, 1.0f, 1.0f);
		texture_uniform.send(player);
		model_uniform.send(character_trans);
		for (int i = 0; i < character.size(); i++) {
			character[i].draw();
		}

		glfwSwapBuffers(window);		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}