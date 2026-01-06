#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv_in;
layout (location = 2) in vec3 normal_in;

out vec2 uv;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	uv = uv_in;
	normal = normal_in;
	gl_Position = projection * view * model * vec4(pos, 1.0);
}