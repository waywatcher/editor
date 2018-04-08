#include "graphics/window.h"

Window::Window(std::shared_ptr<Settings> pSettings)
		:
	xEvents(),
	pSettings(pSettings)
{

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		throw Exception(std::string("SDL could not initialize: ").append(SDL_GetError()));
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		//Create window
		window = SDL_CreateWindow(
				"Test",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				pSettings->get<int>("graphics.resolution.x"),
				pSettings->get<int>("graphics.resolution.y"), 
				SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
			);
		if( window == NULL )
			throw Exception(std::string("Window could not be created: ").append(SDL_GetError()));
		else
		{
			context = SDL_GL_CreateContext(window);
		}
	}

	//setup glew
	glewExperimental = GL_TRUE;
	glewInit();

	pScene = std::shared_ptr<Scene>(new World(pSettings));
}//constructor

Window::~Window()
{
	bKeepLooping = false;

	//Destroy window
	SDL_DestroyWindow( window );

	SDL_GL_DeleteContext(context);

	//Quit SDL subsystems
	SDL_Quit();
}//deconstrucotr

void Window::renderLoop()
{
	while(bKeepLooping)
	{
		xEvents.put();
		render();
		SDL_GL_SwapWindow(window);
	}//while
}//function

void Window::loop()
{
	auto xLast = boost::chrono::high_resolution_clock::now();
	while(bKeepLooping)
	{
		processEvents();
		auto xNow = boost::chrono::high_resolution_clock::now();
		double time_span = boost::chrono::duration_cast<boost::chrono::duration<double>>(xNow-xLast).count();
		if(time_span < 0.01)
			boost::this_thread::sleep_for(boost::chrono::milliseconds((int)(10 - time_span*1000)+1));
		xNow = boost::chrono::high_resolution_clock::now();
		time_span = boost::chrono::duration_cast<boost::chrono::duration<double>>(xNow-xLast).count();
		xLast = xNow;
		pScene->update(time_span);
	}//while
}//function

void Window::action(std::string s)
{
	if(pScene != nullptr)
		pScene->action(s, *this);
}///function

