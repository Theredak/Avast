#pragma once
#include "GL/glew.h"
#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string &file);
	~Texture();

	// Loads the texture
	bool load(const std::string &file);
	// Unloads the texture
	bool unload();

	// Binds the texture to the last active slot
	void bind() const;
	// Binds the texture to a specific slot
	void bind(int textureSlot) const;
	// Unbinds the last active texture slot
	void unbind() const;
	// Unbinds a specific texture slot
	void unbind(int textureSlot) const;

private:
	GLuint _TexHandle;

	std::string filename;

	GLenum _InternalFormat;
	GLenum _Target;
	GLenum _FilterMag = GL_NEAREST; // Filter parameter
	GLenum _FilterMin = GL_NEAREST;
	GLenum _WrapU = GL_MIRRORED_REPEAT; //GL_CLAMP Per axis Wrap parameter
	GLenum _WrapV = GL_MIRRORED_REPEAT;//GL_MIRRORED_REPEAT;

	int sizeX;
	int sizeY;
	int channels;
};