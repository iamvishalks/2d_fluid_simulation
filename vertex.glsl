#version 330 core

void main()
{
    layout(location  = 0) in vec2 position;
    layout(location = 1) in vec3 color; 
    gl_Position = vec4(
        0.5*(2.0 * float(gl_VertexID & 1) - 1), 
        0.5*(2.0 * float((gl_VertexID >> 1) & 1)-1),
        0.0,  
        1.0);
}