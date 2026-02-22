#include "player.h"
#include "obj_importer.h"
#include "object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Player::Player(Uniform* transform_uniform) : 
	nodes(vector<Node>()),
	node_names(map<string, int>()),
	meshes(vector<Mesh>()),
	transform_uniform(transform_uniform)
{
	vector<Object> joints = obj_importer("res/Player/Joints.obj");
	for (int i=0; i<joints.size(); i++) {
		float x, y, z;
		joints[i].get_avg_vertex_pos(&x, &y, &z);
		nodes.push_back(Node {x, y, z, x, y, z});
		node_names.insert({joints[i].get_name(), i});
	}

	vector<Object> model = obj_importer("res/Player/Player.obj");
	for (int i=0; i<model.size(); i++) {
		float x, y, z;
		model[i].get_avg_vertex_pos(&x, &y, &z);
		glm::mat4 transform = glm::mat4(1.0);
		transform = glm::translate(
			transform, 
			glm::vec3((float)-x + (float)i * 0.2, (float)-y, (float)-z)
		);
		model[i].transform(transform);
		model[i].get_avg_vertex_pos(&x, &y, &z);
		std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
		meshes.push_back(model[i].to_mesh());
	}
}

void Player::draw() const {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].draw();
	}
}