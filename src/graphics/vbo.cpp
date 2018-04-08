#include "graphics/vbo.h"

VBO::VBO(unsigned int iNum, void* pVertices, size_t uiSize, bool bStatic)
		:
	iNum(iNum),
    uiSize(uiSize),
    bStatic(bStatic)
{
	glGenBuffers(1, &vbo); // Generate buffer
	activate();
	glBufferData(GL_ARRAY_BUFFER, uiSize*iNum, pVertices,
		bStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}//constructor

/// When no data is give we always want to create a dynamic buffer
VBO::VBO(unsigned int iNum, size_t uiSize)
		:
	iNum(iNum),
    uiSize(uiSize),
    bStatic(false)
{
	glGenBuffers(1, &vbo); // Generate buffer
	activate();
	glBufferData(GL_ARRAY_BUFFER, uiSize*iNum, NULL, GL_DYNAMIC_DRAW);
}//constructor

void VBO::set(void* pVertices)
{
    assert(!bStatic);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uiSize*iNum, pVertices);
}

void VBO::activate()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}//function

void VBO::deactivate()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}//function

void VAO::draw(float x, float y, float s)
{
	activate();
	std::vector<float> f = {
		s, 0, 0,
		0, s, 0,
		x, y, 1
	};
	pShader->setMat("modelView", f);
	glDrawArrays(GL_TRIANGLES, 0, iNumElements);
}//function

VBO::~VBO()
{
	glDeleteBuffers(1, &vbo);
}//deconstructor

void VBO::link(
		std::shared_ptr<Shader> pShader,
		std::string sName,
		unsigned int uiSizeOne,
		GLenum eType,
		bool bNormalized,
		unsigned int uiStride
	)
{
	GLint iIn = glGetAttribLocation(pShader->iShaderProgram, sName.c_str());
	if(iIn == -1)
		throw Exception(std::string("Attribute not found in shader: ").append(sName));
	glVertexAttribPointer(
			iIn, 
			uiSizeOne, 
			eType, 
			bNormalized ? GL_TRUE : GL_FALSE, 
			uiStride, 
			0
		);
	glEnableVertexAttribArray(iIn);
}//function