#ifndef MESH_H
#define MESH_H

#include <vector>

using std::vector;

class Mesh {
	public:
		/// @brief Constructs a mesh from vectors of vertices and indices
		/// The vertices vector must conform to the vertex layout of three
		/// floats for position, two for texture coordinate + three for normals.
		/// @param vertices The vertices to construct from
		/// @param indices The indices to construct from
		Mesh(vector<float> vertices, vector<unsigned int> indices);

		/// @brief Constructs a mesh from just vertices, generating the indicies
		/// The vertices vector must conform to the vertex layout of three
		/// floats for position and two floats for texture coordinate.
		/// @param vertices The vertices to construct from
		Mesh(vector<float> vertices);

		/// @brief Draws the mesh
		void draw();
	private:
		Mesh();
		int num_indices;
		unsigned int vao_id; 
};

#endif