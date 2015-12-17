#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

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

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreateCircuit();
	void CreateObstacles();
	void CreateDynObstacles();

	void UpdateDynObstacles();
	void ResetDynObstacles();

public:

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

};
