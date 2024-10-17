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

protected:
	PhysBody* body;
	Module* listener;
};

class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 25), _listener)
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
	Box(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50), _listener)
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

class Map : public PhysicEntity
{
public:
	// Pivot 0, 0
	static constexpr int pinballCoquete[116] = {
	540, 0,
	540, 720,
	401, 720,
	464, 676,
	463, 540,
	445, 520,
	445, 485,
	467, 460,
	481, 423,
	481, 107,
	478, 95,
	470, 88,
	458, 83,
	447, 86,
	435, 92,
	422, 98,
	408, 106,
	401, 101,
	404, 91,
	414, 81,
	425, 71,
	440, 57,
	454, 53,
	469, 56,
	485, 65,
	496, 77,
	503, 95,
	506, 109,
	506, 540,
	540, 540,
	540, 0,
	0, 0,
	1, 720,
	106, 720,
	41, 679,
	41, 531,
	61, 505,
	61, 431,
	35, 402,
	25, 374,
	22, 354,
	22, 231,
	38, 203,
	41, 165,
	47, 112,
	58, 75,
	72, 48,
	91, 28,
	109, 16,
	133, 9,
	477, 9,
	496, 17,
	510, 30,
	522, 46,
	529, 65,
	534, 86,
	540, 120,
	540, 540
	};


	Map(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(_x , _y , pinballCoquete, 116), _listener)
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

	pimball_map = LoadTexture("Assets/PinballCoquete1.png");
	circle = LoadTexture("Assets/wheel.png"); 
	box = LoadTexture("Assets/crate.png");
	
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	
	entities.emplace_back(new Map(App->physics, 0, 0, this, pimball_map));

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
		entities.emplace_back(new Circle(App->physics, GetMouseX(), GetMouseY(), this, circle));
		
	}

	if(IsKeyPressed(KEY_TWO))
	{
		entities.emplace_back(new Box(App->physics, GetMouseX(), GetMouseY(), this, box));
	}

	if(IsKeyPressed(KEY_THREE))
	{
		entities.emplace_back(new Map(App->physics, GetMouseX(), GetMouseY(), this, pimball_map));
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
