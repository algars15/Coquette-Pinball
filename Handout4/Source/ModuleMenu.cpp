#pragma once

#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "ModuleMenu.h"
#include "Module.h"
#include "Application.h"


ModuleMenu::ModuleMenu(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleMenu::~ModuleMenu()
{

}

bool ModuleMenu::Start()
{
	menuTexture = LoadTexture("Assets/menu.png");
	loseTexture = LoadTexture("Assets/lose_screen");

	LOG("Loading Intro assets");
	bool ret = true;


	return ret;
}

update_status ModuleMenu::Update()
{
	DrawTexture(menuTexture, 0, 0, WHITE);
	return UPDATE_CONTINUE;
}



bool ModuleMenu::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}