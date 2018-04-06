#include "graphics/shader.h"



Shader::Shader(std::shared_ptr<Settings> pSettings, std::string shaderName)
		:
	shaderName(shaderName),
	iVertex(
		compileShader(
			loadFile(pSettings->get<std::string>(
				std::string("graphics.shaders.")
				.append(shaderName)
				.append(".vertex_shader"))
			),
			GL_VERTEX_SHADER
		)
	),
	iGeometry(
		compileShader(
			loadFile(pSettings->get<std::string>(
				std::string("graphics.shaders.")
				.append(shaderName)
				.append(".geometry_shader"))
			),
			GL_GEOMETRY_SHADER
		)
	),
	iFragment(
		compileShader(
			loadFile(pSettings->get<std::string>(
				std::string("graphics.shaders.")
				.append(shaderName)
				.append(".fragment_shader"))
			),
			GL_FRAGMENT_SHADER
		)
	),
	iShaderProgram(glCreateProgram())
{
	if(iVertex != 0)
		glAttachShader(iShaderProgram, iVertex);
	if(iGeometry != 0)
		glAttachShader(iShaderProgram, iGeometry);
	if(iFragment != 0)
		glAttachShader(iShaderProgram, iFragment);

	glBindFragDataLocation(iShaderProgram, 0, "outColor");

	glLinkProgram(iShaderProgram);

	GLint iStatus;
	glGetProgramiv(iShaderProgram, GL_LINK_STATUS, &iStatus);

	if(iStatus != GL_TRUE)
	{
		char aBuffer[512];
		glGetProgramInfoLog(iShaderProgram, 512, NULL, aBuffer);
		std::cerr << "programm info log: " << std::endl;
		std::cerr << aBuffer << std::endl;
		throw Exception("could not link programm");
	}//if
}//constructor

std::string Shader::loadFile(std::string sFileName)
{
	if(sFileName == "null")
		return "null";
	std::ifstream xIfs ( sFileName, std::ifstream::in);
	if(!xIfs.good())
		throw Exception(std::string("cannot open shader source: ").append(sFileName));
	std::string sRet((std::istreambuf_iterator<char>(xIfs)),
					std::istreambuf_iterator<char>());
	return sRet;
}//function

GLuint Shader::compileShader(std::string sSource, GLenum eShaderType)
{
	if(sSource == "null")
		return 0;
	GLuint iShader = glCreateShader(eShaderType);
	const GLchar* pSource = (const GLchar*)sSource.c_str();
	glShaderSource(iShader, 1, &pSource, NULL);
	glCompileShader(iShader);
	GLint iStatus;
	glGetShaderiv(iShader, GL_COMPILE_STATUS, &iStatus);
	if(iStatus == GL_TRUE)
		return iShader;
	char aBuffer[512];
	glGetShaderInfoLog(iShader, 512, NULL, aBuffer);
	std::cerr << "shader info log: " << std::endl;
	std::cerr << aBuffer << std::endl;
	std::cerr << "shader source: " << std::endl;
	std::cerr << sSource << std::endl;
	throw Exception("could not compile shader");
}//function

Shader::~Shader()
{
	glDeleteShader(iVertex);
	glDeleteShader(iFragment);
	glDeleteShader(iGeometry);
	glDeleteProgram(iShaderProgram);
}//deconstructor

void Shader::activate()
{
	glUseProgram(iShaderProgram);
}//function

void Shader::deactivate()
{
	glUseProgram(0);
}//function