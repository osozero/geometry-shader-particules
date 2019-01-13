#pragma once
#include <sstream>
#include <glm/matrix.hpp>

class shader
{
public:
	shader(const char* vertexSourcePath,const char* fragmentSourcePath, const char* geometrySourcePath=nullptr);
	void use();
	void setUniform4m(const char* name, glm::mat4 m);
	void setUniform3v(const char* name, glm::vec3 v);
	void setFloat(const char* name, float f);
	unsigned int getId() const
	{
		return this->id;
	}
	~shader(){};
private:
	unsigned int id;

	std::string readFile(const char *filePath);
};

