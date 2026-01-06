#include "obj_importer.h"
#include "mesh.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

using std::ifstream;
using std::stringstream;
using std::unordered_map;

struct Position {
	float x, y, z;
};

struct TexCoord {
	float u, v;
};

struct Normal {
	float x, y, z;
};

// Used as key in the vertex -> index hashmap
// Actual vertices are pushed to the vertex array as individual integers
struct VertexKey {
	Position pos;
	TexCoord tex_coord;
	Normal norm;
};

// Stupid C++
// You have to define all this just to use a hashmap
template <class T>
inline void hash_combine(size_t& seed, const T& v) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
	template <>
	struct hash<VertexKey> {
		size_t operator()(const VertexKey& key) const {
			size_t seed = 32;
			hash_combine(seed, key.pos.x);
			hash_combine(seed, key.pos.y);
			hash_combine(seed, key.pos.z);
			hash_combine(seed, key.tex_coord.u);
			hash_combine(seed, key.tex_coord.v);
			hash_combine(seed, key.norm.x);
			hash_combine(seed, key.norm.y);
			hash_combine(seed, key.norm.z);
			return seed;
		}
	};
}

inline bool operator==(const VertexKey& a, const VertexKey& b) {
    return a.pos.x == b.pos.x && a.pos.y == b.pos.y && a.pos.z == b.pos.z &&
        a.tex_coord.u == b.tex_coord.u && a.tex_coord.v == b.tex_coord.v &&
        a.norm.x == b.norm.x && a.norm.y == b.norm.y && a.norm.z == b.norm.z;
}

vector<Mesh> obj_importer(const string& obj_path) {
	ifstream file = ifstream(obj_path);
	if (!file) throw std::invalid_argument("Could not read file: " + obj_path);

	// Used when the OBJ file later references indices to things
	vector<Position> temp_vertices = vector<Position>();
	vector<TexCoord> temp_tex_coords = vector<TexCoord>();
	vector<Normal> temp_normals = vector<Normal>();
	// Default values for faces missing information
	temp_tex_coords.push_back(TexCoord {0.0, 0.0});
	temp_normals.push_back(Normal {0.0, 1.0, 0.0});
	// For garunteeing each vertex is unique, for optimal indexed rendering
	unordered_map<VertexKey, int> vertex_hash = unordered_map<VertexKey, int>();
	// The final data sent to the Mesh builder
	vector<float> vertex_data;
	vector<unsigned int> index_data;

	int next_index = 0;

	string line;
	while (getline(file, line)) {
		stringstream line_stream = stringstream(line);
		string field;
		getline(line_stream, field, ' ');

		// New object being defined
		if (field == "o") {

		}
		// Vertex position
		if (field == "v") {
			getline(line_stream, field, ' ');
			float x = stof(field);
			getline(line_stream, field, ' ');
			float y = stof(field);
			getline(line_stream, field, ' ');
			float z = stof(field);
			temp_vertices.push_back(Position {x, y, z});
		}
		// Normal
		if (field == "vn") {
			getline(line_stream, field, ' ');
			float x = stof(field);
			getline(line_stream, field, ' ');
			float y = stof(field);
			getline(line_stream, field, ' ');
			float z = stof(field);
			temp_normals.push_back(Normal {x, y, z});
		}
		// Texture coordinate
		if (field == "vt") {
			getline(line_stream, field, ' ');
			float u = stof(field);
			getline(line_stream, field, ' ');
			float v = stof(field);
			temp_tex_coords.push_back(TexCoord {u, v});
		}
		// Real shit goes down here
		if (field == "f") {
			// bool defined_normals = false;
			int first_vertex = -1;
			int last_vertex = -1;
			int i = 0;
			// For each vertex in the face..
			while (getline(line_stream, field, ' ')) {
				stringstream vertex_desc(field);
				string part;
				VertexKey vertex;
				// For each value in the vertex..
				for (int i = 0; getline(vertex_desc, part, '/'); i++) {
					if (!part.empty()) {
						int index = stoi(part);
						if (i == 0) {
							// Indexes in OBJ files are 1-based. We subtract one
							// to convert it to zero based
							vertex.pos = temp_vertices[index - 1];
						}
						if (i == 1) {
							// We don't convert this to zero based as we added
							// a 0, 0 texture coordinate as the 0th item.
							vertex.tex_coord = temp_tex_coords[index];
						}
						if (i == 2) {
							vertex.norm = temp_normals[index];
							// defined_normals = true;
						}

					} else {
						if (i == 1) {
							// If there are no texture coordinates for this 
							// face, add a texture coordinate from index zero 
							vertex.tex_coord = temp_tex_coords[0];
						}
					}
				}

				int current_vertex;
				if (vertex_hash.count(vertex) == 1) {
					current_vertex = vertex_hash[vertex];
				} else {
					current_vertex = next_index ++;
					vertex_hash[vertex] = current_vertex;
					vertex_data.push_back(vertex.pos.x);
					vertex_data.push_back(vertex.pos.y);
					vertex_data.push_back(vertex.pos.z);
					vertex_data.push_back(vertex.tex_coord.u);
					vertex_data.push_back(vertex.tex_coord.v);
					vertex_data.push_back(vertex.norm.x);
					vertex_data.push_back(vertex.norm.y);
					vertex_data.push_back(vertex.norm.z);
				}

				if (first_vertex == -1) first_vertex = current_vertex;

				// From the third vertex and onwards, aka >= index 2, aka > 1
				if (i > 1) {
					// Triangle fan type beat
					index_data.push_back(first_vertex);
					index_data.push_back(last_vertex);
					index_data.push_back(current_vertex);
				}

				last_vertex = current_vertex;
				i ++;
			}
		}
	}

	int bytes_with_indexing = vertex_data.size() * 4 + index_data.size() * 4;
	int bytes_default = index_data.size() * 8 * 4;
	std::cout << "Bytes with indexing: " << bytes_with_indexing << std::endl;
	std::cout << "Bytes without indexing: " << bytes_default << std::endl;

	Mesh mesh = Mesh(vertex_data, index_data);
	vector<Mesh> meshes = vector<Mesh>();
	meshes.push_back(mesh);
	return meshes;
}