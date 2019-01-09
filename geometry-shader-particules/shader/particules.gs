#version 330 core
layout (triangles) in;
layout (line_strip,max_vertices=5) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

out vec3 color;

void main()
{
	gl_Position=vec4(gs_in[0].normal,1.0);
	EmitVertex();
	
	gl_Position=vec4(gs_in[0].normal,1.0)+vec4(0,0,gs_in[0].normal.z,1.0);
	EmitVertex();

	color=vec3(0,1,0);

	EndPrimitive();

}