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

#include "model.h"

constexpr static int WINDOW_WIDTH = 1600;
constexpr static int WINDOW_HEIGHT = 1200;

static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (glfwInit() != GL_TRUE)
	{
		cout << "Failed to init GLFW" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	try
	{
		GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Airplane control symulator", nullptr, nullptr);
		if (window == nullptr)
		{
			throw runtime_error("Failed to create window");
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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
			view = glm::rotate(view, glm::radians((GLfloat)glfwGetTime() * 1120), glm::vec3(1.0f, 0.0f, 0.0f));

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (GLfloat)(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.0f);

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

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}