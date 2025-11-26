#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "shader.h"

using std::ifstream;
using std::stringstream;
using std::string;

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

	// Triangle
	float tri_vertices[] = {
		// Positions     // UVs
		0.5,  -0.5, 0.0, 0.5,  -0.5, // Bottom right
		-0.5, -0.5, 0.0, -0.5, -0.5, // Bottom left
		0.0,  0.5,  0.0, 0.0,  0.5   // Top
	};
	unsigned int tri_indices[] {
		0, 1, 2,
	};

	// Square
	float square_vertices[] = {
		// Positions     // UVs
		 0.5,  0.5, 0.0,  0.5,  0.5, // Top right
		 0.5, -0.5, 0.0,  0.5, -0.5, // Bottom right
		-0.5, -0.5, 0.0, -0.5, -0.5, // Bottom left
		-0.5,  0.5, 0.0  -0.5,  0.5  // Top left
	};
	unsigned int square_indices[] {
		0, 1, 3,
		1, 2, 3
	};

	// Vertex buffer
	unsigned int vert_buff_id;
	glGenBuffers(1, &vert_buff_id);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices), tri_vertices, GL_STATIC_DRAW);

	// Index buffer
	unsigned int element_buffer_id;
	glGenBuffers(1, &element_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tri_indices), tri_indices, GL_STATIC_DRAW);

	Shader shader("vert.glsl", "frag.glsl");
	shader.use();

	// VAO 
	unsigned int vert_array_id;
	glGenVertexArrays(1, &vert_array_id);
	glBindVertexArray(vert_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Uniform uniform = shader.get_uniform("our_colour");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Uniform (assumes correct shader program bound)
		float time = glfwGetTime();
		float greeness = (sin(time) / 2.0) + 0.5;
		float blueness = (cos(time) / 2.0) + 0.5;
		uniform.send(0.0, greeness, blueness, 1.0);

		// Assumes correct program and VAO bound
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}