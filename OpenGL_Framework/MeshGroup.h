#pragma once
#include <string>
#include <vector>
#include <memory>
#include "GL/glew.h"
#include "Math.h"
#include "Transform.h"

struct MeshFaceFloat;

class MeshGroup : public Transform
{
public:
	MeshGroup();
	~MeshGroup();

	bool loadFiles(const std::vector<std::string> &files);
	void unload();

	unsigned int getNumFaces() const;
	unsigned int getNumVertices() const;

	void bind() const;
	void draw() const;
	static void unbind();
private:
	GLuint VAO = 0;

	unsigned int _NumFrames = 0;
	GLuint *VBO_Vert = 0;
	GLuint *VBO_UVs = 0;
	GLuint *VBO_Normals = 0;
	std::string _File;

	unsigned int _NumFaces = 0;
	unsigned int _NumVert = 0;

	bool loaded = false;
};
