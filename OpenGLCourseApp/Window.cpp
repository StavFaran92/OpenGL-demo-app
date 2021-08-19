#include "Window.h"

Window::Window() :
	width(800), height(600) 
{

	for (size_t i = 0; i < 1024; i++) 
	{
		keys[i] = 0;
	}
}

Window::Window(GLint width, GLint height) {
	this->width = width;
	this->height = height;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::initialize() 
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle keys and mouse
	CreateCallback();
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

void Window::CreateCallback()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::GetXChange() {
	GLfloat change = xChange;
	xChange = 0;
	return change;
}

GLfloat Window::GetYChange() {
	GLfloat change = yChange;
	yChange = 0;
	return change;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
		{
			myWindow->keys[key] = true;
			printf("Pressed %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			myWindow->keys[key] = false;
			printf("Released %d\n", key);
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (myWindow->mouseFirstMoved)
	{
		myWindow->lastX = xPos;
		myWindow->lastY = yPos;
		myWindow->mouseFirstMoved = false;
	}

	myWindow->xChange = xPos - myWindow->lastX;
	myWindow->yChange = myWindow->lastY - yPos;

	myWindow->lastX = xPos;
	myWindow->lastY = yPos;
}



Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}