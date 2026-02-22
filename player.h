#ifndef PLAYER_H
#define PLAYER_H

#include "mesh.h"
#include "shader.h" // For uniforms
#include <map>
#include <vector>
#include <string>

using std::vector;
using std::map;
using std::string;

/// @brief A physics node generally representing a joint on the player
struct Node {
	int x, y, z;
	int pre_x, pre_y, pre_z;
};

/// @brief  Represents a segment connecting two joints, a distance constraint
struct Segment {
	int start_idx;
	int end_idx;
	int length;
};

/// @brief The player, also handles verlet integration ragdoll physics
/// Abstracting physics out would just take more time, because I would have to
/// think of a clean interface.
class Player {
	public:
		/// @brief Load and initialise the player
		/// This loads multiple files so may take some time
		/// Takes the transform uniform so that the player's limbs may be 
		/// transformed into the correct locations when drawing.
		/// @param transform_uniform 
		Player(Uniform* transform_uniform);

		/// @brief Draw the player
		/// You should set the player's texture/colour uniforms before drawing,
		/// but the transform uniform is rapidly updated by this draw function
		void draw() const;
	private:
		Player(); // Forbid construction without uniform
		vector<Node> nodes;
		map<string, int> node_names; // To lookup a node index with its name
		vector<Mesh> meshes;
		Uniform* transform_uniform;
};

#endif