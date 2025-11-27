#include "shader.h"

using std::cerr;
using std::endl;

map<string, unsigned int> Shader::shaders;
unsigned int* Shader::shader_in_use;

Shader::Shader(const string& vertex_path, const string& fragment_path) : 
	allocated_textures(map<int, int>())
{
	unsigned int vert_shader_id;
	if (shaders.count(vertex_path) > 0) {
		vert_shader_id = shaders[vertex_path];
	} else {
		// Load from file
		ifstream file(vertex_path);
		stringstream buffer;
		buffer << file.rdbuf();
		string shader_string = buffer.str();
		const char* shader_chars = shader_string.c_str();

		// Compile the fella
		vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert_shader_id, 1, &shader_chars, NULL);
		glCompileShader(vert_shader_id);

		// Check for errors
		int success;
		glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			char err[1024]; glGetShaderInfoLog(vert_shader_id, 1024, NULL, err);
			cerr << "Vertex shader error:\n" << err << endl;
		}

		shaders[vertex_path] = vert_shader_id;
	}

	unsigned int frag_shader_id;
	if (shaders.count(fragment_path) > 0) {
		frag_shader_id = shaders[fragment_path];
	} else {
		// Load from file
		ifstream file(fragment_path);
		stringstream buffer;
		buffer << file.rdbuf();
		string shader_string = buffer.str();
		const char* shader_chars = shader_string.c_str();

		// Compile the fella
		frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader_id, 1, &shader_chars, NULL);
		glCompileShader(frag_shader_id);

		// Check for errors
		int success;
		glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			char err[1024]; 
			glGetShaderInfoLog(frag_shader_id, 1024, NULL, err);
			cerr << "Fragment shader error:\n" << err << endl;
		}

		shaders[fragment_path] = frag_shader_id;
	}

	// Compile shader program
	program_id = glCreateProgram();
	glAttachShader(program_id, vert_shader_id);
	glAttachShader(program_id, frag_shader_id);
	glLinkProgram(program_id);

	// Check for errors
	int success;
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024]; 
		glGetProgramInfoLog(program_id, 1024, NULL, infoLog);
        std::cerr << "Shader program link error:\n" << infoLog << endl;
    }
}

Shader::Shader() {}

void Shader::use() {
	shader_in_use = &program_id;
	glUseProgram(program_id);
}

Uniform Shader::get_uniform(const string& uniform) {
	return Uniform(program_id, uniform, this);
}

Uniform::Uniform(
	unsigned int program_id, 
	const string& uniform, 
	Shader* shader
) : 
	uniform_location(glGetUniformLocation(program_id, uniform.c_str())),
	shader(shader)
{}

void Uniform::send(float arg1, float arg2, float arg3, float arg4) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform4f(uniform_location, arg1, arg2, arg3, arg4);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(float arg1, float arg2, float arg3) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform3f(uniform_location, arg1, arg2, arg3);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(float arg1, float arg2) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform2f(uniform_location, arg1, arg2);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(float arg1) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform1f(uniform_location, arg1);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(int arg1, int arg2, int arg3, int arg4) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform4i(uniform_location, arg1, arg2, arg3, arg4);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(int arg1, int arg2, int arg3) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform3i(uniform_location, arg1, arg2, arg3);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(int arg1, int arg2) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform2i(uniform_location, arg1, arg2);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(int arg1) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }
	glUniform1i(uniform_location, arg1);
	if (inactive) { glUseProgram(*shader->shader_in_use); }
}

void Uniform::send(Texture& texture) {
	int inactive = shader->program_id != *shader->shader_in_use; 
	if (inactive) { glUseProgram(shader->program_id); }

	// Stores uniform location to texure binding
	if (shader->allocated_textures.count(uniform_location) == 0) {
		int count = shader->allocated_textures.size();
		shader->allocated_textures[uniform_location] = count;
	}
	int binding = shader->allocated_textures[uniform_location];

	if (binding > 15) {
		cerr << "Some graphics cards do not support this many textures" << endl;
	}

	glActiveTexture(GL_TEXTURE0 + binding);
	glBindTexture(GL_TEXTURE_2D, texture.texture_id);

	glUniform1i(uniform_location, binding);

	if (inactive) { glUseProgram(*shader->shader_in_use); }
}