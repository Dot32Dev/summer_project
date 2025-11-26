#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "shader.h"
#include "mesh.h"

using std::vector;

const GLint WIDTH = 1280;
const GLint HEIGHT = 720;

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

	// Square
	vector<float> square_vertices = {
		// Positions     // UVs
		 0.5 + 0.1,  0.5 + 0.1, 0.0 + 0.1,  0.5 + 0.1,  0.5 + 0.1, // Top right
		 0.5 + 0.1, -0.5 + 0.1, 0.0 + 0.1,  0.5 + 0.1, -0.5 + 0.1, // Bottom right
		-0.5 + 0.1, -0.5 + 0.1, 0.0 + 0.1, -0.5 + 0.1, -0.5 + 0.1, // Bottom left
		-0.5 + 0.1,  0.5 + 0.1, 0.0 + 0.1, -0.5 + 0.1,  0.5 + 0.1  // Top left
	};
	vector<unsigned int> square_indices = {
		0, 1, 3,
		1, 2, 3
	};
	Mesh square(square_vertices, square_indices);

	// Triangle
	vector<float> tri_vertices = {
		// Positions     // UVs
		 0.5, -0.5, 0.0,  0.5, -0.5, // Bottom right
		-0.5, -0.5, 0.0, -0.5, -0.5, // Bottom left
		 0.0,  0.5, 0.0,  0.0,  0.5  // Top
	};
	Mesh triangle(tri_vertices);

	Shader shader("vert.glsl", "frag.glsl");
	shader.use();

	Uniform uniform = shader.get_uniform("our_colour");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		uniform.send(1.0f, 0.5f, 0.2f, 1.0f);
		square.draw();

		// Uniform (assumes correct shader program bound)
		float time = glfwGetTime();
		float greeness = (sin(time) / 2.0) + 0.5;
		float blueness = (cos(time) / 2.0) + 0.5;
		uniform.send(0.0, greeness, blueness, 1.0);

		triangle.draw();

		glfwSwapBuffers(window);		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}