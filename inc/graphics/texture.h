#ifndef TEXTURE_H
#define TEXTURE_H

#include <Python.h>
#include "util/settings.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include "graphics/shader.h"
#include "util/CImg.h"

class Texture
{
public:
    const float uiTexOffset;
	Texture(float uiTexOffset)
            :
        uiTexOffset(uiTexOffset)
    {}//constructor
};//class

class Textures
{
public:
	GLuint iTex;
	std::shared_ptr<Settings> pSettings;
	std::vector<std::pair<std::string, std::shared_ptr<Texture>>> vTextures;
    std::vector<float> vPixels;

	Textures(std::shared_ptr<Settings> pSettings);

	std::shared_ptr<Texture> get(std::string sName);
};//class

#endif