#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Timer.h"

#define NUM_CHECKPOINTS 3
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

	void ResetDynObstacles();
	
private:
	void CreateCircuit();
	void CreateCheckpoints();
	void CreateObstacles();
	void CreateDynObstacles();

	void UpdateDynObstacles();

	void RenderScene();


public:

	//Scene variables (not objects)
	int next_checkpoint_index = 0;
	int lap_count = 1;

	Timer play_timer;
	int best_time_min = 0;
	int best_time_sec = 0;

	bool debug = false;

	//Car
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	//Circuit
	p2List<PhysBody3D*> circuitbody_list;
	p2List<Cube> circuitcube_list;

	
	//CheckPoint
	p2List<PhysBody3D*> pb_checkpoint_list;
	p2List<Cube> checkpoint_list;

	//Static Obstacles
	p2List<PhysBody3D*> obstaclebody_list;
	p2List<Cylinder> obstaclecylinder_list;
	p2List<Cube> obstaclecube_list;
	p2List<Sphere> obstaclesphere_list;

	//Dynamic Obstacles
	PhysBody3D* pb_spinningcube;
	Cube p_spinningcube;
	int angle;

	PhysBody3D* pb_pendulum;
	Sphere p_pendulum;
	PhysBody3D* pb_panchor;
	Sphere p_panchor;

	PhysBody3D* pb_door1;
	Cube p_door1;
	mat4x4 origin_door1;
	PhysBody3D* pb_d1anchor;
	Sphere p_d1anchor;

	PhysBody3D* pb_door2;
	Cube p_door2;
	mat4x4 origin_door2;
	PhysBody3D* pb_d2anchor;
	Sphere p_d2anchor;

	//SoundFX
	int lapFX;
	int checkpointFX;
	int victoryFX;
	int crash1FX;

};
