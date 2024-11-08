#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"


class ModuleMenu : public Module
{
public:
	ModuleMenu(Application* app, bool start_enabled = true);
	~ModuleMenu();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	
	Texture2D menuTexture;
};