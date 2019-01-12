#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Libs/Matrix2D.h"

extern int const SCREEN_WIDTH;
extern int const SCREEN_HEIGHT;

class Shader
{
public:
	unsigned int ID;

	Shader() : ID(-1) {

	}

	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str(); // converting from c++ string to c string
		const char* fShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		unsigned int vertex, fragment;
		/*int success;
		char infoLog[512];*/
		
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glValidateProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setTransform(const std::string &name, Matrix2D value)
	{
		float proj4[4][4];
		float model[16] = { 0.0f };
		
		glModel(value, model);

		// to preserve the aspect ratio of objects drawn in orthographic projection
		glOrtho(-SCREEN_HEIGHT/2.0f, SCREEN_HEIGHT/2.0f, -SCREEN_WIDTH/2.0f, SCREEN_WIDTH/2.0f, -1.0f, 1.0f, proj4);

		// kinda slow, reduce to 1 call if needed
		//printf("%i : %i\n", ID, glGetUniformLocation(ID, name.c_str()));
		glUniformMatrix4fv(2, 1, GL_FALSE, model);
		glUniformMatrix4fv(3, 1, GL_FALSE, &proj4[0][0]);
	}

private:

	void glModel(Matrix2D m, float mat4[16]) {
		mat4[10] = 1.0f;
		mat4[15] = 1.0f;
		mat4[12] = m.m[0][2];
		mat4[13] = m.m[1][2];

		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				mat4[4 * j + i] = m.m[i][j];
			}
		}
	}

	void glOrtho(float b, float t, float l, float r, float n, float f, float M[4][4])
	{
		// set OpenGL perspective projection matrix
		M[0][0] = 2 / (r - l);
		M[0][1] = 0;
		M[0][2] = 0;
		M[0][3] = 0;

		M[1][0] = 0;
		M[1][1] = 2 / (t - b);
		M[1][2] = 0;
		M[1][3] = 0;

		M[2][0] = 0;
		M[2][1] = 0;
		M[2][2] = -2 / (f - n);
		M[2][3] = 0;

		M[3][0] = -(r + l) / (r - l);
		M[3][1] = -(t + b) / (t - b);
		M[3][2] = -(f + n) / (f - n);
		M[3][3] = 1;
	}

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif