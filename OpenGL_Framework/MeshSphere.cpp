#include "MeshSphere.h"
#include <vector>
#include "Math.h"

void MeshSphere::initMesh(const unsigned int xSlices, const unsigned int ySlices)
{
	std::vector<vec4> vertexData;
	std::vector<vec4> textureData;
	std::vector<vec4> normalData;

	
	float x = (float)xSlices;
	float y = (float)ySlices;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			float s0 = sin(((j + 0) / y)* PI);
			float s1 = sin(((j + 1) / y)* PI);
		
			float c0 = cos((2.0f * (j + 0) / y - 1.0f)* PI * 0.5f + PI * 0.5f);
			float c1 = cos((2.0f * (j + 1) / y - 1.0f)* PI * 0.5f + PI * 0.5f);

			//s0 = 1.0f;
			//s1 = 1.0f;
			vertexData.push_back(vec4(s0 * sin((i + 0) * PI*2.0f / x), c0, s0 * cos((i + 0) * PI*2.0f / x), 0.0f));
			vertexData.push_back(vec4(s1 * sin((i + 0) * PI*2.0f / x), c1, s1 * cos((i + 0) * PI*2.0f / x), 0.0f));
			vertexData.push_back(vec4(s0 * sin((i + 1) * PI*2.0f / x), c0, s0 * cos((i + 1) * PI*2.0f / x), 0.0f));
			vertexData.push_back(vec4(s1 * sin((i + 0) * PI*2.0f / x), c1, s1 * cos((i + 0) * PI*2.0f / x), 0.0f));
			vertexData.push_back(vec4(s1 * sin((i + 1) * PI*2.0f / x), c1, s1 * cos((i + 1) * PI*2.0f / x), 0.0f));
			vertexData.push_back(vec4(s0 * sin((i + 1) * PI*2.0f / x), c0, s0 * cos((i + 1) * PI*2.0f / x), 0.0f));

			textureData.push_back(vec4());
			textureData.push_back(vec4());
			textureData.push_back(vec4());
			textureData.push_back(vec4());
			textureData.push_back(vec4());
			textureData.push_back(vec4());
			
			normalData.push_back(vertexData[vertexData.size() - 6].GetNormalized());
			normalData.push_back(vertexData[vertexData.size() - 5].GetNormalized());
			normalData.push_back(vertexData[vertexData.size() - 4].GetNormalized());
			normalData.push_back(vertexData[vertexData.size() - 3].GetNormalized());
			normalData.push_back(vertexData[vertexData.size() - 2].GetNormalized());
			normalData.push_back(vertexData[vertexData.size() - 1].GetNormalized());
		}
	}

	_NumVert = vertexData.size();
	_NumFaces = _NumVert / 3;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Vert);
	glGenBuffers(1, &VBO_UV);
	glGenBuffers(1, &VBO_Normal);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4),  reinterpret_cast<void*>(0));

	//Uvs
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * textureData.size(), &textureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), reinterpret_cast<void*>(0));

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * normalData.size(), &normalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), reinterpret_cast<void*>(0));
}

void MeshSphere::bind() const
{
	glBindVertexArray(VAO);
}

void MeshSphere::unbind()
{
	glBindVertexArray(GL_NONE);
}

void MeshSphere::draw() const
{
	glDrawArrays(GL_TRIANGLES, 0, _NumVert);
}