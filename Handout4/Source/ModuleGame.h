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
class b2PrismaticJoint;


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
	bool GetReturnMain();
	void RestartGame();

public:

	std::vector<PhysicEntity*> entities;
	
	PhysBody* sensor;
	bool sensed;

	Texture2D pimball_map;
	Texture2D pimball_map2;
	Texture2D circle;
	Texture2D box;
	Texture2D palancaTexture;
	Texture2D palanca_invertida;
	Texture2D loseScreen;
	Texture2D spring;
	Texture2D springTop;
	Texture2D springBottom;

	Circle* bola;
	Box* palancaDerecha;
	Box* palancaIzquierda;
	PhysBody* unionPalancaDerecha;
	PhysBody* unionPalancaIzquierda;

	Box* mollaTop;
	Box* mollaBottom;
	Box* molla;
	Shape* base;


	b2RevoluteJoint* jointPalancaIzquierda;
	b2RevoluteJoint* jointPalancaDerecha;

	b2PrismaticJoint* jointMolla;

	//uint32 bonus_fx;
	uint32 springSound;
	uint32 flipperSound;
	uint32 bouncerSound;

	bool mort;
	bool returnMain;
	int vides;
	//score
	int score;
	int record;
	int previous;

	vec2<int> ray;
	bool ray_on;
	int velocitatPalanca;
	int forcaImpuls;
	Vector2 startPos;

};
