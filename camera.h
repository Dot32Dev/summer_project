#include <glm/glm.hpp>

class Camera {
	public:
		Camera(glm::vec3 pos);
		glm::mat4 get_view_matrix();
		void pos_input(glm::vec3 input_vector);
		void dir_input(glm::vec2 input_vector);
	private:
		Camera();
		glm::vec3 pos;
		glm::vec2 dir;
};