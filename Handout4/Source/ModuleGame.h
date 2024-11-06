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
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, Vector2 normal);
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

	Box* palancaDerecha;
	Box* palancaIzquierda;
	PhysBody* unionPalancaDerecha;
	PhysBody* unionPalancaIzquierda;

	b2RevoluteJoint* jointPalancaIzquierda;
	b2RevoluteJoint* jointPalancaDerecha;
	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;
	int velocitatPalanca;
	int forcaImpuls;
};
