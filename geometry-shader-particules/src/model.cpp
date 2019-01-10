#include "model.h"

model::model(std::string const& path, bool gamma): gammaCorrection(gamma)
{
	loadModel(path);
}

void model::draw(shader shdr)
{
	for(unsigned int i=0;i<meshes.size();i++)
	{
		meshes[i].draw(shdr);
	}
}

void model::loadModel(std::string const& path)
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if(!scene||scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void model::processNode(aiNode* node, const aiScene* scene)
{
	for(unsigned int i=0;i<node->mNumMeshes;i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for(unsigned int i=0;i<node->mNumChildren;i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

mesh model::processMesh(aiMesh* aimesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for(unsigned int i=0;i< aimesh->mNumVertices;i++)
	{
		Vertex vertex;

		glm::vec3 vector;

		vector.x = aimesh->mVertices[i].x;
		vector.y = aimesh->mVertices[i].y;
		vector.z = aimesh->mVertices[i].z;

		vertex.Position = vector;

		//normals;
		vector.x = aimesh->mNormals[i].x;
		vector.y = aimesh->mNormals[i].y;
		vector.z = aimesh->mNormals[i].z;

		vertex.Normal = vector;

		//texture coords

		if (aimesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			vec.x = aimesh->mTextureCoords[0][i].x;
			vec.y = aimesh->mTextureCoords[0][i].y;

			vertex.TexCoords = vec;
		}else
		{
			vertex.TexCoords = glm::vec2(0.0, 0.0);
		}

		//tangent
		vector.x = aimesh->mTangents[i].x;
		vector.y = aimesh->mTangents[i].y;
		vector.z = aimesh->mTangents[i].z;

		vertex.Tangent = vector;

		//bitangent

		vector.x = aimesh->mBitangents[i].x;
		vector.y = aimesh->mBitangents[i].y;
		vector.z = aimesh->mBitangents[i].z;

		vertex.Bitangent = vector;

		vertices.push_back(vertex);
	}

	for(unsigned int i=0;i< aimesh->mNumFaces;i++)
	{
		aiFace face = aimesh->mFaces[i];

		for(unsigned int j=0;j<face.mNumIndices;j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return mesh(vertices, indices, textures);
}

std::vector<Texture> model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for(unsigned int i=0;i<mat->GetTextureCount(type);i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;

		for(unsigned int j=0;j<texturesLoaded.size();j++)
		{
			if(std::strcmp(texturesLoaded[j].path.data(),str.C_Str())==0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}

		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = textureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	return textures;
}

model::~model()
{
}
