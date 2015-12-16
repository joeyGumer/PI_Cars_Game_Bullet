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

	PhysBody3D* last_checkpoint = NULL;
};
