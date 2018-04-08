#include "graphics/texture.h"


Textures::Textures(std::shared_ptr<Settings> pSettings)
		:
	pSettings(pSettings),
    vTextures(),
    vPixels()
{
    // get the list of textures to load
    auto vTexList = pSettings->get_vec("graphics.textures.list");
    unsigned int uiNumTex = vTexList.size();
    // get the texture dimensions
    unsigned int uiPixelX = pSettings->get<unsigned int>("graphics.textures.size.x");
    unsigned int uiPixelY = pSettings->get<unsigned int>("graphics.textures.size.x");
    unsigned int uiChanels = pSettings->get<unsigned int>("graphics.textures.size.z");

    // allocate the space required for all textures
    vPixels.reserve(uiPixelX * uiPixelY * uiChanels * uiNumTex);
    vTextures.reserve(uiNumTex);

    // load all textures (@todo not happy with the cImg library...)
    unsigned int uiPicOffset = 0;
    for(std::string sTexName : vTexList)
    {
        std::string sFileName = pSettings->get<std::string>("graphics.textures." + sTexName);
        cimg_library::CImg<unsigned int> xTex(sFileName.c_str());
        //this could be done much faster for sure
        for(int y = (int)uiPixelY-1; y >= 0; y--)
            for(unsigned int x = 0; x < uiPixelX; x++)
                for(unsigned int z = 0; z < uiChanels; z++)
                    vPixels.push_back( (float)xTex.atXYZC(x,y,0,z) / 255.0f );
        vTextures.push_back(std::make_pair(
            sTexName, std::shared_ptr<Texture>(new Texture(uiPicOffset * uiPixelY))));
        uiPicOffset += 1;
    }//for

    //setup the texture
    glGenTextures(1, &iTex);
    glBindTexture(GL_TEXTURE_2D, iTex);
    //set the texture to repeat in both directions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set the scale in a way that the pics stay pixelated
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            uiPixelX,
            uiPixelY * uiNumTex,
            0,
            GL_RGBA,
            GL_FLOAT,
            &vPixels[0]
        );

    glActiveTexture(GL_TEXTURE0);
}//constructor


std::shared_ptr<Texture> Textures::get(std::string sName)
{
	//check if we loaded that texture
	for(auto xPair : vTextures)
		if(xPair.first == sName)
			return xPair.second;
	//return the default one otherwise
	return vTextures.front().second;
}//function