void Window::processEvents()
{
	xEvents.pull(
		[&]
		(SDL_Event& event)
		{
            Sint32 x;
			switch( event.type ) 
			{
				case SDL_KEYDOWN:
					try
					{
						action(pSettings->get<std::string>(
							std::string("keys.").append(SDL_GetKeyName(event.key.keysym.sym))
								.append(".down")));
					}
					catch(Exception e)
					{
						try
						{
							action(pSettings->get<std::string>(
								std::string("keys.").append(SDL_GetKeyName(event.key.keysym.sym))));
						}
						catch(Exception e)
						{
							std::cout << std::string("Unknown Key: ").append(SDL_GetKeyName(event.key.keysym.sym)) << std::endl;
						}
					}
					break;
				case SDL_MOUSEWHEEL:
					try
					{
                        x = event.wheel.y;
                        if(x > 0)
                        {
                            auto xScrollUp = pSettings->get<std::string>(std::string("mouse.wheel.scroll.up"));
                            while(x-- > 0)
                                action(xScrollUp);
					        break;
                        }
                        if(x < 0)
                        {
                            auto xScrollDown = pSettings->get<std::string>(std::string("mouse.wheel.scroll.down"));
                            while(x++ < 0)
                                action(xScrollDown);
					        break;
                        }
					}
					catch(Exception e)
					{
						try
						{
                            auto xScroll = pSettings->get<std::string>(std::string("mouse.wheel.scroll"));
						    action(xScroll);
						}
						catch(Exception e)
						{
							std::cout << std::string("Unknown: mouse.wheel.scroll") << std::endl;
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					try
					{
						action(pSettings->get<std::string>(std::string("mouse.wheel.click.down")));
					}
					catch(Exception e)
					{
						try
						{
						    action(pSettings->get<std::string>(std::string("mouse.wheel.click")));
						}
						catch(Exception e)
						{
							std::cout << std::string("Unknown: mouse.wheel.click") << std::endl;
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					try
					{
						action(pSettings->get<std::string>(std::string("mouse.wheel.click.up")));
					}
					catch(Exception e)
					{
						try
						{
						    action(pSettings->get<std::string>(std::string("mouse.wheel.click")));
						}
						catch(Exception e)
						{
							std::cout << std::string("Unknown: Mousewheel") << std::endl;
						}
					}
					break;
				case SDL_KEYUP:
					try
					{
						action(pSettings->get<std::string>(
							std::string("keys.").append(SDL_GetKeyName(event.key.keysym.sym))
								.append(".up")));
					}
					catch(Exception e)
					{
						try
						{
							action(pSettings->get<std::string>(
								std::string("keys.").append(SDL_GetKeyName(event.key.keysym.sym))));
						}
						catch(Exception e)
						{
							std::cout << std::string("Unknown Key: ").append(SDL_GetKeyName(event.key.keysym.sym)) << std::endl;
						}
					}
					break;
				case SDL_MOUSEMOTION:
					/* Handle key release. */
					break;
				case SDL_QUIT:
					/* Handle quit requests (like Ctrl-c). */
					exit(0);
					break;
			}//switch
		}//lambda
	);//pull function
}//function

World::World(std::shared_ptr<Settings> pSettings)
		:
	pSettings(pSettings),
    pTextures(new Textures(pSettings)),
	uiChunkSize(pSettings->get<unsigned int>("chunks.size")),
	fCameraZ(pSettings->get<float>("camera.z")),
	fCamSpeed(pSettings->get<float>("camera.speed")),
	fCamZSpeed(pSettings->get<float>("camera.zspeed")),
	pTiles(new Tiles(pSettings, pTextures))
{
	//initialize the actions
	vActions.push_back(std::make_tuple(
			"quit",
			[]
			(World &wo, Window &w)
			{
				w.bKeepLooping = false;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.left = false",
			[]
			(World &wo, Window &w)
			{
				wo.bCamLeft = false;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.left = true",
			[]
			(World &wo, Window &w)
			{
				wo.bCamLeft = true;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.up = false",
			[]
			(World &wo, Window &w)
			{
				wo.bCamUp = false;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.up = true",
			[]
			(World &wo, Window &w)
			{
				wo.bCamUp = true;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.right = false",
			[]
			(World &wo, Window &w)
			{
				wo.bCamRight = false;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.right = true",
			[]
			(World &wo, Window &w)
			{
				wo.bCamRight = true;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.down = false",
			[]
			(World &wo, Window &w)
			{
				wo.bCamDown = false;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.down = true",
			[]
			(World &wo, Window &w)
			{
				wo.bCamDown = true;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.in = true",
			[]
			(World &wo, Window &w)
			{
				wo.bCamIn = true;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.in = false",
			[]
			(World &wo, Window &w)
			{
				wo.bCamIn = false;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.out = true",
			[]
			(World &wo, Window &w)
			{
				wo.bCamOut = true;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.move.out = false",
			[]
			(World &wo, Window &w)
			{
				wo.bCamOut = false;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.step.out",
			[]
			(World &wo, Window &w)
			{
				wo.fCameraZ += wo.fCamZSpeed*wo.fCameraZ/10;
			}
		));
	vActions.push_back(std::make_tuple(
			"camera.step.in",
			[]
			(World &wo, Window &w)
			{
				wo.fCameraZ -= wo.fCamZSpeed*wo.fCameraZ/10;
                if (wo.fCameraZ < 1)
                    wo.fCameraZ = 1;
			}
		));
}//constructor


void Window::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(pScene != nullptr)
		pScene->render();
}//function

void World::render()
{
    float fX = fCameraX;
    float fY = fCameraY;
	float fZ = fCameraZ;

	for(float x=std::floor(fX-fZ/uiChunkSize); x <= std::ceil(fX+fZ/uiChunkSize); x+=1)
		for(float y=std::floor(fY-fZ/uiChunkSize); y <= std::ceil(fY+fZ/uiChunkSize); y+=1)
        {
			get(x*uiChunkSize, y*uiChunkSize)->draw( uiChunkSize*(x-fX-1)*2/fZ, uiChunkSize*(y-fY-1)*2/fZ, uiChunkSize*2/fZ );
        }// for
}//function

void World::action(std::string s, Window& w)
{
	for(auto tuple : vActions)
		if(std::get<0>(tuple) == s)
		{
			std::get<1>(tuple)(*this, w);
			return;
		}//if
	std::cout << std::string("Unknown Action: ").append(s) << std::endl;
}//function

std::shared_ptr<Chunk> World::get(int x, int y)
{
	auto pos = std::make_tuple(x/uiChunkSize,y/uiChunkSize);
	if(xChunks[pos] == nullptr)
		xChunks[pos] = std::shared_ptr<Chunk>(new Chunk(pTiles, pSettings));
	return xChunks[pos];
}//function

void World::set(int x, int y, unsigned int l, std::shared_ptr<Tile> pTile)
{
	get(x,y)->set(x%uiChunkSize, y%uiChunkSize, l, pTile);
}//function

std::shared_ptr<Tile> World::get(int x, int y, unsigned int l)
{
	return get(x,y)->get(x%uiChunkSize, y%uiChunkSize, l);
}//function

void World::update(float fTimeDif)
{
	if(bCamRight)
		fCameraX += fTimeDif * fCamSpeed * fCameraZ / uiChunkSize;
	if(bCamLeft)
		fCameraX -= fTimeDif * fCamSpeed * fCameraZ / uiChunkSize;
	if(bCamUp)
		fCameraY += fTimeDif * fCamSpeed * fCameraZ / uiChunkSize;
	if(bCamDown)
		fCameraY -= fTimeDif * fCamSpeed * fCameraZ / uiChunkSize;
	if(bCamIn)
    {
		fCameraZ -= fTimeDif * fCamZSpeed * uiChunkSize * fCameraZ / 100;
        if (fCameraZ < 1)
            fCameraZ = 1;
    }// if
	if(bCamOut)
		fCameraZ += fTimeDif * fCamZSpeed * uiChunkSize * fCameraZ / 100;
}
