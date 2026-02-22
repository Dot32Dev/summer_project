#ifndef OBJECT_H
#define OBJECT_H

#include "mesh.h"
#include <string>
#include <glm/glm.hpp>

using std::string;

/// The object class exists as an intermediary between loading the object and 
/// sending it to the GPU to be drawn. This lets the loader include object names
/// in its returned data, and also allows the object to be modified before 
/// uploading to the GPU, e.g, to apply transforms to the vertex positions. When
/// you have collected or modified all the data you wish from an object, you get
/// the Mesh with to_mesh(), which you can draw. Meshes lose all information
/// about what their name is or what vertices are involved, as after uplading
/// them to the GPU, the only data necessary to be stored on the CPU is the 
/// OpenGL ID (similar to a pointer) of the data on the GPU.
class Object {
	public:
		/// @brief Constructs an object from a name, verticies and indices
		/// The vertices vector must conform to the Mesh Class's vertex layout 
		/// of three floats for position, two for texture coordinates, and three 
		//// for normals.
		/// @param Name The name of the object
		/// @param vertices Every unique vertex in the object
		/// @param indices Indices into the vertex vec to construct triangles
		Object(
			string Name, 
			vector<float> vertices, 
			vector<unsigned int> indices
		);

		/// @brief Get the mesh from the object that you can draw
		/// The object is not destroyed, so you can make modifications to the 
		/// object and make a mesh for each version if you so wish.
		/// Creating a mesh is not free and should not be called continuously in
		/// a loop. A mesh should be created once and then stored for repeated
		/// rendering.
		/// @return The mesh
		Mesh to_mesh() const;

		/// @brief Get the object's name
		/// @return The name of the object
		string get_name() const;

		/// @brief Get the average position of each vertex in the object
		/// The value is returned via pointers
		/// @param out_x Will set the float at this address to the average x pos
		/// @param out_y Will set the float at this address to the average y pos
		/// @param out_z Will set the float at this address to the average z pos
		void get_avg_vertex_pos(float* out_x, float* out_y, float* out_z) const;

		/// @brief Transform every vertex in the model by a matrix
		/// @param matrix The matrix to transform by
		void transform(const glm::mat4& matrix);
	private:
		string name;
		vector<float> vertices; 
		vector<unsigned int> indices;

};

#endif