#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;

const GLint WIDTH = 1280;
const GLint HEIGHT = 720;

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Summer project", NULL, NULL);
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

	// Square
	vector<float> square_vertices = {
		// Positions      // UVs
		 0.5,  0.5, 0.0,  1.0, 1.0, // Top right
		 0.5, -0.5, 0.0,  1.0, 0.0, // Bottom right
		-0.5, -0.5, 0.0,  0.0, 0.0, // Bottom left
		-0.5,  0.5, 0.0,  0.0, 1.0  // Top left
	};
	vector<unsigned int> square_indices = {
		0, 1, 3,
		1, 2, 3
	};
	Mesh square(square_vertices, square_indices);
	glm::mat4 square_trans = glm::mat4(1.0);
	square_trans = glm::translate(square_trans, glm::vec3(0.0, 0.0, -0.1));

	// Triangle
	vector<float> tri_vertices = {
		// Positions      // UVs
		 0.5, -0.5, 0.0,  1.0,  0.0, // Bottom right
		-0.5, -0.5, 0.0,  0.0,  0.0, // Bottom left
		 0.0,  0.5, 0.0,  0.5,  1.0  // Top
	};
	Mesh triangle(tri_vertices);
	glm::mat4 triangle_trans = glm::mat4(1.0);

	Shader shader("res/vert.glsl", "res/frag.glsl");
	shader.use();

	Texture man = Texture::from_image("res/man.jpg");
	Texture dot32 = Texture::from_image("res/revector_reveal_clean.png");
	Uniform texture_uniform = shader.get_uniform("our_texture");

	Uniform colour_uniform = shader.get_uniform("our_colour");
	Uniform model_uniform = shader.get_uniform("model");
	Uniform view_uniform = shader.get_uniform("view");
	Uniform projection_uniform = shader.get_uniform("projection");

	glm::vec3 rotation_axis = glm::vec3(0.0, 1.0, 1.0);

	glm::mat4 view = glm::mat4(1.0);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -1.0));

	glm::mat4 projection = glm::mat4(1.0);
	projection = glm::perspective(glm::radians(90.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);

	view_uniform.send(view);
	projection_uniform.send(projection);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float movement_speed = 0.1;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			view = glm::translate(view, glm::vec3(0.0, 0.0, movement_speed));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			view = glm::translate(view, glm::vec3(0.0, 0.0, -movement_speed));
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			view = glm::translate(view, glm::vec3(movement_speed, 0.0, 0.0));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			view = glm::translate(view, glm::vec3(-movement_speed, 0.0, 0.0));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			view = glm::translate(view, glm::vec3(0.0, -movement_speed, 0.0));
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			view = glm::translate(view, glm::vec3(0.0, movement_speed, 0.0));
		}
		view_uniform.send(view);

		triangle_trans = glm::rotate(triangle_trans, 0.1f, rotation_axis);

		colour_uniform.send(1.0f, 0.5f, 0.2f, 1.0f);
		texture_uniform.send(dot32);
		model_uniform.send(square_trans);
		square.draw();

		float time = glfwGetTime();
		float greeness = (sin(time) / 2.0) + 0.5;
		float blueness = (cos(time) / 2.0) + 0.5;

		colour_uniform.send(0.0, greeness, blueness, 1.0);
		texture_uniform.send(man);
		model_uniform.send(triangle_trans);
		triangle.draw();

		glfwSwapBuffers(window);		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}