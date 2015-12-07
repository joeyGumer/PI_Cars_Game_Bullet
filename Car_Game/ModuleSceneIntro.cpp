#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateCircuit();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	p2List_item<Cube>* tmp;
	tmp = circuitcube_list.getFirst();
	while (tmp != NULL)
	{
		tmp->data.Render();
		tmp = tmp->next;
	}

	p2List_item<Cylinder>* tmp2;
	tmp2 = circuitcylinder_list.getFirst();
	while (tmp2 != NULL)
	{
		tmp2->data.Render();
		tmp2 = tmp2->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateCircuit()
{
	//1stSegment
	Cube segment1(8, 1, 30);
	segment1.SetPos(0, 3, 0);
	segment1.color.Set(255, 0, 0);
	circuitcube_list.add(segment1);
	PhysBody3D* segmentbody1 = App->physics->AddBody(segment1, 0.0f);
	circuitbody_list.add(segmentbody1);

	//2ndSegment
	Cylinder segment2(4, 1);
	segment2.SetPos(0, 3, 15);
	segment2.SetRotation(90, { 0, 0, 1 });
	segment2.color.Set(255, 0, 0);
	circuitcylinder_list.add(segment2);
	PhysBody3D* segmentbody2 = App->physics->AddBody(segment2, 0.0f);
	circuitbody_list.add(segmentbody2);

	//3rdSegment
	Cube segment3(8, 1, 30);
	segment3.SetPos(-6, 3, 30);
	segment3.SetRotation(-20, { 0, 1, 0 });
	segment3.color.Set(255, 0, 0);
	circuitcube_list.add(segment3);
	PhysBody3D* segmentbody3 = App->physics->AddBody(segment3, 0.0f);
	circuitbody_list.add(segmentbody3);
	/*
	Cube circuit2(8, 1, 45);
	circuit2.SetPos(-7.5f, 3, 34.8f);
	circuit2.SetRotation(340, { 0, 1, 0 });
	circuit_list.add(circuit2);
	PhysBody3D* circuitbody2 = App->physics->AddBody(circuit2, 0.0f);
	circuitbody_list.add(circuitbody2);
	*/
	
}

