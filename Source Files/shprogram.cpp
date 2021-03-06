#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#include "shprogram.h"

ShaderProgram::ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// prepare vertex and fragment shaders
	string vertex_code = read_shader_code(vertexPath);
	GLuint vertex_id = compile_shader(vertex_code.c_str(), GL_VERTEX_SHADER);

	string fragment_code = read_shader_code(fragmentPath);
	GLuint fragment_id = compile_shader(fragment_code.c_str(), GL_FRAGMENT_SHADER);

	// link shader program
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	glLinkProgram(program_id);

	validShaderProgramCompilation();

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

string ShaderProgram::read_shader_code(const GLchar* shaderPath)
{
	ifstream shader_file;
	shader_file.exceptions(ifstream::badbit);

	shader_file.open(shaderPath);
	stringstream shader_stream;
	shader_stream << shader_file.rdbuf();
	shader_file.close();
	return shader_stream.str();
}

GLuint ShaderProgram::compile_shader(const GLchar* shaderCode, GLenum shaderType)
{
	GLuint shader_id = glCreateShader(shaderType);
	glShaderSource(shader_id, 1, &shaderCode, NULL);
	glCompileShader(shader_id);
	validShaderCompilation(shader_id);
	return shader_id;
}

void ShaderProgram::validShaderCompilation(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		string msg = string("Shader compilation: ") + infoLog;
		throw runtime_error("Failed to compile shader : " + msg);
	}
}

void ShaderProgram::validShaderProgramCompilation()
{
	GLint success;
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program_id, sizeof(infoLog), NULL, infoLog);
		string msg = string("Shader program linking: ") + infoLog;
		throw runtime_error("Failed to link shader program : " + msg);
	}
}