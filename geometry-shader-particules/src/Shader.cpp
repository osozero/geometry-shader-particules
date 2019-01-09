#include "Shader.h"
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include "glm/gtc/type_ptr.hpp"


shader::shader(const char* vertexSourcePath, const char* fragmentSourcePath, const char* geometrySourcePath)
{
	auto strVertex = readFile(vertexSourcePath);
	auto strFragment = readFile(fragmentSourcePath);
	std::string strGeometry;

	auto vertexSource = strVertex.c_str();
	auto fragmentSource = strFragment.c_str();
	const char* geometrySource = nullptr;


	if(geometrySourcePath!=nullptr)
	{
		strGeometry = readFile(geometrySourcePath);
		geometrySource = strGeometry.c_str();
	}

	unsigned int vId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vId, 1, &vertexSource, nullptr);
	glCompileShader(vId);
	
	int success;
	char infoLog[512];

	glGetShaderiv(vId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fId, 1, &fragmentSource, nullptr);
	glCompileShader(fId);

	glGetShaderiv(fId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int gId = 0;

	if(geometrySourcePath!=nullptr)
	{
		gId = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gId, 1, &geometrySource, nullptr);
		glCompileShader(gId);

		glGetShaderiv(gId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(gId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	}

	id = glCreateProgram();

	glAttachShader(id, vId);
	
	if(geometrySourcePath!=nullptr)
	{
		glAttachShader(id,gId);
	}

	glAttachShader(id, fId);

	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vId);
	glDeleteShader(fId);

	if(geometrySourcePath!=nullptr)
	{
		glDeleteShader(gId);
	}
}

void shader::use()
{
	glUseProgram(id);
}

void shader::setUniform4m(const char* name, glm::mat4 m)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(m));
}

void shader::setUniform3v(const char* name, glm::vec3 v)
{
	glUniform3f(glGetUniformLocation(id, name), v.x, v.y, v.z);
}

std::string shader::readFile(const char* filePath)
{

	std::ifstream in;

	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		in.open(filePath);

		std::stringstream ss;

		ss << in.rdbuf();
		in.close();
		return ss.str();
	}catch (std::ifstream::failure &e)
	{
		in.close();
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		return nullptr;
	}	
}
