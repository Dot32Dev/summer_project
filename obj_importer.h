#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include <vector>
#include <string>
#include "mesh.h"

using std::vector;
using std::string;

vector<Mesh> obj_importer(const string& obj_path);

#endif