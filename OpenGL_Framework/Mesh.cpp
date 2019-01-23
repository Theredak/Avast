#define _CRT_SECURE_NO_WARNINGS

#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <MiniMath\Core.h>

#define CHAR_BUFFER_SIZE 128 //Size of the character buffer.
#define BUFFER_OFFSET(i) reinterpret_cast<void*>(i)

struct meshFace
{
	meshFace()
	{
		verts[0] = 0;
		verts[1] = 0;
		verts[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}
	meshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned t1, unsigned t2, unsigned t3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		verts[0] = v1;
		verts[1] = v2;
		verts[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned verts[3];
	unsigned textureUVs[3];
	unsigned normals[3];
};
struct MeshFaceFloat
{
	MeshFaceFloat()
	{
		verts[0] = 0;
		verts[1] = 0;
		verts[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;

	}
	MeshFaceFloat(float v1, float v2, float v3, float v4,
		float t1, float t2, float t3, float t4,
		float n1, float n2, float n3, float n4)
	{
		verts[0] = v1;
		verts[1] = v2;
		verts[2] = v3;
		verts[3] = v4;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;
		textureUVs[3] = t4;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
		normals[3] = n4;
	}
	float verts[4];
	float textureUVs[4];
	float normals[4];
	float face[16];
};
union meshTranslation
{
public:
	float m_array[16];
	char m_charArray[sizeof(float) * 16];
};

Mesh::Mesh()
{

}
Mesh::~Mesh()
{

}

//load funtion for mesh and send to openGL
bool Mesh::loadFromFile(const std::string &file)
{
	std::fstream input;
	input.open(file);

	if (!input)
	{
		std::cout << "Error loading file" << std::endl;
		return false;
	}

	char inputString[CHAR_BUFFER_SIZE];

	// unique data
	std::vector<vec3> vertexData;
	std::vector<vec2> textureData;
	std::vector<vec3> normalData;
	//index face data
	std::vector<meshFace> faceData;
	//openGL ready data
	std::vector<float> unpackedVertData;
	std::vector<float> unpackedTextureData;
	std::vector<float> unpackedNormalData;

	while (!input.eof())
	{
		input.getline(inputString, CHAR_BUFFER_SIZE);

		if (std::strstr(inputString, "#") != nullptr)
		{
			// this line is a comment
			continue;
		}
		else if (std::strstr(inputString, "vn") != nullptr)
		{
			//this line has normal data
			vec3 temp;

			std::sscanf(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
			normalData.push_back(temp);
		}
		else if (std::strstr(inputString, "vt") != nullptr)
		{
			//this line has UV data
			vec2 temp;

			std::sscanf(inputString, "vt %f %f", &temp.x, &temp.y);
			textureData.push_back(temp);
		}
		else if (std::strstr(inputString, "v") != nullptr)
		{
			//this line has vertex data
			vec3 temp;

			std::sscanf(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
			vertexData.push_back(temp);
		}
		else if (std::strstr(inputString, "f") != nullptr)
		{
			//this line contains face data
			meshFace temp;

			std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&temp.verts[0], &temp.textureUVs[0], &temp.normals[0],
				&temp.verts[1], &temp.textureUVs[1], &temp.normals[1],
				&temp.verts[2], &temp.textureUVs[2], &temp.normals[2]);

			faceData.push_back(temp);
		}
	}
	input.close();
	//unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			unpackedVertData.push_back(vertexData[faceData[i].verts[j] - 1].x);
			unpackedVertData.push_back(vertexData[faceData[i].verts[j] - 1].y);
			unpackedVertData.push_back(vertexData[faceData[i].verts[j] - 1].z);

			unpackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
			unpackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);

			unpackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
			unpackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
			unpackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);
		}
	}
	_numFaces = faceData.size();
	_numVerts = _numFaces * 3;

