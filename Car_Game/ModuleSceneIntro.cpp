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

	/*App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));*/

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
	Cube segment2(12, 1, 12);
	segment2.SetPos(0, 3, 15);
	//segment2.SetRotation(90, { 0, 0, 1 });
	segment2.color.Set(255, 0, 0);
	circuitcube_list.add(segment2);
	PhysBody3D* segmentbody2 = App->physics->AddBody(segment2, 0.0f);
	circuitbody_list.add(segmentbody2);
	

	//3rdSegment
	Cube segment3(8, 1, 40);
	segment3.SetPos(-7, 3, 37);
	segment3.SetRotation(-20, { 0, 1, 0 });
	segment3.color.Set(255, 0, 0);
	circuitcube_list.add(segment3);
	PhysBody3D* segmentbody3 = App->physics->AddBody(segment3, 0.0f);
	circuitbody_list.add(segmentbody3);

	//4thSegment
	Cube segment4(12, 1, 12);
	segment4.SetPos(-13, 3, 60);
	segment4.SetRotation(-10, { 0, 1, 0 });
	segment4.color.Set(255, 0, 0);
	circuitcube_list.add(segment4);
	PhysBody3D* segmentbody4 = App->physics->AddBody(segment4, 0.0f);
	circuitbody_list.add(segmentbody4);

	//5thSegment
	Cube segment5(8, 1, 30);
	segment5.SetPos(-7, 3, 75);
	segment5.SetRotation(30, { 0, 1, 0 });
	segment5.color.Set(255, 0, 0);
	circuitcube_list.add(segment5);
	PhysBody3D* segmentbody5 = App->physics->AddBody(segment5, 0.0f);
	circuitbody_list.add(segmentbody5);

	//6thSegment
	Cube segment6(12, 1, 12);
	segment6.SetPos(1.5f, 3, 90);
	segment6.SetRotation(20, { 0, 1, 0 });
	segment6.color.Set(255, 0, 0);
	circuitcube_list.add(segment6);
	PhysBody3D* segmentbody6 = App->physics->AddBody(segment6, 0.0f);
	circuitbody_list.add(segmentbody6);

	//7thSegment
	Cube segment7(35, 1, 8);
	segment7.SetPos(22, 3, 90);
	segment7.color.Set(255, 0, 0);
	circuitcube_list.add(segment7);
	PhysBody3D* segmentbody7 = App->physics->AddBody(segment7, 0.0f);
	circuitbody_list.add(segmentbody7);

	//8thSegment
	Cube segment8(12, 1, 12);
	segment8.SetPos(41, 3, 90);
	segment8.SetRotation(-10, { 0, 1, 0 });
	segment8.color.Set(255, 0, 0);
	circuitcube_list.add(segment8);
	PhysBody3D* segmentbody8 = App->physics->AddBody(segment8, 0.0f);
	circuitbody_list.add(segmentbody8);

	//9thSegment
	Cube segment9(105, 1, 8);
	mat4x4 a = segment9.transform.rotate(45, { 0, 1, 0 });
	mat4x4 b = segment9.transform.rotate(20, { 0, 0, 1 });
	mat4x4 c = segment9.transform.rotate(0, { 1, 0, 0 });
	segment9.transform = segment9.transform * a * b * c;
	segment9.SetPos(78, 20, 55);
	segment9.color.Set(255, 0, 0);
	circuitcube_list.add(segment9);
	PhysBody3D* segmentbody9 = App->physics->AddBody(segment9, 0.0f);
	circuitbody_list.add(segmentbody9);

	//10thSegment
	Cube segment10(12, 1, 8);
	segment10.SetPos(115.2f, 37.92f, 17.0f);
	segment10.SetRotation(-45, { 0, 1, 0 });
	segment10.color.Set(255, 0, 0);
	circuitcube_list.add(segment10);
	PhysBody3D* segmentbody10 = App->physics->AddBody(segment10, 0.0f);
	circuitbody_list.add(segmentbody10);

	//11thSegment
	Cube segment11(8, 1, 42);
	segment11.SetPos(114, 37.92f, -6);
	segment11.color.Set(255, 0, 0);
	circuitcube_list.add(segment11);
	PhysBody3D* segmentbody11 = App->physics->AddBody(segment11, 0.0f);
	circuitbody_list.add(segmentbody11);

	//12thSegment
	Cube segment12(16, 1, 16);
	segment12.SetPos(114, 37.92f, -34);
	segment12.SetRotation(90, { 0, 1, 0 });
	segment12.color.Set(255, 0, 0);
	circuitcube_list.add(segment12);
	PhysBody3D* segmentbody12 = App->physics->AddBody(segment12, 0.0f);
	circuitbody_list.add(segmentbody12);

	//13thSegment
	Cube segment13(8, 1, 70);
	segment13.SetPos(72, 37.92f, -34);
	segment13.SetRotation(90, { 0, 1, 0 });
	segment13.color.Set(255, 0, 0);
	circuitcube_list.add(segment13);
	PhysBody3D* segmentbody13 = App->physics->AddBody(segment13, 0.0f);
	circuitbody_list.add(segmentbody13);

	//14thSegment
	Cube segment14(8, 1, 70);
	segment14.SetPos(-15, 37.92f, -34);
	segment14.SetRotation(90, { 0, 1, 0 });
	segment14.color.Set(255, 0, 0);
	circuitcube_list.add(segment14);
	PhysBody3D* segmentbody14 = App->physics->AddBody(segment14, 0.0f);
	circuitbody_list.add(segmentbody14);

	//15thSegment
	Cube segment15(16, 1, 16);
	segment15.SetPos(-57, 37.92f, -34);
	segment15.color.Set(255, 0, 0);
	circuitcube_list.add(segment15);
	PhysBody3D* segmentbody15 = App->physics->AddBody(segment15, 0.0f);
	circuitbody_list.add(segmentbody15);

	//16thSegment
	Cube segment16(8, 1, 60);
	mat4x4 d = segment16.transform.rotate(0, { 0, 1, 0 });
	mat4x4 e = segment16.transform.rotate(0, { 0, 0, 1 });
	mat4x4 f = segment16.transform.rotate(-10, { 1, 0, 0 });
	segment16.transform = segment16.transform * d * e * f;
	segment16.SetPos(-57, 37.92f, -70);
	segment16.color.Set(255, 0, 0);
	circuitcube_list.add(segment16);
	PhysBody3D* segmentbody16 = App->physics->AddBody(segment16, 0.0f);
	circuitbody_list.add(segmentbody16);
}

