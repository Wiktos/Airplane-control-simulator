#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <irrKlang\irrKlang.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <windows.h>

using namespace std;

#include "Model.h"
#include "camera.h"

static void processInput(GLFWwindow* window);

Camera camera(glm::vec3(0.00122279, 2.84064f, 5.60991f));
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (glfwInit() != GL_TRUE)
	{
		cout << "Failed to init GLFW" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	try
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Airplane control symulator", monitor, nullptr);
		if (window == nullptr)
		{
			throw runtime_error("Failed to create window");
		}
		glfwMakeContextCurrent(window);
		glViewport(0, 0, mode->width, mode->height);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			cout << "Failed to init GLEW" << endl;
			return -1;
		}

		ShaderProgram airplaneShader("./Resource Files/shaders/airplaneVertexShader.vert",
			"./Resource Files/shaders/airplaneFragmentShader.frag");

		Model boeing727("./Resource Files/models/boeing727/Boeing727.obj");
		
		irrklang::ISoundEngine* symulatorSoundEnginee = irrklang::createIrrKlangDevice();
		symulatorSoundEnginee->play2D("./Resource Files/sounds/avion_sound.mp3", GL_TRUE);
		Sleep(100);
		symulatorSoundEnginee->play2D("./Resource Files/sounds/avion_sound.mp3", GL_TRUE);

		while (!glfwWindowShouldClose(window))
		{
			GLfloat currFrame = glfwGetTime();
			deltaTime = currFrame - lastFrame;
			lastFrame = currFrame;

			glfwPollEvents();
			glfwSwapBuffers(window);
			processInput(window);

			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 model;
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
			model = glm::translate(model, glm::vec3(-10.0f, -30.0f, 0.0f));

			glm::mat4 view;
			view = camera.viewMatrix();

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (GLfloat)(mode->width / mode->height), 0.1f, 100.0f);

			airplaneShader.use();
			airplaneShader.setMat4("model", model);
			airplaneShader.setMat4("view", view);
			airplaneShader.setMat4("projection", projection);

			boeing727.draw(airplaneShader);
		}

		symulatorSoundEnginee->drop();
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}
 

	glfwTerminate(); 
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.moveForward(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.moveBackward(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.moveRight(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.moveLeft(deltaTime);
	}
}