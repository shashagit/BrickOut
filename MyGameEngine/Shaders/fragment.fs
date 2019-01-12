#version 430 core
in vec2 verTexCoord;
out vec4 FragColor;

uniform sampler2D tex;

void main() 
{ 
	FragColor = texture(tex, verTexCoord); 
}