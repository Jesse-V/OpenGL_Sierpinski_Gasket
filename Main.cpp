#include <chrono>
#include <thread>

#include "vector.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

const int SPEED = 1;
const int NumVertices = 36;
GLfloat rotation[3] = {45.0, -45.0, 0.0};

GLuint theta;

point4 points[NumVertices];
color4 colors[NumVertices];


const point4 vertex_positions[8] = 
{	
	point4(	-0.5,	-0.5,	 0.5),	
	point4(	-0.5,	 0.5,	 0.5),	
	point4(	 0.5,	 0.5,	 0.5),	
	point4(	 0.5,	-0.5,	 0.5),	
	point4(	-0.5,	-0.5,	-0.5),	
	point4(	-0.5,	 0.5,	-0.5),	
	point4(	 0.5,	 0.5,	-0.5),	
	point4(	 0.5,	-0.5,	-0.5)	
};


const color4 vertex_colors[8] = 
{	
	color4(0.0,		0.0,	0.0),	//black	
	color4(1.0,		0.0,	0.0),	//red	
	color4(1.0,		1.0,	0.0),	//yellow	
	color4(0.0,		1.0,	0.0),	//green	
	color4(0.0,		0.0,	1.0),	//blue	
	color4(1.0,		0.0,	1.0),	//magenta	
	color4(1.0,		1.0,	1.0),	//white	
	color4(0.0,		1.0,	1.0)	//cyan	
};


//generates two triangles for each face and assigns colors to the vertices
//int Index = 0;  // global variable indexing into VBO arrays 
void quad(int a, int b, int c, int d, int& index)
{
	colors[index] = vertex_colors[a]; points[index] = vertex_positions[a]; index++; 
	colors[index] = vertex_colors[b]; points[index] = vertex_positions[b]; index++; 
	colors[index] = vertex_colors[c]; points[index] = vertex_positions[c]; index++; 
	
	colors[index] = vertex_colors[a]; points[index] = vertex_positions[a]; index++; 
	colors[index] = vertex_colors[c]; points[index] = vertex_positions[c]; index++; 
	colors[index] = vertex_colors[d]; points[index] = vertex_positions[d]; index++; 
}


//generates 12 triangles: 36 vertices and 36 colors
void colorcube()
{
	int index = 0;
	quad(1,	0,	3,	2, index);	
	quad(2,	3,	7,	6, index);	
	quad(3,	0,	4,	7, index);	
	quad(6,	5,	1,	2, index);	
	quad(4,	5,	6,	7, index);	
	quad(5,	4,	0,	1, index);
}



void init()
{
	colorcube();

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



/*
#include <GL/glut.h>

#include <iostream> //Using the standard output for fprintf
#include <functional>
#include <chrono>
#include <thread>

#include "Shader.hpp"
#include "Program.hpp"
#include "Sierpinski.cpp"

GLint attribute;
std::shared_ptr<cs5400::Program> program;

/* Assembles the vertex data and shoves the vertices to the vertex shader 
void setupVertexData()
{
	glEnableVertexAttribArray(attribute);
	auto verticesData = getVertices();

	/* Describe our vertices array to OpenGL (it can't guess its format automatically) 
	glVertexAttribPointer(
		attribute,					// attribute
		verticesData.first,			// number of elements per vertex, here (x,y)
		GL_FLOAT,					// the type of each element
		GL_FALSE,					// take our values as-is
		0,							// no extra data between each position
		verticesData.second.data()	// pointer to the C array
	);
 
	/* Push all the vertices to the vertex shader 
	glDrawArrays(GL_LINES, 0, verticesData.second.size() / verticesData.first);
	glDisableVertexAttribArray(attribute);
}



/* Clears the background and prepares the screen for a new drawing 
void resetDisplay()
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //Clear the background as white
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program->getHandle()); //Tell OpenGL which program to use
}


void onIdle()
{
	std::chrono::milliseconds duration(50); //20 fps
	std::this_thread::sleep_for(duration); //forget time.h or windows.h, this is the real way to sleep!

	std::cout << "onIdle..." << std::endl;
}


/* Called when the object needs to be redrawn, such as during a resize 
void onDisplay()
{
	resetDisplay();
	setupVertexData();
	glutSwapBuffers(); //display the result
}



/* Load vertex and fragment shaders, and sets the display function 
bool initializeResources()
{
	try
	{
		program = cs5400::make_program
		(
			cs5400::make_vertexShader("vertex.glsl"),
			cs5400::make_fragmentShader("fragment.glsl")   
		);

		glutDisplayFunc(onDisplay);
		glutIdleFunc(onIdle);
		return true;
	}
	catch(std::exception& e)
	{
		std::cerr << e.what();
		return false;
	}
}



/* Initializes glew 
bool initializeWrangler()
{
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return false;
	}

	return true;
}







/* Entry point for the program. Initializes the window and launches display 
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	initializeGlutWindow(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), "OpenGL Application by Jesse Victors");

	//try to initialize extension wrangler and the shaders
	if (!initializeWrangler() || !initializeResources())
		return EXIT_FAILURE;

	glutMainLoop();
	
	return EXIT_SUCCESS;
}*/