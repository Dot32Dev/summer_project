#include "shader.h"

using std::cerr;
using std::endl;

map<string, unsigned int> Shader::shaders;

Shader::Shader(const string& vertex_path, const string& fragment_path) {
	unsigned int vert_shader_id;
	if (shaders.count(vertex_path) > 0) {
		vert_shader_id = shaders[vertex_path];
	} else {
		// Load from file
		ifstream file("vert.glsl");
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
		ifstream file("frag.glsl");
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

		shaders[fragment_path] = vert_shader_id;
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
	glUseProgram(program_id);
}

Uniform Shader::get_uniform(const string& uniform) {
	return Uniform(program_id, uniform);
}

Uniform::Uniform(unsigned int program_id, const string& uniform) : 
	uniform_location(glGetUniformLocation(program_id, uniform.c_str()))
{}

void Uniform::send(float arg1, float arg2, float arg3, float arg4) {
	glUniform4f(uniform_location, arg1, arg2, arg3, arg4);
}

void Uniform::send(float arg1, float arg2, float arg3) {
	glUniform3f(uniform_location, arg1, arg2, arg3);
}

void Uniform::send(float arg1, float arg2) {
	glUniform2f(uniform_location, arg1, arg2);
}

void Uniform::send(float arg1) {
	glUniform1f(uniform_location, arg1);
}

void Uniform::send(int arg1, int arg2, int arg3, int arg4) {
	glUniform4i(uniform_location, arg1, arg2, arg3, arg4);
}

void Uniform::send(int arg1, int arg2, int arg3) {
	glUniform3i(uniform_location, arg1, arg2, arg3);
}

void Uniform::send(int arg1, int arg2) {
	glUniform2i(uniform_location, arg1, arg2);
}

void Uniform::send(int arg1) {
	glUniform1i(uniform_location, arg1);
}