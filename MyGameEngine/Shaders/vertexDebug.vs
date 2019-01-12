#version 430 core
layout(location = 0) in vec2 aPos;

layout(location = 2) uniform mat4 transformx;
layout(location = 3) uniform mat4 projection;

void main() 
{ 
	gl_Position = projection * transformx * vec4(aPos, 0.0, 1.0); 
}