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

vec3 Object::mean_centre() const {
	vec3 sum = vec3(.0f, .0f, .0f);
	int vertex_count = vertices.size() / FLOATS_PER_VERTEX;
	for (int i = 0; i < vertex_count; i++) {
		sum += vec3(
			vertices[i * 8], 
			vertices[i * 8 + 1],
			vertices[i * 8 + 2]
		);
	}
	return sum / (float)vertex_count;
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