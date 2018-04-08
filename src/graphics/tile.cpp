#include "graphics/tile.h"

Tile::Tile(std::string sName, std::shared_ptr<Settings> pSettings, std::shared_ptr<Textures> pTextures)
		:
	sName(sName),
    pTex(pTextures->get(pSettings->get<std::string>("gameplay.tiles." + sName + ".texture")))
{

}//constructor

Chunk::Chunk(std::shared_ptr<Tiles> pTiles, std::shared_ptr<Settings> pSettings)
		:
	iSize(pSettings->get<unsigned int>("chunks.size")),
	iLayers(pSettings->get<unsigned int>("chunks.layers")),
	vTiles(iSize*iSize*iLayers),
    pVAO(new ChunkVAO(pSettings, pSettings->get_vec("graphics.textures.list").size()))
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
    pVAO->draw(fx, fy, s);
}///function
