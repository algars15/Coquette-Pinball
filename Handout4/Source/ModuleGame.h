#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;

class Box; 
class Circle;
class Shape;

class b2RevoluteJoint;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void UpdateFlipper(b2RevoluteJoint* joint, bool isPressed, bool right);

public:

	std::vector<PhysicEntity*> entities;
	
	PhysBody* sensor;
	bool sensed;

	Texture2D pimball_map;
	Texture2D circle;
	Texture2D box;
	Texture2D palancaTexture;
	Texture2D palanca_invertida;
	Texture2D loseScreen;
	//Texture2D spring;
	//Texture2D springTop;
	//Texture2D springBottom;

	Circle* bola;
	Box* palancaDerecha;
	Box* palancaIzquierda;
	PhysBody* unionPalancaDerecha;
	PhysBody* unionPalancaIzquierda;

	//Box* mollaTop;
	//Box* mollaBottom;

	b2RevoluteJoint* jointPalancaIzquierda;
	b2RevoluteJoint* jointPalancaDerecha;

	uint32 bonus_fx;

	bool mort;
	int vides;

	vec2<int> ray;
	bool ray_on;
	int velocitatPalanca;
};
