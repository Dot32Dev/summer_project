#version 330 core

out vec4 frag_colour;
in vec2 uv;
in vec3 normal;

uniform vec4 our_colour;
uniform sampler2D our_texture;

void main() {
	// frag_colour = vec4(1.0, 0.5, 0.2, 1.0);
	// frag_colour = our_colour * texture(our_texture, uv);
	frag_colour = vec4(normal, 1.0) * our_colour * texture(our_texture, uv);
}