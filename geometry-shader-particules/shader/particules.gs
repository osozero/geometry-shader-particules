#version core 330

layout (triangles) in;
layout (line_strip,max_vertices=5) out;

uniform vec3 norm;

out vec3 color;

void main()
{
	gl_Position= gl_in[0].gl_Position + vec4(0.1,0,0,0);
	EmitVertex();
	
	gl_Position= gl_in[0].gl_Position + vec4(0.0,-0.4,0,0);
	EmitVertex();

	color=vec3(1,0,0);

	EndPrimitive();

}