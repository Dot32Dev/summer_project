#include "player.h"
#include "obj_importer.h"
#include "object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Player::Player(Uniform* transform_uniform) : 
	nodes(vector<Node>()),
	meshes(vector<Mesh>()),
	segment_ids(vector<int>()),
	transform_uniform(transform_uniform)
{
	map<string, int> node_names; // To lookup a node index with its name
	map<string, int> object_names;

	vector<Object> joints = obj_importer("res/Player/Joints.obj");
	for (int i=0; i<joints.size(); i++) {
		float x, y, z;
		joints[i].get_avg_vertex_pos(&x, &y, &z);
		nodes.push_back(Node {vec3(x, y, z), vec3(x, y, z)});
		node_names.insert({joints[i].get_name(), i});
	}

	vector<Object> objects = obj_importer("res/Player/Player.obj");
	for (int i=0; i<objects.size(); i++) {
		float x, y, z;
		objects[i].get_avg_vertex_pos(&x, &y, &z);
		glm::mat4 transform = glm::mat4(1.0);
		transform = glm::translate(
			transform, 
			glm::vec3((float)-x + (float)i * 0.2, (float)-y, (float)-z)
		);
		objects[i].transform(transform);
		objects[i].get_avg_vertex_pos(&x, &y, &z);
		std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
		meshes.push_back(objects[i].to_mesh());
	}
}

void Player::draw() const {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].draw();
	}
}

void Player::add_segment(int joint_start, int joint_end, int model) {
	segment_ids[model] = segment_ids.size();
	add_segment(joint_start, joint_end);
}

void Player::add_segment(int joint_start, int joint_end) {
	segments.push_back(Segment {
		joint_start,
		joint_end,
		length(nodes[joint_start].pos - nodes[joint_end].pos)
	});
}