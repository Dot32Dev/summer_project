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
	Uniform transform_uniform = shader.get_uniform("transform");

	glm::vec3 rotation_axis = glm::vec3(0.0, 0.0, 1.0);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		triangle_trans = glm::rotate(triangle_trans, 0.1f, rotation_axis);

		colour_uniform.send(1.0f, 0.5f, 0.2f, 1.0f);
		texture_uniform.send(dot32);
		transform_uniform.send(square_trans);
		square.draw();

		float time = glfwGetTime();
		float greeness = (sin(time) / 2.0) + 0.5;
		float blueness = (cos(time) / 2.0) + 0.5;

		colour_uniform.send(0.0, greeness, blueness, 1.0);
		texture_uniform.send(man);
		transform_uniform.send(triangle_trans);
		triangle.draw();

		glfwSwapBuffers(window);		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}