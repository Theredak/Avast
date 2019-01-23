#include "ShaderProgram.h"

#include <vector>
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram()
{

}
ShaderProgram::ShaderProgram(const std::string &vertFile, const std::string &fragFile)
{
	load(vertFile, fragFile);
}
ShaderProgram::~ShaderProgram()
{
	if (_isInit)
	{
		unLoad();
	}
}


bool ShaderProgram::load(const std::string &vertFile, const std::string &fragFile)
{
	vertexFilename = vertFile;
	fragFilename = fragFile;

	// create shader and program objects
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	_program = glCreateProgram();

	// load source code
	std::string source = ReadFile(vertFile);
	const GLchar *temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_vertexShader, 1, &temp, NULL);

	source = ReadFile(fragFile);
	temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_fragShader, 1, &temp, NULL);

	//compile code
	if (!compileShader(_vertexShader))
	{
		std::cout << "vertex shader failed to compile.\n";
		outputShaderLog(_vertexShader);
		unLoad();

		return false;
	}
	if (!compileShader(_fragShader))
	{
		std::cout << "fragment shader failed to compile.\n";
		outputShaderLog(_fragShader);
		unLoad();

		return false;
	}

	// set up program object
	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragShader);

	if (!linkProgram())
	{
		std::cout << "shader program failed to link. \n";

		outputProgramLog();
		unLoad();

		return false;
	}

	_isInit = true;
	return true;
}
bool ShaderProgram::reload()
{
	return load(vertexFilename, fragFilename);
}
bool ShaderProgram::isLoaded() const
{
	return _isInit;
}

void ShaderProgram::unLoad()
{
	if (_vertexShader != 0)
	{
		glDetachShader(_program, _vertexShader);
		glDeleteShader(_vertexShader);
		_vertexShader = 0;
	}
	if (_fragShader != 0)
	{
		glDetachShader(_program, _fragShader);
		glDeleteShader(_fragShader);
		_fragShader = 0;
	}
	if (_program != 0)
	{		
		glDeleteProgram(_program);
		_program = 0;
	}

	_isInit = false;
}
bool ShaderProgram::linkProgram()
{	
	glLinkProgram(_program);

	GLint success;
	glGetProgramiv(_program, GL_LINK_STATUS, &success);

	return success == GL_TRUE;
}

void ShaderProgram::bind() const
{
	glUseProgram(_program);
}
void ShaderProgram::unBind()
{
	glUseProgram(0);
}


void ShaderProgram::addAttribute(unsigned int index, const std::string &attribName)
{
	glBindAttribLocation(_program, index, attribName.c_str());
}

int ShaderProgram::getAttribLocation(const std::string &attribName)
{
	return glGetAttribLocation(_program, attribName.c_str());
}
int ShaderProgram::getUniformLocation(const std::string &uniformName) 
{
	return glGetUniformLocation(_program, uniformName.c_str());
}


void ShaderProgram::sendUniform(const std::string &name, int integer)
{
	GLint location = getUniformLocation(name);
	glUniform1i(location, integer);
}
void ShaderProgram::sendUniform(const std::string &name, unsigned int unsigned_integer)
{
	GLint location = getUniformLocation(name);
	glUniformui64NV(location, unsigned_integer);
}
void ShaderProgram::sendUniform(const std::string &name, float scaler)
{
	GLint location = getUniformLocation(name);
	glUniform1f(location, scaler);
}
void ShaderProgram::sendUniform(const std::string &name, const vec2 &vector)
{
	GLint location = getUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
}
void ShaderProgram::sendUniform(const std::string &name, const vec3 &vector)
{
	GLint location = getUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
}
void ShaderProgram::sendUniform(const std::string &name, const vec4 &vector)
{
	GLint location = getUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}
void ShaderProgram::sendUniformMat3(const std::string &name, float *matrix, bool transpose)
{
	GLint location = getUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}
void ShaderProgram::sendUniformMat4(const std::string &name, float *matrix, bool transpose)
{
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
}

std::string ShaderProgram::ReadFile(const std::string &fileName) const
{
	std::ifstream inStream(fileName);

	if (!inStream.good())
	{
		std::cout << "shader file not found.\n";
		return std::string();
	}
	std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));
	return data;
}
bool ShaderProgram::compileShader(GLuint shader) const
{
	glCompileShader(shader);
	GLint success;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	return success == GL_TRUE;
}
void ShaderProgram::outputShaderLog(GLuint shader) const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	glGetShaderInfoLog(shader, sizeof(char) * 512, &infoLen, &infoLog[0]);

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
}
void ShaderProgram::outputProgramLog() const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLen);

	glGetProgramInfoLog(_program, sizeof(char) * 512, &infoLen, &infoLog[0]);

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
}