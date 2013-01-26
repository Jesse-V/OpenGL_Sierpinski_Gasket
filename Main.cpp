#include <chrono>
#include <thread>

#include "Angel.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

const int NumVertices = 36;

point4 points[NumVertices];
color4 colors[NumVertices];

point4	vertex_positions[8]	=	{	
				point4(	-0.5,	-0.5,	 0.5,	1.0	),	
				point4(	-0.5,	 0.5,	 0.5,	1.0	),	
				point4(	 0.5,	 0.5,	 0.5,	1.0	),	
				point4(	 0.5,	-0.5,	 0.5,	1.0	),	
				point4(	-0.5,	-0.5,	-0.5,	1.0	),	
				point4(	-0.5,	 0.5,	-0.5,	1.0	),	
				point4(	 0.5,	 0.5,	-0.5,	1.0	),	
				point4(	 0.5,	-0.5,	-0.5,	1.0	)	
};

color4	vertex_colors[8]	=	{	
				color4(0.0,	0.0,	0.0,	1.0),	//	black	
				color4(1.0,	0.0,	0.0,	1.0),	//	red	
				color4(1.0,	1.0,	0.0,	1.0),	//	yellow	
				color4(0.0,	1.0,	0.0,	1.0),	//	green	
				color4(0.0,	0.0,	1.0,	1.0),	//	blue	
				color4(1.0,	0.0,	1.0,	1.0),	//	magenta	
				color4(1.0,	1.0,	1.0,	1.0),	//	white	
				color4(0.0,	1.0,	1.0,	1.0)	//	cyan	
};

enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int Axis = Xaxis;
GLfloat Theta[NumAxes] = {0.0, 0.0, 0.0};

GLuint theta;

int Index = 0;  // global variable indexing into VBO arrays 
void quad(int a, int b, int c, int d)
{ 
    colors[Index] = vertex_colors[a]; points[Index] = vertex_positions[a]; Index++; 
    colors[Index] = vertex_colors[b]; points[Index] = vertex_positions[b]; Index++; 
    colors[Index] = vertex_colors[c]; points[Index] = vertex_positions[c]; Index++; 
    colors[Index] = vertex_colors[a]; points[Index] = vertex_positions[a]; Index++; 
    colors[Index] = vertex_colors[c]; points[Index] = vertex_positions[c]; Index++; 
    colors[Index] = vertex_colors[d]; points[Index] = vertex_positions[d]; Index++; 
}

void colorcube()
{	
	quad(1,	0,	3,	2);	
	quad(2,	3,	7,	6);	
	quad(3,	0,	4,	7);	
	quad(6,	5,	1,	2);	
	quad(4,	5,	6,	7);	
	quad(5,	4,	0,	1);	
}

void init(void)
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3fv(theta, 1, Theta);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 033: //escape key
		case 'q': case 'Q':
			exit(EXIT_SUCCESS);
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
			case GLUT_LEFT_BUTTON:		Axis = Xaxis;	break;
			case GLUT_MIDDLE_BUTTON:	Axis = Yaxis;	break;
			case GLUT_RIGHT_BUTTON:		Axis = Zaxis;	break;
		}
	}
}

void idle(void)
{
	Theta[Axis] += 0.8;

	if (Theta[Axis] > 360.0)
		Theta[Axis] -= 360.0;

	std::chrono::milliseconds duration(50); //20 fps
	std::this_thread::sleep_for(duration); //forget time.h or windows.h, this is the real way to sleep!

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Color Cube");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

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



/* Initializes glut. Sets the window size and title to the specified values 
void initializeGlutWindow(int width, int height, const std::string& windowTitle)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow(windowTitle.c_str());
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