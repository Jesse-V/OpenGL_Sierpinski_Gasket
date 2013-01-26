/*
void main(void) 
{
	gl_FragColor[0] = gl_FragColor[1] = gl_FragColor[2] = 0;
	//gl_Position
}*/

#version 150

in vec4 color;
out vec4 fColor;

void main()
{
	fColor = color;
}