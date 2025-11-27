#version 330 core

out vec4 frag_colour;
in vec2 uv;

uniform vec4 our_colour;
uniform sampler2D our_texture;

void main() {
	// frag_colour = vec4(1.0, 0.5, 0.2, 1.0);
	frag_colour = our_colour * texture(our_texture, uv);
}