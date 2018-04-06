#include "graphics/tile.h"

Tile::Tile(std::string sName, std::shared_ptr<Settings> pSettings)
		:
	sName(sName),
	pVao(std::shared_ptr<TileVAO>(new TileVAO(pSettings, sName)))
{
}//constructor

void Tile::draw(float x, float y, float s)
{
	pVao->draw(x,y,s);
}///function

Chunk::Chunk(std::shared_ptr<Tiles> pTiles, std::shared_ptr<Settings> pSettings)
		:
	iSize(pSettings->get<unsigned int>("chunks.size")),
	iLayers(pSettings->get<unsigned int>("chunks.layers")),
	vTiles(iSize*iSize*iLayers)
{
	for(unsigned int i=0; i < iLayers; i++)
		for(unsigned int x=0; x < iSize; x++)
			for(unsigned int y=0; y < iSize; y++)
				set(x, y, i, pTiles->getDefault());
}//constructor

unsigned int Chunk::index(unsigned int x, unsigned int y, unsigned int l)
{
	return x*iSize*iLayers + y*iLayers + l;
}//function

void Chunk::set(unsigned int x, unsigned int y, unsigned int l, std::shared_ptr<Tile> pTile)
{
	if(vTiles.size() <= index(x, y, l))
		throw Exception("Chunk: index out of bounds");
	vTiles[index(x, y, l)] = pTile;
}//function

std::shared_ptr<Tile> Chunk::get(unsigned int x, unsigned int y, unsigned int l)
{
	if(vTiles.size() <= index(x, y, l))
		throw Exception("Chunk: index out of bounds");
	return vTiles[index(x, y, l)];
}//function

void Chunk::draw(float fx, float fy, float s)
{
	for(unsigned int i=0; i < iLayers; i++)
		for(unsigned int x=0; x < iSize; x++)
			for(unsigned int y=0; y < iSize; y++)
				get(x,y,i)->draw(fx+x*s/((float)iSize), fy+y*s/((float)iSize), s/iSize);
}///function
