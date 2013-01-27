#include <chrono>
#include <thread>
#include <cmath>

#include "Shader.h"
#include "Triangle.struct"

const int SPEED = 1;
const int NumVertices = 36;
GLfloat rotation[3] = {45.0, -45.0, 0.0};

GLuint theta;

Point points[NumVertices];
Point colors[NumVertices];


const Point vertex_positions[8] = 
{	
	{   -0.5,	-0.5,	 0.5},
	{	-0.5,	 0.5,	 0.5},
	{	 0.5,	 0.5,	 0.5},
	{	 0.5,	-0.5,	 0.5},
	{	-0.5,	-0.5,	-0.5},
	{	-0.5,	 0.5,	-0.5},
	{	 0.5,	 0.5,	-0.5},
	{	 0.5,	-0.5,	-0.5}
};


//generates two triangles for each face
void createFace(int a, int b, int c, int d, int& index)
{
	points[index++] = vertex_positions[a]; 
	points[index++] = vertex_positions[b];
	points[index++] = vertex_positions[c]; 
	
	points[index++] = vertex_positions[a]; 
	points[index++] = vertex_positions[c];
	points[index++] = vertex_positions[d];
}


//generates 12 triangles: 36 vertices
void generateCube()
{
	int index = 0;
	createFace(1,	0,	3,	2, index);	
	createFace(2,	3,	7,	6, index);	
	createFace(3,	0,	4,	7, index);	
	createFace(6,	5,	1,	2, index);	
	createFace(4,	5,	6,	7, index);	
	createFace(5,	4,	0,	1, index);
}


float scale(float val, int begin, int end)
{
	return val * (end - begin) + begin;
}


void colorCube()
{
	for (int j = 0; j < NumVertices; j++)
	{
		float val = points[j].y + 0.5;
		
		if (val > 0.5)
		{
			//colors[j] = color4(238 / 255.0,	139 / 255.0, 34 / 255.0);
			colors[j] = {
				scale(val, 160, 34) / 255,
				scale(val, 82, 139) / 255,
				scale(val, 45, 34) / 255
				};
		}
		else
			colors[j] = {val,	val, val};
	}
}



void init()
{
	generateCube();
	colorCube();

	GLuint program = InitShader("vertex.glsl", "fragment.glsl");
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	theta = glGetUniformLocation(program, "theta");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}



void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniform3fv(theta, 1, rotation); //apply view angle
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	
	glutSwapBuffers();
}



void keyboardInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			rotation[0] += SPEED; //add to X axis
			break;

		case 's':
			rotation[0] -= SPEED; //subtract from X axis
			break;

		case 'd':
			rotation[1] += SPEED; //add to Y axis
			break;

		case 'a':
			rotation[1] -= SPEED; //subtract from Y axis
			break;

		case 'e':
			rotation[2] += SPEED; //add to Z axis
			break;

		case 'q':
			rotation[2] -= SPEED; //subtract from Z axis
			break;
	}

	rotation[0] = (rotation[0] < 360.0) ? rotation[0] : rotation[0] - 360;
	rotation[1] = (rotation[1] < 360.0) ? rotation[1] : rotation[0] - 360;
	rotation[2] = (rotation[2] < 360.0) ? rotation[2] : rotation[0] - 360;
}



void sleep(int milliseconds)
{
	std::chrono::milliseconds duration(milliseconds);
	std::this_thread::sleep_for(duration); //forget time.h or windows.h, this is the real way to sleep!
}



void onIdle()
{
	sleep(50); //20 fps
	glutPostRedisplay();
}



/* Initializes glut. Sets the window size and title to the specified values */
void initializeGlutWindow(int width, int height, const std::string& windowTitle)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow(windowTitle.c_str());
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	int windowHeight = glutGet(GLUT_SCREEN_HEIGHT) - 20;
	initializeGlutWindow(windowHeight, windowHeight, "OpenGL Application by Jesse Victors");

	glewInit();
	init();

	glutDisplayFunc(render);
	glutKeyboardFunc(keyboardInput);
	glutIdleFunc(onIdle);

	glutMainLoop();
	return 0;
}