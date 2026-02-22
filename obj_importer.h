#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include <vector>
#include <string>
#include "object.h"

using std::vector;
using std::string;

/// @brief Load an OBJ file from filepath
/// The output of the loader is a vector of objects. These objects 
/// retain name and vertex data info and can have operations performed on them.
/// In order to draw, a mesh must be generated from an object. Meshes are stored
/// on the GPU and therefore cannot be modified after creation, nor can the
/// vertices or name of a mesh be seen.
/// @param obj_path The filepath to load
/// @return A vector of objects contained within the file
vector<Object> obj_importer(const string& obj_path);

#endif