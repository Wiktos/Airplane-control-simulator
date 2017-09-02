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

static void processInput(GLFWwindow* window);

Camera camera(glm::vec3(0.0f, -0.5f, 7.0f));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

unique_ptr<Airplane> boeing727;

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, SOIL_LOAD_RGB);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			SOIL_free_image_data(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

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

		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};
		// skybox VAO
		unsigned int skyboxVAO, skyboxVBO;
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		vector<string> faces
		{
				"./Resource Files/textures/cubemap/miramar_rt.tga",
				"./Resource Files/textures/cubemap/miramar_lf.tga",
				"./Resource Files/textures/cubemap/miramar_ft.tga",
				"./Resource Files/textures/cubemap/miramar_dn.tga",
				"./Resource Files/textures/cubemap/miramar_bk.tga",
				"./Resource Files/textures/cubemap/miramar_ft.tga"
		};
		GLuint cubemapTexture = loadCubemap(faces);


		while (!glfwWindowShouldClose(window))
		{
			GLfloat currFrame = glfwGetTime();
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

			// draw skybox as last
			glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
			skyboxShader.use();
			view = glm::mat4(glm::mat3(camera.viewMatrix())) * (float)glfwGetTime(); // remove translation from the view matrix
			skyboxShader.setMat4("view", view);
			skyboxShader.setMat4("projection", projection);
			// skybox cube
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS); // set depth function back to default
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
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		boeing727->leanBackward(deltaTime);
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