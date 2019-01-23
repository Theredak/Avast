#define _CRT_SECURE_NO_WARNINGS

#include "MeshGroup.h"
#include <fstream>
#include <iostream>
#include "MiniMath/Core.h"


#define CHAR_BUFFER_SIZE 128 //Size of the character buffer.
#define BUFFER_OFFSET(i) reinterpret_cast<void*>(i)

struct MeshFace
{
	MeshFace()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned u1, unsigned u2, unsigned u3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = u1;
		textureUVs[1] = u2;
		textureUVs[2] = u3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned vertices[4];
	unsigned textureUVs[4];
	unsigned normals[4];
};

struct MeshFaceFloat
{
	MeshFaceFloat()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

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
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
		vertices[3] = v4;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;
		textureUVs[3] = t4;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
		normals[3] = n4;
	}
	union
	{
		struct
		{
			float vertices[4];
			float textureUVs[4];
			float normals[4];
		};

		float face[16];
	};

};


union meshTranslation
{
public:
	float m_array[16];
	char m_charArray[sizeof(float) * 16];
};

MeshGroup::MeshGroup()
{

}

MeshGroup::~MeshGroup()
{

}

bool MeshGroup::loadFiles(const std::vector<std::string> &files)
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
		std::vector<MeshFace> faceData;

		float x, y, z;
		MeshFace faceTemp;

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
					&faceTemp.vertices[0], &faceTemp.textureUVs[0], &faceTemp.normals[0],
					&faceTemp.vertices[1], &faceTemp.textureUVs[1], &faceTemp.normals[1],
					&faceTemp.vertices[2], &faceTemp.textureUVs[2], &faceTemp.normals[2]);
				if (numSuccess < 9)
				{
					numSuccess = std::sscanf(line, "f %u//%u %u//%u %u//%u",
						&faceTemp.vertices[0], &faceTemp.normals[0],
						&faceTemp.vertices[1], &faceTemp.normals[1],
						&faceTemp.vertices[2], &faceTemp.normals[2]);
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
				unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].x); //We use -1 because OBJ specs start at 1 instead of 0.
				unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].y);
				unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].z);
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

		unsigned int indexOffset = c * 3; // We have 3 VBOs

		glEnableVertexAttribArray(0 + indexOffset);
		glEnableVertexAttribArray(1 + indexOffset);
		glEnableVertexAttribArray(2 + indexOffset);

		//Vertex
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Vert[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedVertexData.size(), &unPackedVertexData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));

		//UVs
		glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));

		//Normals
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)2 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));


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

void MeshGroup::unload()
{
	for (int c = 0; c < _NumFrames; ++c)
	{
		glDeleteBuffers(1, &VBO_Normals[c]);
		glDeleteBuffers(1, &VBO_UVs[c]);
		glDeleteBuffers(1, &VBO_Vert[c]);
	}
	glDeleteVertexArrays(1, &VAO);
	VBO_Normals = nullptr;
	VBO_UVs = nullptr;
	VBO_Vert = nullptr;
	VAO = 0;

	_NumFaces = 0;
	_NumVert = 0;
	_NumFrames = 0;

	loaded = 0;
}

unsigned int MeshGroup::getNumFaces() const
{
	return _NumFaces;
}

unsigned int MeshGroup::getNumVertices() const
{
	return _NumVert;
}

void MeshGroup::bind() const
{
	glBindVertexArray(VAO);
}

void MeshGroup::unbind()
{
	glBindVertexArray(0);
}

void MeshGroup::draw() const
{
	glDrawArrays(GL_TRIANGLES, 0, _NumVert);
}
