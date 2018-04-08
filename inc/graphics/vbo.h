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
    const unsigned int iNum;
    const size_t uiSize;
    const bool bStatic;

    VBO(unsigned int iNum, void* pVertices, size_t uiSize, bool bStatic = true);
    VBO(unsigned int iNum, size_t uiSize);
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
    void set(void* pVertices);

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

	std::shared_ptr<VBO> add(
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
        return pVBO;
	}//function

	template<typename T>
	std::shared_ptr<VBO> add(
			T* pVertices,
			std::string sName,
			unsigned int uiSizeOne,
			GLenum eType,
			bool bStatic = true,
			bool bNormalized = false,
			unsigned int uiStride = 0
		)
	{
		return add(
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

	template<typename T>
	std::shared_ptr<VBO> add(
			std::string sName,
			unsigned int uiSizeOne,
			GLenum eType,
			bool bNormalized = false,
			unsigned int uiStride = 0
		)
	{
		return add(
				std::shared_ptr<VBO>(new VBO(
						iNumElements * uiSizeOne, 
						sizeof(T)
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
    std::shared_ptr<VBO> pTexOffsetBuffer;
    std::vector<float> vTexOffsets;

	std::vector<float> getVertexPosGrid()
	{
		std::vector<float> vRet(uiChunkSize * uiChunkSize * uiChunkLayers * 3 * 2 * 2);

		const float one = 1.f / uiChunkSize;

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
	std::vector<float> getTexPosGrid(unsigned int uiNumTextures)
	{
		std::vector<float> vRet(uiChunkSize * uiChunkSize * uiChunkLayers * 3 * 2 * 2);

        // this ensures that we do not accidentally draw one pixel further than the image
        // floating point arithmetic is not perfectly accurate...
        const float zero = 0.000001f;
        const float one =  0.999999f;

		unsigned int coord = 0;
		for(unsigned int z = 0; z < uiChunkLayers; z++)
			for(unsigned int y = 0; y < uiChunkSize; y++)
				for(unsigned int x = 0; x < uiChunkSize; x++)
				{
					vRet[ coord++ ] = zero;
					vRet[ coord++ ] = zero;

					vRet[ coord++ ] = zero;
					vRet[ coord++ ] = one/uiNumTextures;

					vRet[ coord++ ] = one;
					vRet[ coord++ ] = zero;


					vRet[ coord++ ] = one;
					vRet[ coord++ ] = one/uiNumTextures;

					vRet[ coord++ ] = one;
					vRet[ coord++ ] = zero;

					vRet[ coord++ ] = zero;
					vRet[ coord++ ] = one/uiNumTextures;
				}//for
		return vRet;
	}//fuction

    inline void setTex(unsigned int uiX, unsigned int uiY, unsigned int uiZ, const float uiF)
    {
        for(unsigned int uiI = 0; uiI < 6; uiI++)
            vTexOffsets[ ((uiX + uiY * uiChunkSize) * uiChunkLayers + uiZ) * 6 + uiI] = uiF;
    }//function

    inline void setTex(unsigned int uiX, unsigned int uiY, unsigned int uiZ, std::shared_ptr<Texture> pTex)
    {
        setTex(uiX, uiY, uiZ, pTex->uiTexOffset);
    }//function

    inline void updateTex()
    {
        pTexOffsetBuffer->set(&vTexOffsets[0]);
    }//function

	ChunkVAO(std::shared_ptr<Settings> pSettings, unsigned int uiNumTextures)
			:
		VAO(
				std::shared_ptr<Shader>(new Shader(pSettings, "textured")),
				pSettings->get<unsigned int>("chunks.size") *
				pSettings->get<unsigned int>("chunks.size") *
				pSettings->get<unsigned int>("chunks.layers") *
				3 * 2
			),
		uiChunkSize(pSettings->get<unsigned int>("chunks.size")),
		uiChunkLayers(pSettings->get<unsigned int>("chunks.layers")),
        vTexOffsets(uiChunkSize * uiChunkSize * uiChunkLayers * 6)
	{
		//generate pos and tex
		add<float>(getVertexPosGrid().data(), "pos", 2, GL_FLOAT);
		add<float>(getTexPosGrid(uiNumTextures).data(), "tex", 2, GL_FLOAT);
		pTexOffsetBuffer = add<float>("texOffset", 1, GL_FLOAT);

        //text data
        for(unsigned int ui = 0; ui < uiChunkSize * uiChunkSize * uiChunkLayers * 6; ui++)
            vTexOffsets[ui] = 0.5;
        setTex(std::rand()%uiChunkSize, std::rand()%uiChunkSize, 0, 0);
        setTex(std::rand()%uiChunkSize, std::rand()%uiChunkSize, 0, 0);
        setTex(std::rand()%uiChunkSize, std::rand()%uiChunkSize, 0, 0);
        updateTex();
	}//constructor

};//class

#endif