#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 col;

in vec3 normal;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
	gl_Position = projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
	vs_out.normal = vec3(projection*view*model * vec4(normal,1.0));
	color = col;
}