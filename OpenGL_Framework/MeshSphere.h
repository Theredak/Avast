#pragma once
#include "GL/glew.h"
#include "Transform.h"

class MeshSphere : public Transform
{
public:
	void initMesh(const unsigned int x, const unsigned int y);

	void draw() const;
	void MeshSphere::bind() const;
	static void MeshSphere::unbind();
private:
	GLuint VAO = 0;
	
	GLuint VBO_Vert   = 0;
	GLuint VBO_UV     = 0;
	GLuint VBO_Normal = 0;

	unsigned int _NumFaces = 0;
	unsigned int _NumVert = 0;
};