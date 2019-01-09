#define GLEW_STATIC


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ostream>
#include <iostream>
#include "Shader.h"

const unsigned int WIDTH = 1800;
const unsigned int HEIGHT = 1200;


glm::vec3 camera(0.0, 1.0, -0.3);
glm::vec3 cameraFront(0, 0, -1);
glm::vec3 up(0, 1.0, 0.0);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, 1200, "GeometryShader", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glewInit();

	shader appShader("shader/particules.vs", "shader/particules.fs");
	shader normalDisplayerShader("shader/particules.vs", "shader/particules.fs", "shader/particules.gs");

	float vertices[] = {
		-0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, // top-left
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f // bottom-left
	};

	glm::mat4 view(1);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	glm::vec3 normal(glm::cross(glm::vec3(0.0, 0.5, 0.0) - glm::vec3(-0.5f, -0.5f, 0.0f),
	                            glm::vec3(0.5f, -0.5f, 0.0f) - glm::vec3(-0.5f, -0.5f, 0.0f)));


	normal = glm::normalize(normal);

	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(0, 0, -10));
	unsigned int vbo, vao;

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);


	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	appShader.use();


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		view = glm::lookAt(camera, camera+cameraFront, up);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLineWidth(1.0f);

		appShader.use();
		appShader.setUniform4m("projection", projection);
		appShader.setUniform4m("model", model);
		appShader.setUniform4m("view", view);
		appShader.setUniform3v("normal", normal);

		glBindVertexArray(vao);


		glDrawArrays(GL_TRIANGLES, 0, 3);


		normalDisplayerShader.use();
		normalDisplayerShader.setUniform4m("projection", projection);
		normalDisplayerShader.setUniform4m("model", model);
		normalDisplayerShader.setUniform4m("view", view);
		normalDisplayerShader.setUniform3v("normal", normal);
		
		
		glLineWidth(25.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();

	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
	float cameraSpeed = 0.005f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera += cameraSpeed * cameraFront;
	}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera -= cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}