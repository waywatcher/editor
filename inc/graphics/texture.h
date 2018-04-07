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
	GLuint iTex;
	Texture(std::string sFileName);
	static void link(
			std::shared_ptr<Shader> pShader,
			std::string sName,
			unsigned int uiSizeOne,
			GLenum eType,
			bool bNormalized,
			unsigned int uiStride
		);
};//class

class Textures
{
public:
	std::shared_ptr<Settings> pSettings;
	std::vector<std::tuple<std::string, std::shared_ptr<Texture>>> vTextures;

	Textures(std::shared_ptr<Settings> pSettings);

	void load(std::string sName);
	std::shared_ptr<Texture> get(std::string sName);
};//class

#endif