	//send data to openGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_verts);
	glGenBuffers(1, &VBO_uvs);
	glGenBuffers(1, &VBO_norms);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); //vertex
	glEnableVertexAttribArray(1); // UVs
	glEnableVertexAttribArray(2); // normals

	glBindBuffer(GL_ARRAY_BUFFER, VBO_verts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedVertData.size(), &unpackedVertData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, sizeof(vec3), reinterpret_cast<void*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedTextureData.size(), &unpackedTextureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, sizeof(vec2), reinterpret_cast<void*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_norms);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedNormalData.size(), &unpackedNormalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 4, GL_FLOAT, GL_FALSE, sizeof(vec3), reinterpret_cast<void*>(0));

	//clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertexData.clear();
	textureData.clear();
	normalData.clear();
	faceData.clear();
	unpackedVertData.clear();
	unpackedTextureData.clear();
	unpackedNormalData.clear();

	return true;
}
bool Mesh::loadFiles(const std::vector<std::string>& files)
{
	_NumFrames = files.size();

	VBO_Vert = new GLuint[_NumFrames];
	VBO_UVs = new GLuint[_NumFrames];
	VBO_Normals = new GLuint[_NumFrames];
	glGenVertexArrays(1, &VAO);


	glBindVertexArray(VAO);

	for (unsigned int c = 0; c < _NumFrames; ++c)
	{
		glGenBuffers(1, &VBO_Vert[c]);
		glGenBuffers(1, &VBO_UVs[c]);
		glGenBuffers(1, &VBO_Normals[c]);

		std::ifstream input;
		input.open(files[c]);

		if (!input)
		{
			std::cout << "Error: Could not open file \"" << files[c] << "\"!\n";
			loaded = false;
			return false;

		}
		char line[CHAR_BUFFER_SIZE];

		vec3 topCorn; //Bounding box corner
		vec3 btmCorn; //Bounding box corner

		//Unique data
		std::vector<vec3> vertexData;
		std::vector<vec2> textureData;
		std::vector<vec3> normalData;
		//index/face data
		std::vector<meshFace> faceData;

		float x, y, z;
		meshFace faceTemp;

		//Use the face data to create an array of verts, uvs, and normals for faster use in openGL
		std::vector<float> unPackedVertexData;
		std::vector<float> unPackedTextureData;
		std::vector<float> unPackedNormalData;

		std::vector<MeshFaceFloat> unPackedFaceData;

		while (!input.eof()) //While the file has not ended, continue pulling in data.
		{
			input.getline(line, CHAR_BUFFER_SIZE);

			switch (line[0])
			{
			case 'v':
				switch (line[1])
				{
				case ' ':
					std::sscanf(line, "v %f %f %f", &x, &y, &z);
					vertexData.push_back(vec3(x, y, z));
					break;
				case 't':
					std::sscanf(line, "vt %f %f", &x, &y);
					textureData.push_back(vec2(x, y));
					break;
				case 'n':
					std::sscanf(line, "vn %f %f %f", &x, &y, &z);
					normalData.push_back(vec3(x, y, z));
					break;
				}
				break;
			case 'f':
			{
				int numSuccess = std::sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&faceTemp.verts[0], &faceTemp.textureUVs[0], &faceTemp.normals[0],
					&faceTemp.verts[1], &faceTemp.textureUVs[1], &faceTemp.normals[1],
					&faceTemp.verts[2], &faceTemp.textureUVs[2], &faceTemp.normals[2]);
				if (numSuccess < 9)
				{
					numSuccess = std::sscanf(line, "f %u//%u %u//%u %u//%u",
						&faceTemp.verts[0], &faceTemp.normals[0],
						&faceTemp.verts[1], &faceTemp.normals[1],
						&faceTemp.verts[2], &faceTemp.normals[2]);
					faceTemp.textureUVs[0] = 1;
					faceTemp.textureUVs[1] = 1;
					faceTemp.textureUVs[2] = 1;

					faceData.push_back(faceTemp);
					if (numSuccess < 6)
					{
						std::cout << "OH SHIT";
					}
				}
				else
				{
					faceData.push_back(faceTemp);
				}
			}
			break;
			case 'o':
			case '#':
			case 'm':
			default:
				break;
			}
		}
		input.close();

		if (textureData.size() < 2)
		{
			textureData.push_back(vec2());
		}

		//Unpack the data
		for (unsigned i = 0; i < faceData.size(); i++)
		{
			for (unsigned j = 0; j < 3; j++)
			{
				unPackedVertexData.push_back(vertexData[faceData[i].verts[j] - 1].x); //We use -1 because OBJ specs start at 1 instead of 0.
				unPackedVertexData.push_back(vertexData[faceData[i].verts[j] - 1].y);
				unPackedVertexData.push_back(vertexData[faceData[i].verts[j] - 1].z);
				unPackedVertexData.push_back(1.0f);

				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);
				unPackedTextureData.push_back(0.0f);
				unPackedTextureData.push_back(1.0f);

				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);
				unPackedNormalData.push_back(1.0f);
			}
		}

		unPackedFaceData.resize(faceData.size());

		_NumFaces = faceData.size();
		_NumVert = _NumFaces * 3;

		unsigned int indexOffset = c * 2; // We have 3 VBOs
		unsigned int indexOffset2 = c;

		glEnableVertexAttribArray(0 + indexOffset);
		glEnableVertexAttribArray(1 + indexOffset2);
		glEnableVertexAttribArray(2 + indexOffset);

		//Vertex
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Vert[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedVertexData.size(), &unPackedVertexData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(vec3), BUFFER_OFFSET(0));

		//UVs
		glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1 + indexOffset2, 4, GL_FLOAT, GL_FALSE, sizeof(vec2), BUFFER_OFFSET(0));

		//Normals
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)2 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(vec3), BUFFER_OFFSET(0));


		vertexData.clear(); //Clear the vectors from RAM now that everything's in the GPU.
		textureData.clear();
		normalData.clear();
		faceData.clear();
		unPackedVertexData.clear();
		unPackedTextureData.clear();
		unPackedNormalData.clear();
		unPackedFaceData.clear();
	}
	//Begin cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	loaded = true;
	return true;
}
// release data from openGL (VRAM)
void Mesh::unload()
{
	glDeleteBuffers(1, &VBO_norms);
	glDeleteBuffers(1, &VBO_uvs);
	glDeleteBuffers(1, &VBO_verts);
	glDeleteVertexArrays(1, &VAO);

	VBO_norms = 0;
	VBO_uvs = 0;
	VBO_verts = 0;
	VAO = 0;

	_numFaces = 0;
	_numVerts = 0;
}

