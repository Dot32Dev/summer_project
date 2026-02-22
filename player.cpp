#include "player.h"
#include "obj_importer.h"
#include "object.h"

Player::Player(Uniform* transform_uniform) : 
	nodes(vector<Node>()),
	node_names(map<string, int>()),
	meshes(vector<Mesh>()),
	transform_uniform(transform_uniform)
{
	vector<Object> joints = obj_importer("res/Player/Joints.obj");
	for (int i=0; i<joints.size(); i++) {
		int x, y, z;
		joints[i].get_average_vertex_pos(&x, &y, &z);
		nodes.push_back(Node {x, y, z, x, y, z});
		node_names.insert({joints[i].get_name(), i});
	}

	vector<Object> model = obj_importer("res/Player/Player.obj");
	for (int i=0; i<model.size(); i++) {
		meshes.push_back(model[i].to_mesh());
	}
}

void Player::draw() const {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].draw();
	}
}