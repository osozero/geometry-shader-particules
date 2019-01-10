#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>

class shader;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO;


	mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void draw(shader shader);
	~mesh();

private:
	unsigned int VBO, EBO;

	void setupMesh();
};