//void Mesh::Unload()
//{
//	for (int c = 0; c < _NumFrames; ++c)
//	{
//		glDeleteBuffers(1, &VBO_Normals[c]);
//		glDeleteBuffers(1, &VBO_UVs[c]);
//		glDeleteBuffers(1, &VBO_Vert[c]);
//	}
//	glDeleteVertexArrays(1, &VAO);
//	VBO_Normals = nullptr;
//	VBO_UVs = nullptr;
//	VBO_Vert = nullptr;
//	VAO = 0;
//
//	_NumFaces = 0;
//	_NumVert = 0;
//	_NumFrames = 0;
//
//	loaded = 0;
//}

void Mesh::bind() const
{
	glBindVertexArray(VAO);
}

void Mesh::unbind()
{
	glBindVertexArray(GL_NONE);
}
void Mesh::draw() const
{
	glDrawArrays(GL_TRIANGLES, 0, _numVerts);
}
unsigned int Mesh::getNumFaces() const
{
	return _numFaces;
}
unsigned int Mesh::getNumVerts() const
{
	return _numVerts;
}

void Mesh::checkCollisionZ(KeyInput in)
{
	if (this->m_pLocalPosition.z < -14)
	{
		in.P1Forward = false;
	}
	if (this->m_pLocalPosition.z > -6)
	{
		in.P1Backward = false;
	}
}

void Mesh::checkCollisionX(KeyInput in)
{
	if (this->m_pLocalPosition.x < -14)
	{
		in.moveForward = false;
	}
	if (this->m_pLocalPosition.x > -6)
	{
		in.moveBackward = false;
	}
}
