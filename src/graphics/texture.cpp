#include "graphics/texture.h"

Texture::Texture(std::string sFileName)
{
	glGenTextures(1, &iTex);
	glBindTexture(GL_TEXTURE_2D, iTex);
	//set the texture to repeat in both directions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set the scale in a way that the pics stay pixelated
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	cimg_library::CImg<unsigned int> xTex(sFileName.c_str());

	float data[xTex.width()][xTex.height()][xTex.spectrum()];

	for(int x = 0; x < xTex.width(); x++)
		for(int y = 0; y < xTex.height(); y++)
			for(int z = 0; z < xTex.spectrum(); z++)
				data[x][y][z] = (float)xTex.atXYZC(x,y,0,z) / 255.0f;

	auto format = GL_RGB;

	if(xTex.spectrum() == 4)
		format = GL_RGBA;

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			format,
			xTex.width(),
			xTex.height(),
			0,
			format,
			GL_FLOAT,
			data
		);

	glActiveTexture(GL_TEXTURE0);

}//constructor

Textures::Textures(std::shared_ptr<Settings> pSettings)
		:
	pSettings(pSettings)
{
	load("default");
}//constructor

void Textures::load(std::string sName)
{
	std::string sFileName = pSettings->get<std::string>(
		std::string("graphics.textures.").append(sName));
	vTextures.push_back(std::make_tuple(
		sName, std::shared_ptr<Texture>(new Texture(sFileName))));
}//function

std::shared_ptr<Texture> Textures::get(std::string sName)
{
	//check if texture exists already
	for(auto tuple : vTextures)
		if(std::get<0>(tuple) == sName)
			return std::get<1>(tuple);
	//load it otherwise
	load(sName);
	return std::get<1>(vTextures.back());
}//function

void Texture::link(
		std::shared_ptr<Shader> pShader,
		std::string sName,
		unsigned int uiSizeOne,
		GLenum eType,
		bool bNormalized,
		unsigned int uiStride
	)
{
}//function