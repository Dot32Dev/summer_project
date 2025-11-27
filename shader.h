#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <GL/glew.h>
#include "texture.h"

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
		void send(Texture& texture);
	private:
		friend class Shader; // So that only the shader may construct Uniforms
		Uniform(unsigned int program_id, const string& uniform, Shader* shader);
		Uniform();
		int uniform_location;
		// A pointer to the parent shader of the uniform so that shader-specific
		// operations may be made.
		Shader* shader;
};

class Shader {
	public:
		/// @brief Create a new shader from shader files
		/// @param vertex_path The file path to the vertex shader
		/// @param fragment_path The file path to the fragment shader
		Shader(const string& vertex_path, const string& fragment_path);

		/// @brief Enable the shader
		void use();

		/// @brief The name of the uniform to get the location of
		/// Returns a Uniform object which can be used to send values to the 
		/// shader. If this uniform is used after its shader is destroyed, there
		/// will be a use-after-free. Usage:
		/// Uniform colour = shader.get_uniform("colour");
		/// colour.send(1.0, 0.5, 0.1, 1.0); // Assumes colour is a vec4
		/// @param uniform The string variable name of the uniform
		/// @return a Uniform object which can be used to send values to shader
		Uniform get_uniform(const string& uniform);
	private:
		// Creating a shader with no shader files is not allowed
		Shader();
		// Using a static map means that if you create various shaders with the 
		// same vertex shader file, it won't have to reload that file from disk
		// each time.
		static map<string, unsigned int> shaders;
		// Storing the current shader which is in use can help prevent illegal
		// operations from being conducted when a shader is not in use
		static unsigned int* shader_in_use;
		// The OpenGL shader program ID
		unsigned int program_id;
		// Stores uniform location to texure binding
		// This is so that if we bind two images to the same uniform, or even 
		// the same image to the same uniform twice, we don't create two texture 
		// bindings for it, we just overwrite the image that was previously set.
		map<int, int> allocated_textures;
		// Friend because the Uniform needs to access the allocated textures map
		// (The reason allocated textures aren't stored in the Uniform as a 
		// static map is that they are shader-dependant.)
		friend class Uniform;
};

#endif // SHADER_H