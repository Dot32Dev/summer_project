#include <vector>

using std::vector;

class Mesh {
	public:
		Mesh(vector<float> vertices, vector<unsigned int> indices);
		Mesh(vector<float> vertices);
		void draw();
	private:
		Mesh();
		int num_indices;
		unsigned int vao_id; 
};