#include "object.h"

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

void Object::get_average_vertex_pos(int* out_x, int* out_y, int* out_z) const {
	int x = 0, y = 0, z = 0;
	// Vertex layout is three floats for position, two floats for texture 
	// coordinates, and three for normals. That's 8 floats per vertex.
	int vertex_count = vertices.size()/8;
	for (int i = 0; i < vertex_count; i++) {
		x += vertices[i * 8];
		y += vertices[i * 8 + 1];
		z += vertices[i * 8 + 2];
	}

	*out_x = x / vertex_count;
	*out_y = y / vertex_count;
	*out_z = z / vertex_count;
}