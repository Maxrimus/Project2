#version 430
out vec4 fragColor;
uniform vec4 color;
in vec4 vertColor;
void main()
{
	fragColor = color * vertColor;
}