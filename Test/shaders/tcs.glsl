#version 460 core
layout (vertices = 3) out;
in VS_OUT {
vec4 color;
} tcs_in;

void main(void) {
// Only if I am invocation 0 ...    
if (gl_InvocationID == 0)    {
gl_TessLevelInner[0] = 7.0;
gl_TessLevelOuter[0] = 7.0; 
gl_TessLevelOuter[1] = 7.0;
gl_TessLevelOuter[2] = 7.0;
}    // Everybody copies their input to their output 
gl_out[gl_InvocationID].gl_Position =  gl_in[gl_InvocationID].gl_Position; 
}
