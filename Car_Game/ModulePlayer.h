#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;
struct PhysBody3D;

#define MAX_ACCELERATION 3500.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 250.0f


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	void Reset();

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	mat4x4 origin;

	PhysBody3D* last_checkpoint = NULL;

	bool isJumping;

	//SoundFX
	int jumpFX;
	int crash2FX;
};