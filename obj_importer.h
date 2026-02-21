#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include <vector>
#include <string>
#include "mesh.h"

using std::vector;
using std::string;

/// @brief An object contained within an OBJ file
/// Each object has a name and mesh associated with it
struct Object {
	string Name;
	Mesh mesh;
};

/// @brief  Load an OBJ file from filepath
/// @param obj_path The filepath to load
/// @return A vector of objects contained within the file
vector<Object> obj_importer(const string& obj_path);

#endif