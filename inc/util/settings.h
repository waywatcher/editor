#ifndef SETTINGS_H
#define SETTINGS_H

#include "util/json.h"

class Settings : public Json
{
public:
	Settings()
			:
		Json("gameSettings")
	{}//constructor
};//class

#endif