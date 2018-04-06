#ifndef CHUNK_H
#define CHUNK_H

#include "graphics/vbo.h"


class Tile
{
public:
	std::string sName;
	std::shared_ptr<TileVAO> pVao;

	Tile(std::string sName, std::shared_ptr<Settings> pSettings);

	void draw(float x, float y, float s);

};//class

class Tiles
{
public:
	std::vector<std::shared_ptr<Tile>> vTiles;

	Tiles(std::shared_ptr<Settings> pSettings)
	{
		for(std::string sTexName : pSettings->get_vec("graphics.texturelist"))
		{
			vTiles.push_back(std::shared_ptr<Tile>(new Tile(sTexName, pSettings)));
		}//for
	}//constructor

	std::shared_ptr<Tile> get(std::string sName)
	{
		for(auto pTile : vTiles)
			if(pTile->sName == sName)
				return pTile;
		throw Exception("unknown tilename");
	}//function

	std::shared_ptr<Tile> getDefault()
	{
		return get("default");
	}//function
};//class

class Chunk
{
public:
	const unsigned int iSize;
	const unsigned int iLayers;
	std::vector<std::shared_ptr<Tile>> vTiles;

	unsigned int index(unsigned int x, unsigned int y, unsigned int l);
	std::shared_ptr<Tile> get(unsigned int x, unsigned int y, unsigned int l);
	void set(unsigned int x, unsigned int y, unsigned int l, std::shared_ptr<Tile> pTile);
	void draw(float x, float y, float s);
	
	Chunk(std::shared_ptr<Tiles> pTiles, std::shared_ptr<Settings> pSettings);
};//class


#endif