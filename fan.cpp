#include <iostream>
#include <stdlib.h>
#include <glew.h>
#include <glfw3.h>
#include "Shader.h"
#include "Shape.h"
using namespace std;

Shape** shapes;
const int SHAPE_COUNT = 3;
int WINDOW_WIDTH = 1300, WINDOW_HEIGHT = 900;
Box* box;
Vase* vas;
Hierarchy fan(new Ovaloid(0, -0.6, 0, 35, Vertex(0.05, 0.5), 1));

GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
GLuint VertexArrayID;
void mouseMoveEvent(GLFWwindow* window, double x, double y)
{
	double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
	double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
	printf("X : %f, Y : %f\n", mod_x, mod_y);
}
void mouseClickEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
		double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
		if (action == GLFW_PRESS)
		{
			printf("LEFT CLICK ON : (%lf, %lf)\n", mod_x, mod_y);
		}
		else if (action == GLFW_RELEASE)
		{
			printf("LEFT RELEASE ON : (%lf, %lf)\n", mod_x, mod_y);
		}
	}
}
void screenResizeEvent(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
void keyboardPressEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
		fan.getParent()->translate(Vertex(-0.01, 0, 0));
		fan.getChild(0)->getParent()->translate(Vertex(-0.01, 0, 0));
	}
	else if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
		//DO SOMETHING HERE
	}
	else if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
		//DO SOMETHING HERE
	}
	else if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
		//DO SOMETHING HERE
	}
	else if (key == GLFW_KEY_Z && action == GLFW_REPEAT) {
		//DO SOMETHING HERE
	}
	else if (key == GLFW_KEY_X && action == GLFW_REPEAT) {
		//DO SOMETHING HERE
	}
}
void initializeGLFW() {
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
}
void initializeWindow() {
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Computer Graphics", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouseMoveEvent);
	glfwSetMouseButtonCallback(window, mouseClickEvent);
	glfwSetKeyCallback(window, keyboardPressEvent);
	glfwSetFramebufferSizeCallback(window, screenResizeEvent);

	glEnable(GL_DEPTH_TEST);
}
void initializeGLEW() {
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		return;
	}
}
void initializeShapes() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	shapes = new Shape * [SHAPE_COUNT];
	Vertex vertex[][3] =
	{
		{ Vertex(-0.7f, 0.3f), Vertex(-0.5, 0.7f), Vertex(-0.3f, 0.3f) }
	};
	Vertex control_pts[8] =
	{
		Vertex(0, -0.8),
		Vertex(-0.3, -0.6),
		Vertex(-0.5, -0.2),
		Vertex(-0.3, 0.2),
		Vertex(-0.15, 0.4),
		Vertex(-0.1, 0.5),
		Vertex(-0.15, 0.6),
		Vertex(-0.3, 0.75)
	};
	shapes[0] = new Triangle(vertex[0], -0.5f, 0.5f);
	shapes[1] = new Circle(0.5f, 0.5f, 0, 100, 0.1, 1);
	shapes[2] = new Ovaloid(-0.5f, -0.5f, 0.5);
	box = new Box(0.5, -0.5, 0);
	Vertex a[11] = { {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{1,0} };
	vas = new Vase(control_pts, 8, 0, 0, 0, 25, 1.0, 20);
	char vertexShader[][100] = { "shaders/circle/vertex.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_2.shader","shaders/triangle/vertex_1.shader" };
	char fragmentShader[][100] = { "shaders/triangle/.shader", "shaders/triangle/red.shader", "shaders/triangle/red.shader","shaders/triangle/red.shader" };
	char fragmentOutlineShader[][100] = { "shaders/circle/fragment_outline.shader", "shaders/triangle/fragment_outline_1.shader", "shaders/triangle/fragment_outline_2.shader","shaders/triangle/fragment_outline_1.shader" };

	/*for (int i = 0; i < SHAPE_COUNT; i++)
	{
		shapes[i]->initiateBuffer();
		shapes[i]->initiateShader(vertexShader[i], fragmentShader[i]);
		shapes[i]->initiateOutlineShader(vertexShader[i], fragmentOutlineShader[i]);
	}
	box->initiateBuffer();
	box->initiateShader(vertexShader[3], fragmentShader[3]);
	box->initiateOutlineShader(vertexShader[3], fragmentOutlineShader[3]);*/

	fan.rotate(fan.getParent()->getPosition(), fan.getParent()->getEuler(2), 90);
	fan.addChild(new Hierarchy(new Ovaloid(fan.getParent()->getPosition().x,
		fan.getParent()->getPosition().y + 0.4,
		0, 25, Vertex(0.4, 0.1), 1))); //tiang kipas angin
	fan.getChild(0)->rotate(fan.getChild(0)->getParent()->getPosition(),
		fan.getChild(0)->getParent()->getEuler(2), 90);
	fan.getChild(0)->addChild(new Hierarchy(new Ovaloid(
		fan.getChild(0)->getParent()->getPosition().x,
		fan.getChild(0)->getParent()->getPosition().y + 0.425,
		0, 10, Vertex(0.3, 0.1), 1))); //pangkal rotasi kipas angin
	fan.getChild(0)->getChild(0)->addChild(new Hierarchy(new Ovaloid(
		fan.getChild(0)->getChild(0)->getParent()->getPosition().x - 0.35,
		fan.getChild(0)->getChild(0)->getParent()->getPosition().y,
		0, 10, Vertex(0.05, 0.3), 1))); //baling baling kipas angin
	fan.initiateBuffer();
	fan.initiateShader(vertexShader[1],fragmentShader[1]);
	fan.initiateOutlineShader(vertexShader[1],fragmentOutlineShader[1]);
	fan.resetEuler();
	/*vas->rotate(vas->getPosition(), Vertex(1, 0, 0), 130);
	vas->initiateBuffer();
	vas->initiateShader(vertexShader[0], fragmentShader[0]);
	vas->initiateOutlineShader(vertexShader[1], fragmentShader[1]);
	vas->resetEuler();*/
}

void render() {
	glEnableVertexAttribArray(0);
	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*shapes[0]->rotate(shapes[0]->getPosition(), Vertex(1, 0, 0), 0.2);
		shapes[1]->rotate(shapes[1]->getPosition(), Vertex(0, 1, 0), 0.2);
		shapes[2]->rotate(shapes[2]->getPosition(), Vertex(0, 0, 1), 0.2);
		for (int i = 0; i < SHAPE_COUNT; i++) {
			shapes[i]->drawPolygon();
			shapes[i]->drawPolyline();
		}
		box->drawPolygon();
		box->drawPolyline();
		box->rotate(box->getPosition(), Vertex(1, 1, 0), 0.1);*/

		fan.drawPolygon();
		fan.drawPolyline();
		/*vas->drawPolygon();
		vas->drawPolyline();
		vas->rotate(vas->getPosition(), Vertex(1, 0, 0), 0.5);*/
		fan.getChild(0)->getChild(0)->getParent()->rotate(fan.getParent()->getPosition(), Vertex(0, 1, 0), 0.1);
		fan.getChild(0)->getChild(0)->getChild(0)->getParent()->rotate(fan.getParent()->getPosition(), Vertex(0, 1, 0), 0.1);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	glDisableVertexAttribArray(0);
}

void cleanup() {
	delete shapes;
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
}

int main()
{
	initializeGLFW();
	initializeWindow();
	initializeGLEW();
	initializeShapes();
	
	render();
}