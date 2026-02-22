#ifndef PLAYER_H
#define PLAYER_H

#include "mesh.h"
#include "shader.h" // For uniforms
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

using std::vector;
using std::map;
using std::string;
using glm::vec3;
using glm::length;

/// @brief A physics node generally representing a joint on the player
struct Node {
	vec3 pos;
	vec3 prev_pos;
};

/// @brief  Represents a segment connecting two joints, a distance constraint
struct Segment {
	int start_idx;
	int end_idx;
	float length;
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
		void add_segment(int joint_start, int joint_end, int model);
		void add_segment(int joint_start, int joint_end);
		vector<Node> nodes;
		vector<Segment> segments;
		vector<Mesh> meshes;
		// E.g the first item in this vector represents which segment index the
		// first mesh should be connected to
		vector<int> segment_ids;
		Uniform* transform_uniform;
};

#endif