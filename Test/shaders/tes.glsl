#version 460 core
layout (triangles, equal_spacing, ccw) in;
in VS_OUT {
vec4 color;
} tes_in;

void main(void) { 
gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +
				gl_TessCoord.y * gl_in[1].gl_Position +
				gl_TessCoord.z * gl_in[2].gl_Position);
}