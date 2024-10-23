#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		body->listener = listener;
	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

public:
	PhysBody* body;
	Module* listener;
};

class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, int radious, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, radious), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 1.0f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f};
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;

};

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, int w, int h, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, w, h), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f}, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;

};

class Shape : public PhysicEntity
{
public:
	// Pivot 0, 0


	Shape(ModulePhysics* physics, int _x, int _y, int points[], int numberOfPoints, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(_x , _y , points, numberOfPoints), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 1.0f, WHITE);
	}
private:
	Texture2D texture;
};


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	pimball_map = LoadTexture("Assets/map.png");
	circle = LoadTexture("Assets/bola.png"); 
	box = LoadTexture("Assets/crate.png");
	palancaTexture = LoadTexture("Assets/palanca.png");
	palanca_invertida = LoadTexture("Assets/palanca_inverted.png");
	
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	
	//MAP
	int map[82] = {
	480, 0,
	480, 640,
	360, 640,
	413, 605,
	413, 475,
	406, 463,
	397, 458,
	397, 433,
	416, 414,
	429, 379,
	429, 84,
	421, 71,
	402, 65,
	367, 87,
	355, 88,
	393, 51,
	417, 52,
	438, 66,
	447, 95,
	447, 480,
	480, 480,
	480, 70,
	474, 41,
	453, 15,
	432, 5,
	98, 5,
	76, 16,
	54, 42,
	35, 95,
	30, 151,
	30, 178,
	18, 192,
	18, 325,
	27, 355,
	52, 383,
	52, 443,
	33, 465,
	33, 605,
	88, 640,
	0, 640,
	0, 0
	};

	int map1[36] = {
	295, 283,
	351, 256,
	371, 233,
	373, 191,
	373, 146,
	381, 141,
	389, 141,
	395, 147,
	395, 183,
	395, 284,
	395, 348,
	388, 355,
	377, 356,
	311, 311,
	300, 311,
	304, 302,
	303, 291,
	299, 286
	};

	int map2[42] = {
	82, 206,
	90, 216,
	102, 255,
	117, 269,
	154, 284,
	141, 287,
	141, 301,
	155, 315,
	144, 316,
	106, 352,
	76, 327,
	81, 313,
	69, 298,
	80, 279,
	69, 273,
	69, 266,
	81, 248,
	68, 240,
	67, 231,
	73, 226,
	73, 212
	};

	int map3[20] = {
	65, 133,
	66, 115,
	74, 84,
	96, 47,
	106, 42,
	118, 41,
	128, 48,
	144, 61,
	145, 75,
	74, 142
	};

	int map4[14] = {
	76, 523,
	82, 517,
	90, 522,
	90, 571,
	155, 604,
	137, 607,
	77, 578
	};

	int map5[14] = {
	292, 604,
	357, 571,
	357, 523,
	363, 520,
	370, 525,
	370, 576,
	307, 605
	};

	int palanca[20] = {
	2, 0,
	57, 0,
	60, 3,
	60, 16,
	57, 20,
	52, 19,
	4, 12,
	0, 8,
	0, 3,
	2, 0
	};



	entities.emplace_back(new Shape(App->physics, 0, 0, map, 82, this, pimball_map));
	entities.emplace_back(new Shape(App->physics, 0, 0, map1, 36, this, pimball_map));
	entities.emplace_back(new Shape(App->physics, 0, 0, map2, 42, this, pimball_map));
	entities.emplace_back(new Shape(App->physics, 0, 0, map3, 20, this, pimball_map));
	entities.emplace_back(new Shape(App->physics, 0, 0, map4, 14, this, pimball_map));
	entities.emplace_back(new Shape(App->physics, 0, 0, map5, 14, this, pimball_map));

	palancaIzquierda = new Box(App->physics, 215 - palanca_invertida.width / 2, 607 + palanca_invertida.height / 2, palanca_invertida.width, palanca_invertida.height, this, palanca_invertida);
	palancaDerecha = new Box(App->physics, 295 - palancaTexture.width / 2, 607 + palancaTexture.height / 2, palancaTexture.width, palancaTexture.height, this, palancaTexture);

	entities.emplace_back(palancaDerecha);
	entities.emplace_back(palancaIzquierda);

	jointPalancaIzquierda = App->physics->CreateWeldJoint(palancaIzquierda->body, palancaDerecha->body);

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	

	if(IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if(IsKeyPressed(KEY_ONE))
	{
		entities.emplace_back(new Circle(App->physics, GetMouseX(), GetMouseY(), circle.width/2, this, circle));
		
	}

	if(IsKeyPressed(KEY_TWO))
	{
		entities.emplace_back(new Box(App->physics, GetMouseX(), GetMouseY(), palancaTexture.width, palancaTexture.height, this, palancaTexture));
	}

	// Prepare for raycast ------------------------------------------------------
	
	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	for (PhysicEntity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
	}
	

	// ray -----------------
	if(ray_on == true)
	{
		vec2f destination((float)(mouse.x-ray.x), (float)(mouse.y-ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}
