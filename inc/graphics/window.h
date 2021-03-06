#ifndef WINDOW_H
#define WINDOW_H

#define GLEW_STATIC
#include <Python.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "util/exception.h"
#include <iostream>
#include <thread>
#include "util/settings.h"
#include <vector>
#include <functional>
#include <cmath>
#include <map>
#include <tuple>
#include "graphics/tile.h"
#include "graphics/vbo.h"
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

class Window;

class Scene
{
public:
	virtual void action(std::string s, Window& w) {};
	virtual void mouseMoveAction(Window& w, float x, float y) {};
	virtual void render() {};
	virtual void update(float fTimeDif) {};
};//scene

/**
 * @brief A window.
 * @details
 * This class is responsible for opening and closing a window.
 */
class Window
{
public:
	class EventBuffer
	{
	public:
		const static unsigned int uiSize = 100;
		SDL_Event aEvents[uiSize];
		bool aEventEmpty[uiSize];
		unsigned int uiNextPut = 0;
		unsigned int uiNextPull = 0;

		EventBuffer()
		{
			for(unsigned int x=0; x < uiSize; x++)
				aEventEmpty[x] = true;
		}//constructor

		void put()
		{
			while( aEventEmpty[uiNextPut] && SDL_PollEvent( &aEvents[uiNextPut] ) )
			{
				aEventEmpty[uiNextPut] = false;
				uiNextPut = (uiNextPut + 1) % uiSize;
			}//while
		}//function

		void pull(std::function<void(SDL_Event&)> fDo)
		{
			while(!aEventEmpty[uiNextPull])
			{
				fDo(aEvents[uiNextPull]);
				aEventEmpty[uiNextPull] = true;
				uiNextPull = (uiNextPull + 1) % uiSize;
			}//while
		}//function
	};//class

	EventBuffer xEvents;
    Sint32 iLastMouseX = 0;
    Sint32 iLastMouseY = 0;
	std::shared_ptr<Settings> pSettings;
	std::shared_ptr<Scene> pScene = nullptr;
	bool bKeepLooping = true;
	///@brief The window we'll be rendering to
	SDL_Window* window = nullptr;
	///@brief The surface contained by the window
	SDL_GLContext context;

	Window(std::shared_ptr<Settings> pSettings);
	~Window();
	void processEvents();
	void render();
	void renderLoop();
	void loop();
	void action(std::string s);

};//class


class World: public Scene
{
public:
	std::shared_ptr<Settings> pSettings;
    std::shared_ptr<Textures> pTextures;
	unsigned int uiChunkSize;
	float fCameraX = 0, fCameraY = 0, fCameraZ, fCamSpeed, fCamZSpeed;
	std::shared_ptr<Tiles> pTiles;
	std::map<std::tuple<int, int>, std::shared_ptr<Chunk>> xChunks;
	std::vector<std::tuple<std::string, std::function<void(World&, Window&)>>> vActions;
	unsigned int uiChunkLayers;
	volatile bool bRenderLines = false;
	bool bCamUp = false, bCamLeft = false, bCamRight = false, bCamDown = false, bCamIn = false, bCamOut = false, bMoveWithMouse = false;


	std::shared_ptr<Chunk> get(int x, int y);
	std::shared_ptr<Tile> get(int x, int y, unsigned int l);
	void set(int x, int y, unsigned int l, std::shared_ptr<Tile> pTile);

	World(std::shared_ptr<Settings> pSettings);
	void render();
	void action(std::string s, Window& w);
	virtual void mouseMoveAction(Window& w, float x, float y) 
    {
        if(bMoveWithMouse)
        {
            fCameraX += x*fCameraZ/uiChunkSize;
            fCameraY += y*fCameraZ/uiChunkSize;
        }// if
    }//function
	virtual void update(float fTimeDif);
};//scene

#endif