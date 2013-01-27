#include <chrono>
#include <thread>
#include <cmath>
#include <vector>
#include <assert.h>

#include "Shader.h"
#include "Triangle.struct"
#include "Sierpinski.cpp"

const int ROTATION_SPEED = 1;
const float ZOOM = 0.75f;
GLfloat rotation[3] = {114, 0, 16}; //good initial view

GLuint theta;


void init()
{
	auto vertices = getVertices();
	auto colors = colorModel(vertices);
	assert(vertices.size() == colors.size()); //just to double-check

	Point pointsArray[NUM_VERTICES];
	Point colorsArray[NUM_VERTICES];
	for (int j = 0; j < NUM_VERTICES; j++)
	{
		pointsArray[j] = vertices[j] * ZOOM;
		colorsArray[j] = colors[j] * ZOOM;
	}

	GLuint program = InitShader("vertex.glsl", "fragment.glsl");
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointsArray) + sizeof(colorsArray), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsArray), pointsArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsArray), sizeof(colorsArray), colorsArray);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsArray)));

	theta = glGetUniformLocation(program, "theta");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}



void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniform3fv(theta, 1, rotation); //apply view angle
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
	
	glutSwapBuffers();
}



void keyboardInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			rotation[0] += ROTATION_SPEED; //add to X axis
			break;

		case 's':
			rotation[0] -= ROTATION_SPEED; //subtract from X axis
			break;

		case 'd':
			rotation[1] += ROTATION_SPEED; //add to Y axis
			break;

		case 'a':
			rotation[1] -= ROTATION_SPEED; //subtract from Y axis
			break;

		case 'e':
			rotation[2] += ROTATION_SPEED; //add to Z axis
			break;

		case 'q':
			rotation[2] -= ROTATION_SPEED; //subtract from Z axis
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