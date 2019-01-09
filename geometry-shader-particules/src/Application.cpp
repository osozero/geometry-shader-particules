#define GLEW_STATIC


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <ostream>
#include <iostream>

const unsigned int WIDTH = 1800;
const unsigned int HEIGHT = 1200;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 col;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y,aPos.z, 1.0);\n"
"   color=col;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 color;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(color, 1.0f);\n"
"}\n\0";


const char *geometryShaderSource = "#version 330 core\n"
"layout (triangles) in;\n"
"layout (line_strip,max_vertices =5) out;\n"
"uniform vec3 normal;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"   gl_Position= gl_in[0].gl_Position + vec4(0.1,0,0,0);\n"
"   EmitVertex();\n"
"   gl_Position= gl_in[0].gl_Position + vec4(0.0,-0.4,0,0);\n"
"   EmitVertex();\n"

"   color=vec3(1,0,0);\n"

"   EndPrimitive();\n"
"}\n\0";


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

	glewInit();


	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
	glCompileShader(geometryShader);

	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}


	int normalDisplayerShaderProgram = glCreateProgram();
	glAttachShader(normalDisplayerShaderProgram, vertexShader);
	glAttachShader(normalDisplayerShaderProgram, geometryShader);
	glAttachShader(normalDisplayerShaderProgram, fragmentShader);

	glLinkProgram(normalDisplayerShaderProgram);

	glGetProgramiv(normalDisplayerShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(normalDisplayerShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 1.0f,0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f,0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,0.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 0.0f, 1.0f,0.0f  // bottom-left
	};


	glm::vec3 normal(glm::cross(glm::vec3(0.0, 0.5, 0.0) - glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f) - glm::vec3(-0.5f, -0.5f, 0.0f)));

	normal = glm::normalize(normal);

	unsigned int vbo, vao;

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);


	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	while(!glfwWindowShouldClose(window))
	{
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glLineWidth(1.0f);
			glUseProgram(shaderProgram);
			glBindVertexArray(vao);

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glUseProgram(normalDisplayerShaderProgram);

			glUniform3f(glGetUniformLocation(normalDisplayerShaderProgram, "normal"), normal.x, normal.y, normal.z);

			glLineWidth(5.0f);
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
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}