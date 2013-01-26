#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream> //Using the standard output for fprintf
#include <functional>

#include "Shader.hpp"
#include "Program.hpp"
#include "Sierpinski.cpp"

GLint attribute;
std::shared_ptr<cs5400::Program> program;

/* Assembles the vertex data and shoves the vertices to the vertex shader */
void setupVertexData()
{
	glEnableVertexAttribArray(attribute);
	auto verticesData = getVertices();

	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(
		attribute,					// attribute
		verticesData.first,			// number of elements per vertex, here (x,y)
		GL_FLOAT,					// the type of each element
		GL_FALSE,					// take our values as-is
		0,							// no extra data between each position
		verticesData.second.data()	// pointer to the C array
	);
 
	/* Push all the vertices to the vertex shader */
	glDrawArrays(GL_LINES, 0, verticesData.second.size() / verticesData.first);
	glDisableVertexAttribArray(attribute);
}



/* Clears the background and prepares the screen for a new drawing */
void resetDisplay()
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //Clear the background as white
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program->getHandle()); //Tell OpenGL which program to use
}


void onIdle()
{

}


/* Called when the object needs to be redrawn, such as during a resize */
void onDisplay()
{
	resetDisplay();
	setupVertexData();
	glutSwapBuffers(); //display the result
}



/* Load vertex and fragment shaders, and sets the display function */
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
		//glutIdleFunc(onIdle);
		return true;
	}
	catch(std::exception& e)
	{
		std::cerr << e.what();
		return false;
	}
}



/* Initializes glew */
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



/* Initializes glut. Sets the window size and title to the specified values */
void initializeGlutWindow(int width, int height, const std::string& windowTitle)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow(windowTitle.c_str());
}



/* Entry point for the program. Initializes the window and launches display */
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	initializeGlutWindow(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), "OpenGL Application by Jesse Victors");

	//try to initialize extension wrangler and the shaders
	if (!initializeWrangler() || !initializeResources())
		return EXIT_FAILURE;

	glutMainLoop();
	
	return EXIT_SUCCESS;
}