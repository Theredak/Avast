#pragma once
#include <GL/glew.h>

#include <MiniMath/Core.h>
#include <string>

class ShaderProgram
{
public :
	ShaderProgram();
	ShaderProgram(const std::string &vertFile, const std::string &fragFile);
	~ShaderProgram();

	// loads vertx and fragment shader and places them in a program
	bool load(const std::string &vertFile, const std::string &fragFile);
	bool reload();
	bool isLoaded() const;
	//clear data from openGL
	void unLoad();
	bool linkProgram();
	// use/detach the shader from use
	void bind() const;
	void unBind();

	//requires a re-link before openGL will register change
	void addAttribute(unsigned int index, const std::string &attribName);

	// returns -1 if attribute doesn't exist
	int getAttribLocation(const std::string &attribName);
	// returns -1 if uniform does not exist
	int getUniformLocation(const std::string &uniformName);

	// send data to shaders
	void sendUniform(const std::string &name, int integer);
	void sendUniform(const std::string &name, unsigned int unsigned_integer);
	void sendUniform(const std::string &name, float scaler);
	void sendUniform(const std::string &name, const vec2 &vector);
	void sendUniform(const std::string &name, const vec3 &vector);
	void sendUniform(const std::string &name, const vec4 &vector);
	void sendUniformMat3(const std::string &name, float *matrix, bool transpose);
	void sendUniformMat4(const std::string &name, float *matrix, bool transpose);

private:
	bool _isInit = false;
	GLuint _vertexShader = 0;
	GLuint _fragShader = 0;
	GLuint _program = 0;

	std::string vertexFilename;
	std::string fragFilename;

	std::string ReadFile(const std::string &fileName) const;
	bool compileShader(GLuint shader) const;
	void outputShaderLog(GLuint shader) const;
	void outputProgramLog() const;

};