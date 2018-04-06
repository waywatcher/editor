#include "main.h"

std::ostream& operator<<(std::ostream& out, const Json& json)
{
	boost::property_tree::write_json(out, json.tree);
	return out;
}//operator

int SDL_main(int argc, char *argv[])
{
	try
	{
		std::shared_ptr<Settings> pSettings(new Settings());

		Window w(pSettings);

		//the logic thread
		//currently one but might spawn a lot of threads if python is too slow
		std::thread logic([&](){w.loop();});

		//the rendering thread
		w.renderLoop();

		//make sure the renderer is also done
		logic.join();

		return 0;
	}
	catch (Exception e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}
}//function