#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>
#include "Transform.h"

struct KeyInput
{
	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;
	bool moveForward = false;
	bool moveBackward = false;
	bool P1Left = false;
	bool P1Right = false;
	bool P1Up = false;
	bool P1Down = false;
	bool P1Forward = false;
	bool P1Backward = false;
	bool P1Shoot = false;
	bool P2Shoot = false;
};

struct MeshFaceFloat;

class Mesh : public Transform
{
public:
	Mesh();
	~Mesh();

	//load funtion for mesh and send to openGL
	bool loadFromFile(const std::string &file);
	bool loadFiles(const std::vector<std::string> &files);
	// release data from openGL (VRAM)
	void unload();
	//void Unload();

	void draw() const;
	void Mesh::bind() const;
	static void Mesh::unbind();

	unsigned int getNumFaces() const;
	unsigned int getNumVerts() const;

	void checkCollisionZ(KeyInput in);
	void checkCollisionX(KeyInput in);

	//openGL buffers amd objects
	GLuint VBO_verts = 0;
	GLuint VBO_uvs = 0;
	GLuint VBO_norms = 0;
	GLuint VAO = 0;	
	
private:
	unsigned int _numFaces = 0;
	unsigned int _numVerts = 0;

	unsigned int _NumFrames = 0;
	GLuint *VBO_Vert = 0;
	GLuint *VBO_UVs = 0;
	GLuint *VBO_Normals = 0;
	std::string _File;

	unsigned int _NumFaces = 0;
	unsigned int _NumVert = 0;

	bool loaded = false;
};