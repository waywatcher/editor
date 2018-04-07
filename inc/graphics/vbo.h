#ifndef VBO_H
#define VBO_H

#define GLEW_STATIC
#include "GL/glew.h"
#include "graphics/texture.h"
#include "graphics/shader.h"

class VBO
{
public:
	GLuint vbo;
	unsigned int iNum;

	VBO(unsigned int iNum, void* pVertices, size_t uiSize, bool bStatic = true);
	~VBO();
	void activate();
	void deactivate();
	static void link(
			std::shared_ptr<Shader> pShader,
			std::string sName,
			unsigned int uiSizeOne,
			GLenum eType,
			bool bNormalized,
			unsigned int uiStride
		);

};//class

class VAO
{
public:
	GLuint vao;
	std::shared_ptr<Shader> pShader;
	std::vector<std::shared_ptr<VBO>> vVBO;
	unsigned int iNumElements;

	void activate() {glBindVertexArray(vao);}//function
	void deactivate() {glBindVertexArray(0);}//function
	void draw(float x, float y, float s);

	VAO(std::shared_ptr<Shader> pShader, unsigned int iNumElements)
			:
		pShader(pShader),
		iNumElements(iNumElements)
	{
		glGenVertexArrays(1, &vao);
		activate();

		pShader->activate();
	}//constructor

	void add(
			std::shared_ptr<VBO> pVBO,
			std::string sName,
			unsigned int uiSizeOne,
			GLenum eType,
			bool bNormalized = false,
			unsigned int uiStride = 0
		)
	{
		try
		{
			vVBO.push_back(pVBO);
			pVBO->link(pShader, sName, uiSizeOne, eType, bNormalized, uiStride);
		}
		catch(Exception e)
		{
			std::cerr << "Exception: " << e.what() << std::endl;
		}
	}//function

	template<typename T>
	void add(
			T* pVertices,
			std::string sName,
			unsigned int uiSizeOne,
			GLenum eType,
			bool bStatic = true,
			bool bNormalized = false,
			unsigned int uiStride = 0
		)
	{
		add(
				std::shared_ptr<VBO>(new VBO(
						iNumElements * uiSizeOne, 
						pVertices, 
						sizeof(T),
						bStatic
					)),
				sName,
				uiSizeOne,
				eType,
				bNormalized,
				uiStride
			);
	}//function

};//class

/**
 * creates a grid
 * @note this is extremely inefficient at the moment but whatefs...
 */
class ChunkVAO: public VAO
{
public:
	unsigned int uiChunkSize;
	unsigned int uiChunkLayers;

	std::vector<float> getVertexPosGrid()
	{
		std::vector<float> vRet(uiChunkSize * uiChunkSize * uiChunkLayers * 3 * 2 * 2);

		float one = 1.f / uiChunkSize;

		unsigned int coord = 0;
		for(unsigned int z = 0; z < uiChunkLayers; z++)
			for(unsigned int y = 0; y < uiChunkSize; y++)
				for(unsigned int x = 0; x < uiChunkSize; x++)
				{
					vRet[ coord++ ] = (float)x * one;
					vRet[ coord++ ] = (float)y * one;

					vRet[ coord++ ] = (float)x * one;
					vRet[ coord++ ] = (float)(y+1) * one;

					vRet[ coord++ ] = (float)(x+1) * one;
					vRet[ coord++ ] = (float)y * one;


					vRet[ coord++ ] = (float)(x+1) * one;
					vRet[ coord++ ] = (float)(y+1) * one;

					vRet[ coord++ ] = (float)(x+1) * one;
					vRet[ coord++ ] = (float)y * one;

					vRet[ coord++ ] = (float)x * one;
					vRet[ coord++ ] = (float)(y+1) * one;
				}//for
		return vRet;
	}//fuction
	std::vector<float> getTexPosGrid()
	{
		std::vector<float> vRet(uiChunkSize * uiChunkSize * uiChunkLayers * 3 * 2 * 2);

		unsigned int coord = 0;
		for(unsigned int z = 0; z < uiChunkLayers; z++)
			for(unsigned int y = 0; y < uiChunkSize; y++)
				for(unsigned int x = 0; x < uiChunkSize; x++)
				{
					vRet[ coord++ ] = 0;
					vRet[ coord++ ] = 0;

					vRet[ coord++ ] = 0;
					vRet[ coord++ ] = 1;

					vRet[ coord++ ] = 1;
					vRet[ coord++ ] = 0;


					vRet[ coord++ ] = 1;
					vRet[ coord++ ] = 1;

					vRet[ coord++ ] = 1;
					vRet[ coord++ ] = 0;

					vRet[ coord++ ] = 0;
					vRet[ coord++ ] = 1;
				}//for
		return vRet;
	}//fuction

	ChunkVAO(std::shared_ptr<Settings> pSettings)
			:
		VAO(
				std::shared_ptr<Shader>(new Shader(pSettings, "textured")),
				pSettings->get<unsigned int>("chunks.size") *
				pSettings->get<unsigned int>("chunks.size") *
				pSettings->get<unsigned int>("chunks.layers") *
				3 * 2
			),
		uiChunkSize(pSettings->get<unsigned int>("chunks.size")),
		uiChunkLayers(pSettings->get<unsigned int>("chunks.layers"))
	{
		//setup chunk 
		Texture t("assets/default.bmp");
		//generate pos and tex
		add<float>(getVertexPosGrid().data(), "pos", 2, GL_FLOAT);
		add<float>(getTexPosGrid().data(), "tex", 2, GL_FLOAT);
	}
};//class
/**
 * creates a grid
 * @note this is extremely inefficient at the moment but whatefs...
 */
class TileVAO: public VAO
{
public:

	TileVAO(std::shared_ptr<Settings> pSettings, std::string sTextureName)
			:
		VAO(
				std::shared_ptr<Shader>(new Shader(pSettings, "textured")),
				4
			)
	{
		//setup chunk 
		Texture(pSettings->get<std::string>(std::string("graphics.textures.").append(sTextureName)));
		float vPos[] = {
			0.0, 0.0,
			1.0, 0.0,
			0.0, 1.0,
			1.0, 1.0
		};
		//generate pos and tex
		add<float>(vPos, "pos", 2, GL_FLOAT);
		add<float>(vPos, "tex", 2, GL_FLOAT);
	}
};//class

#endif