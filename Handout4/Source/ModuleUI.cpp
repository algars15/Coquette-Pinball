#pragma once

#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleUI.h"
#include "Module.h"
#include "Application.h"


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Start()
{
	
	duracionFlotante = 2;
	LOG("Loading Intro assets");
	bool ret = true;


	return ret;
}

void ModuleUI::ShowPuntuation(int p, int x, int y)
{
	PuntuacionFlotante* pf = new PuntuacionFlotante();
	pf->puntuacion = p;
	pf->x = x;
	pf->y = y;
	pf->duracion = duracionFlotante;
	pf->tiempoTranscurrido = 0;
	puntuacionesFlotantes.push_back(pf);
}

update_status ModuleUI::Update()
{
	for (int i = 0; i < puntuacionesFlotantes.size(); i++)
	{
		puntuacionesFlotantes[i]->tiempoTranscurrido += GetFrameTime();
		if (puntuacionesFlotantes[i]->tiempoTranscurrido > puntuacionesFlotantes[i]->duracion)
		{
			delete(puntuacionesFlotantes[i]);
			puntuacionesFlotantes.erase(puntuacionesFlotantes.begin() + i);
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleUI::Draw(int puntuation, int lives)
{
	for (int i = 0; i < puntuacionesFlotantes.size(); i++)
	{
		char buffer[16];
		sprintf_s(buffer, "%d", puntuacionesFlotantes[i]->puntuacion);
		DrawText(buffer, puntuacionesFlotantes[i]->x, puntuacionesFlotantes[i]->y, 32, WHITE);
	}
	char puntuacion[16];
	sprintf_s(puntuacion, "%d", puntuation);
	DrawText(puntuacion, 30, 30, 32, WHITE);
	char vidas[16];
	sprintf_s(vidas, "%d", lives);
	DrawText(vidas, 30, 60, 32, WHITE);
}


bool ModuleUI::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}