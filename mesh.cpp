#include "mesh.h"
#include <GL/glew.h>
#include <numeric>
#include <iostream>

using std::iota;

Mesh::Mesh(vector<float> vertices, vector<unsigned int> indices) : 
	num_indices(indices.size()) 
{
	// Vertex buffer
	unsigned int vert_buff_id;
	glGenBuffers(1, &vert_buff_id);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * vertices.size(), 
		vertices.data(), 
		GL_STATIC_DRAW
	);

	// Index buffer
	unsigned int element_buffer_id;
	glGenBuffers(1, &element_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(unsigned int) * indices.size(), 
		indices.data(), 
		GL_STATIC_DRAW
	);

	// VAO 
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);

	// Vertex layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind
	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(vector<float> vertices) {
	vector<unsigned int> indices(vertices.size() / 5);
	iota(indices.begin(), indices.end(), 0);
    *this = Mesh(vertices, indices);
}

Mesh::Mesh() {}

void Mesh::draw() {
	glBindVertexArray(vao_id);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}