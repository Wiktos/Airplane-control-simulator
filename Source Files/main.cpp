#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <irrKlang\irrKlang.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <windows.h>
#include <memory>

using namespace std;

#include "airplane.h"
#include "camera.h"
#include "skybox.h"

static void processInput(GLFWwindow* window);
int sgn(float val) 
{
	return (float(0) < val) - (val < float(0));
}

Camera camera(glm::vec3(0.0f, -0.5f, 7.0f));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

unique_ptr<Airplane> boeing727;
glm::mat4 airplaneControlModelMatrix;
glm::mat4 areaControlModelMatrix;

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

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			cout << "Failed to init GLEW" << endl;
			return -1;
		}

		ShaderProgram airplaneShader("./Resource Files/shaders/airplaneVertexShader.vert",
			"./Resource Files/shaders/airplaneFragmentShader.frag");
		ShaderProgram skyboxShader("./Resource Files/shaders/skyboxVertexShader.vert",
			"./Resource Files/shaders/skyboxFragmentShader.frag");

		boeing727 = unique_ptr<Airplane>(new Airplane("./Resource Files/models/boeing727/Boeing727.obj", 10.0f));

		irrklang::ISoundEngine* symulatorSoundEnginee = irrklang::createIrrKlangDevice();
		symulatorSoundEnginee->play2D("./Resource Files/sounds/avion_sound.mp3", GL_TRUE);
		Sleep(100);
		symulatorSoundEnginee->play2D("./Resource Files/sounds/avion_sound.mp3", GL_TRUE);

		Skybox area;
		glm::mat4 areaModel;

		while (!glfwWindowShouldClose(window))
		{
 			GLfloat currFrame = (GLfloat)glfwGetTime();
			deltaTime = currFrame - lastFrame;
			lastFrame = currFrame;

			glfwSwapBuffers(window);
			glfwPollEvents();
			processInput(window);

			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 airplaneModel;
			airplaneModel = boeing727->modelMatrix();

			glm::mat4 view;
			view = camera.viewMatrix();

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (GLfloat)(mode->width / mode->height), 0.1f, 100.0f);

			airplaneShader.use();
			airplaneShader.setMat4("model", airplaneModel);
			airplaneShader.setMat4("view", view); 
			airplaneShader.setMat4("projection", projection); 

			boeing727->draw(airplaneShader);

			glDepthFunc(GL_LEQUAL);  
			skyboxShader.use();
			
			areaModel = glm::mat4();
			areaModel = glm::translate(areaModel, glm::vec3(0.0f, 0.0f, glfwGetTime() / 75.0f));
			float rollAngle = boeing727->getRollAngle();
			if (rollAngle != 0.0f)
			{
				areaControlModelMatrix = glm::rotate(areaControlModelMatrix, -sin(glm::radians(rollAngle)) * deltaTime / 10.0f,
					glm::vec3(0.0f, 1.0f, 0.0f));
			}
			areaModel *= areaControlModelMatrix;
			
			view = glm::mat4(glm::mat3(camera.viewMatrix())); // remove translation from the view matrix
			
			skyboxShader.setMat4("model", areaModel);
			skyboxShader.setMat4("view", view);
			skyboxShader.setMat4("projection", projection);
			
			area.draw();
			glDepthFunc(GL_LESS);
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
		boeing727->leanForward(deltaTime);
		if (boeing727->isPitchingPossible())
		{
			areaControlModelMatrix = glm::rotate(areaControlModelMatrix, deltaTime / 7.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		boeing727->leanBackward(deltaTime);
		if (boeing727->isPitchingPossible())
		{
			areaControlModelMatrix = glm::rotate(areaControlModelMatrix, deltaTime / 7.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		boeing727->rollRight(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		boeing727->rollLeft(deltaTime);
	}
}