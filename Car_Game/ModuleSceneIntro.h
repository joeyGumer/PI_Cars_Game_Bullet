#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Timer.h"

#define NUM_CHECKPOINTS 2
#define NUM_LAPS 3

struct PhysBody3D;
struct PhysMotor3D;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2, PhysEvent pevent);

	void CreateCircuit();
	void CreateCheckpoints();

public:

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2List<PhysBody3D*> circuitbody_list;
	p2List<Cube> circuitcube_list;
	
	//p2List<Cylinder> circuitcylinder_list;

	p2List<PhysBody3D*> pb_checkpoint_list;
	p2List<Cube> checkpoint_list;

	int next_checkpoint_index = 0;
	int lap_count = 1;

	Timer play_timer;
	int best_time_min = 0;
	int best_time_sec = 0;

	bool debug = false;
};
