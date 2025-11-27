#ifndef SHADER_H
#define SHADER_H

// For loading the shader files
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
// For OpenGL
#include <GL/glew.h>

using std::ifstream;
using std::stringstream;
using std::string;
using std::map;

class Uniform {
	public:
		// C++ has function overloading :)
		// Assumes the relevant shader is currently bound
		// Not going to individually doc comment these for taking too much space
		void send(float arg1, float arg2, float arg3, float arg4);
		void send(float arg1, float arg2, float arg3);
		void send(float arg1, float arg2);
		void send(float arg1);
		void send(int arg1, int arg2, int arg3, int arg4);
		void send(int arg1, int arg2, int arg3);
		void send(int arg1, int arg2);
		void send(int arg1);
	private:
		friend class Shader; // So that only the shader may construct Uniforms
		Uniform(unsigned int program_id, const string& uniform);
		Uniform();
		int uniform_location;
};

class Shader {
	public:
		/// @brief Create a new shader from shader files
		/// @param vertex_path The file path to the vertex shader
		/// @param fragment_path The file path to the fragment shader
		Shader(const string& vertex_path, const string& fragment_path);

		/// @brief Enable the shader
		void use();

		/// @brief The name of the uniform
		/// @param uniform The string variable name of the uniform
		/// @return a Uniform object which can be used to send values to shader
		Uniform get_uniform(const string& uniform);
	private:
		Shader();
		// Using a static map means that if you create various shaders with the 
		// same vertex shader file, it won't have to reload that file from disk
		// each time.
		static map<string, unsigned int> shaders;
		unsigned int program_id;
};

#endif // SHADER_H