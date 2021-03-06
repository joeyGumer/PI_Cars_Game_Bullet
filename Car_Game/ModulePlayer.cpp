#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	isJumping = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 0.5, 4);
	car.chassis_offset.Set(0, 0.5, 0.2);
	car.mass = 1000.0f;
	car.suspensionStiffness = 100.88f;
	car.suspensionCompression = 3.0f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 1000.5;
	car.maxSuspensionForce = 5000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.5f;
	float wheel_radius = 0.4f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width + 0.4f, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width - 0.4f, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width + 0.4f, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width - 0.4f, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 3, 2);
	vehicle->GetTransform(origin.M);

	jumpFX = App->audio->LoadFx("Game/SoundFX/jump.wav");
	crash2FX = App->audio->LoadFx("Game/SoundFX/crash2.wav");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	
	
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || vehicle->GetBody()->getWorldTransform().getOrigin().getY() < 1)
	{
		if (vehicle->GetBody()->getWorldTransform().getOrigin().getY() < 1)
		{
			App->audio->PlayFx(crash2FX);
		}
		Reset();
	}

	/*if (vehicle->GetBody()->getWorldTransform().getOrigin().getY() < 1)
	{
		vehicle->GetBody()->setLinearVelocity({ 0, 0, 0 });
		vehicle->SetTransform(origin.M);
		App->scene_intro->ResetDynObstacles();
	}*/
	
	
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
			acceleration = MAX_ACCELERATION * 2;
				
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0)
			brake = BRAKE_POWER;
		else
			acceleration = -MAX_ACCELERATION * 0.5;
	}

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN && !isJumping)
	{
		vehicle->GetBody()->applyCentralForce({ 0, 500000, 0 });
		App->audio->PlayFx(jumpFX);
		isJumping = true;
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	/*char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);*/

	//----------------
	
	//----------------
	return UPDATE_CONTINUE;
}

void ModulePlayer::Reset()
{
	//Vehicle Reset
	mat4x4 transform;
	last_checkpoint->GetTransform(&transform);
	vehicle->SetTransform(&transform);
	vehicle->GetBody()->setLinearVelocity({ 0, 0, 0 });
	vehicle->GetBody()->setAngularVelocity({ 0, 0, 0 });
	
	//Camera Reset
	App->camera->Reset();

	//Obstacles Reset
	App->scene_intro->ResetDynObstacles();
}

