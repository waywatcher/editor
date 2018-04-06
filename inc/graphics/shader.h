#ifndef SHADER_H
#define SHADER_H

#include "util/settings.h"
#define GLEW_STATIC
#include "GL/glew.h"
#include <fstream>
#include <streambuf>

class Shader
{
public:
	const std::string shaderName;
	const GLuint iVertex, iGeometry, iFragment, iShaderProgram;

	Shader(std::shared_ptr<Settings> pSettings, std::string shaderName);
	~Shader();

	void activate();
	void deactivate();
	void set(std::string sAtribName, float f)
	{
		GLint iPos = glGetUniformLocation(iShaderProgram, sAtribName.c_str());
		glUniform1f(iPos, f);
	}//funciton
	void setVec(std::string sAtribName, float f, float f2)
	{
		GLint iPos = glGetUniformLocation(iShaderProgram, sAtribName.c_str());
		glUniform2f(iPos, f, f2);
	}//funciton
	void setMat(std::string sAtribName, std::vector<float> f)
	{
		if(f.size() != 9)
			throw Exception(std::string("cant set a matrix3f with ").append(std::to_string(f.size())).append(" elements"));
		GLint iPos = glGetUniformLocation(iShaderProgram, sAtribName.c_str());
		glUniformMatrix3fv(iPos, 1, GL_FALSE, f.data());
	}//funciton

	static std::string loadFile(std::string sFileName);
	static GLuint compileShader(std::string sSource, GLenum eShaderType);
};//class

#endif