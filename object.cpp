#include "object.h"
#include <glm/gtc/matrix_transform.hpp>

// Vertex layout is three floats for position, two floats for texture 
// coordinates, and three for normals. That's 8 floats per vertex.
const int FLOATS_PER_VERTEX = 8;

// Most basic shit ever
Object::Object(
	string name, 
	vector<float> vertices, 
	vector<unsigned int> indices
) :
	name(name),
	vertices(vertices),
	indices(indices)
{}

Mesh Object::to_mesh() const {
	return Mesh(vertices, indices);
}

string Object::get_name() const {
	return name;
}

void Object::get_avg_vertex_pos(float* out_x, float* out_y, float* out_z) const {
	float x = 0, y = 0, z = 0;
	int vertex_count = vertices.size() / FLOATS_PER_VERTEX;
	for (int i = 0; i < vertex_count; i++) {
		x += vertices[i * 8];
		y += vertices[i * 8 + 1];
		z += vertices[i * 8 + 2];
	}

	*out_x = x / vertex_count;
	*out_y = y / vertex_count;
	*out_z = z / vertex_count;
}

void Object::transform(const glm::mat4& matrix) {
	int vertex_count = vertices.size() / FLOATS_PER_VERTEX;
	for (int i = 0; i < vertex_count; i++) {
		glm::vec4 vert = glm::vec4(
			vertices[i * 8], 
			vertices[i * 8 + 1],
			vertices[i * 8 + 2],
			1.0f
		);
		vert = matrix * vert;
		vertices[i * 8] = vert.x;
		vertices[i * 8 + 1] = vert.y;
		vertices[i * 8 + 2] = vert.z;
	}
}