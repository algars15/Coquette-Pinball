#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include "box2d.h"

class PhysBody;
class PhysicEntity;
class Box;



class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);



public:

	std::vector<PhysicEntity*> entities;
	
	PhysBody* sensor;
	bool sensed;

	Texture2D pimball_map;
	Texture2D circle;
	Texture2D box;
	Texture2D palancaTexture;
	Texture2D palanca_invertida;
	Texture2D springTop;
	Texture2D springBottom;

	uint32 bonus_fx;

	Box* shooter;
	b2Vec2 shooterInitPos;
	float distancia;
	b2Vec2 shotVel;
	


	vec2<int> ray;
	bool ray_on;
};
