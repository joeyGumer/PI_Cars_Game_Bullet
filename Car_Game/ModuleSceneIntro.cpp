#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"



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
	CreateCheckpoints();

	play_timer.Start();

	App->audio->PlayMusic("Game/sans.ogg");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	circuitbody_list.clear();
	circuitcube_list.clear();
	checkpoint_list.clear();
	pb_checkpoint_list.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	Plane p(0, 1, 0, 0);
	p.color.Set(0, 255, 255);
	p.Render();
	
	p2List_item<Cube>* tmp;
	tmp = circuitcube_list.getFirst();
	for (; tmp; tmp = tmp->next)
		tmp->data.Render();

	if (debug)
	{
		tmp = checkpoint_list.getFirst();
		for (; tmp; tmp = tmp->next)
			tmp->data.Render();
	}
	
	//Put all the painting functions on a simple function
	Cube post1(1, 9, 1);
	post1.SetPos(-5 , 7,0);
	post1.color = Blue;
	post1.Render();

	Cube post2(1, 9, 1);
	post2.SetPos(5, 7, 0);
	post2.color = Blue;
	post2.Render();

	Cube flag(9, 2, 1);
	flag.SetPos(0, 10.5f, 0);
	flag.color = Green;
	flag.Render();

	Cube paint1(9, 0, 1);
	paint1.SetPos(0, 3.55f, 0);
	//paint1.color = Blue;
	paint1.Render();

	Cube paint2(9, 0, 1);
	paint2.SetPos(10, 38.5f, -34);
	paint2.SetRotation(-90, { 0, 1, 0 });
	//paint1.color = Blue;
	paint2.Render();

		Sphere lap1(1);
		lap1.SetPos(3, 10.5f, 0);
		lap1.Render();
		if (lap_count >= 2)
		{
			Sphere lap2(1);
			lap2.SetPos(0, 10.5f, 0);
			lap2.Render();
			if (lap_count >= 3)
			{
				Sphere lap3(1);
				lap3.SetPos(-3, 10.5f, 0);
				lap3.Render();
				if (lap_count > 3)
				{
					play_timer.Stop();

					if (best_time_sec == 0 || play_timer.Read() / 1000 < best_time_sec)
					{
						best_time_sec = play_timer.Read() / 1000;
						best_time_min = best_time_sec / 60;
					}

					lap_count = 1;
					play_timer.Start();
				}
			}
		}
			
	

	//p2List_item<Cylinder>* tmp2;
	
	/*tmp2 = circuitcylinder_list.getFirst();
	while (tmp2 != NULL)
	{
		tmp2->data.Render();
		tmp2 = tmp2->next;
	}*/

	char title[80];
	int sec = play_timer.Read() / 1000;
	int min = sec/60;

	sprintf_s(title, "Lap: %d / %d ---- Time: %d min %d sec --- Best time: %d min %d sec", lap_count, NUM_LAPS, min, sec % 60, best_time_min, best_time_sec % 60);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2, PhysEvent pevent)
{
	//Provisional
	if (body1->IsSensor() /*&& body2 == pb_chassis*/)
	{
		if (pevent == BEGIN_CONTACT)
		{
			if (next_checkpoint_index != pb_checkpoint_list.find(body1))
			{
				App->player->last_checkpoint = body1;
				next_checkpoint_index++;
				
				if (next_checkpoint_index == NUM_CHECKPOINTS)
				{
					next_checkpoint_index = 0;
					lap_count++;
				}
			}
			
		}
	}
}

void ModuleSceneIntro::CreateCheckpoints()
{
	//Checkpoint 1
	Cube checkpoint1(9, 5, 1);
	checkpoint1.SetPos(0, 5, 0);
	checkpoint1.color = Green;
	checkpoint_list.add(checkpoint1);
	PhysBody3D* pb_checkpoint1 = App->physics->AddBody(checkpoint1, 0.0f);
	pb_checkpoint1->SetAsSensor(true);
	pb_checkpoint1->collision_listeners.add(this);
	pb_checkpoint_list.add(pb_checkpoint1);

	App->player->last_checkpoint = pb_checkpoint1;

	//Checkpoint 2
	Cube checkpoint2(9, 5, 1);
	checkpoint2.SetPos(10, 40.92f, -34);
	checkpoint2.SetRotation(-90, { 0, 1, 0 });
	checkpoint2.color = Green;
	checkpoint_list.add(checkpoint2);
	PhysBody3D* pb_checkpoint2 = App->physics->AddBody(checkpoint2, 0.0f);
	pb_checkpoint2->SetAsSensor(true);
	pb_checkpoint2->collision_listeners.add(this);
	pb_checkpoint_list.add(pb_checkpoint2);

	

}
void ModuleSceneIntro::CreateCircuit()
{
	//1stSegment
	Cube segment1(9, 1, 30);
	segment1.SetPos(0, 3, 0);
	segment1.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment1);
	PhysBody3D* segmentbody1 = App->physics->AddBody(segment1, 0.0f);
	circuitbody_list.add(segmentbody1);
	

	//2ndSegment
	Cube segment2(12, 1, 12);
	segment2.SetPos(0, 3, 15);
	segment2.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment2);
	PhysBody3D* segmentbody2 = App->physics->AddBody(segment2, 0.0f);
	circuitbody_list.add(segmentbody2);
	

	//3rdSegment
	Cube segment3(9, 1, 40);
	segment3.SetPos(-7, 3, 37);
	segment3.SetRotation(-20, { 0, 1, 0 });
	segment3.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment3);
	PhysBody3D* segmentbody3 = App->physics->AddBody(segment3, 0.0f);
	circuitbody_list.add(segmentbody3);

	//4thSegment
	Cube segment4(12, 1, 12);
	segment4.SetPos(-13, 3, 60);
	segment4.SetRotation(-10, { 0, 1, 0 });
	segment4.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment4);
	PhysBody3D* segmentbody4 = App->physics->AddBody(segment4, 0.0f);
	circuitbody_list.add(segmentbody4);

	//5thSegment
	Cube segment5(9, 1, 30);
	segment5.SetPos(-7, 3, 75);
	segment5.SetRotation(30, { 0, 1, 0 });
	segment5.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment5);
	PhysBody3D* segmentbody5 = App->physics->AddBody(segment5, 0.0f);
	circuitbody_list.add(segmentbody5);

	//6thSegment
	Cube segment6(12, 1, 12);
	segment6.SetPos(1.5f, 3, 90);
	segment6.SetRotation(20, { 0, 1, 0 });
	segment6.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment6);
	PhysBody3D* segmentbody6 = App->physics->AddBody(segment6, 0.0f);
	circuitbody_list.add(segmentbody6);

	//7thSegment
	Cube segment7(35, 1, 9);
	segment7.SetPos(22, 3, 90);
	segment7.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment7);
	PhysBody3D* segmentbody7 = App->physics->AddBody(segment7, 0.0f);
	circuitbody_list.add(segmentbody7);

	//8thSegment
	Cube segment8(16, 1, 16);
	segment8.SetPos(41, 3, 90);
	segment8.SetRotation(-10, { 0, 1, 0 });
	segment8.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment8);
	PhysBody3D* segmentbody8 = App->physics->AddBody(segment8, 0.0f);
	circuitbody_list.add(segmentbody8);

	//9thSegment
	Cube segment9(105, 1, 8);
	mat4x4 a = segment9.transform.rotate(45, { 0, 1, 0 });
	mat4x4 b = segment9.transform.rotate(20, { 0, 0, 1 });
	mat4x4 c = segment9.transform.rotate(0, { 1, 0, 0 });
	segment9.transform = segment9.transform * a * b * c;
	segment9.SetPos(78, 20.05, 55);
	segment9.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment9);
	PhysBody3D* segmentbody9 = App->physics->AddBody(segment9, 0.0f);
	circuitbody_list.add(segmentbody9);

	//10thSegment
	Cube segment10(12, 1, 8);
	segment10.SetPos(115.2f, 37.92f, 17.0f);
	segment10.SetRotation(-45, { 0, 1, 0 });
	segment10.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment10);
	PhysBody3D* segmentbody10 = App->physics->AddBody(segment10, 0.0f);
	circuitbody_list.add(segmentbody10);

	//11thSegment
	Cube segment11(9, 1, 42);
	segment11.SetPos(114, 37.92f, -6);
	segment11.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment11);
	PhysBody3D* segmentbody11 = App->physics->AddBody(segment11, 0.0f);
	circuitbody_list.add(segmentbody11);

	//12thSegment
	Cube segment12(18, 1, 18);
	segment12.SetPos(114, 37.92f, -34);
	segment12.SetRotation(90, { 0, 1, 0 });
	segment12.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment12);
	PhysBody3D* segmentbody12 = App->physics->AddBody(segment12, 0.0f);
	circuitbody_list.add(segmentbody12);

	//13thSegment
	Cube segment13(9, 1, 70);
	segment13.SetPos(72, 37.92f, -34);
	segment13.SetRotation(90, { 0, 1, 0 });
	segment13.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment13);
	PhysBody3D* segmentbody13 = App->physics->AddBody(segment13, 0.0f);
	circuitbody_list.add(segmentbody13);

	//14thSegment
	Cube segment14(9, 1, 70);
	segment14.SetPos(-15, 37.92f, -34);
	segment14.SetRotation(90, { 0, 1, 0 });
	segment14.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment14);
	PhysBody3D* segmentbody14 = App->physics->AddBody(segment14, 0.0f);
	circuitbody_list.add(segmentbody14);

	//15thSegment
	Cube segment15(18, 1, 18);
	segment15.SetPos(-57, 37.92f, -34);
	segment15.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment15);
	PhysBody3D* segmentbody15 = App->physics->AddBody(segment15, 0.0f);
	circuitbody_list.add(segmentbody15);

	//16thSegment
	Cube segment16(9, 1, 60);
	mat4x4 d = segment16.transform.rotate(0, { 0, 1, 0 });
	mat4x4 e = segment16.transform.rotate(0, { 0, 0, 1 });
	mat4x4 f = segment16.transform.rotate(-10, { 1, 0, 0 });
	segment16.transform = segment16.transform * d * e * f;
	segment16.SetPos(-57, 27.69f, -70);
	segment16.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment16);
	PhysBody3D* segmentbody16 = App->physics->AddBody(segment16, 0.0f);
	circuitbody_list.add(segmentbody16);

	//17thSegment
	Cube segment17(18, 1, 18);
	segment17.SetPos(-57, 17.5f, -105);
	segment17.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment17);
	PhysBody3D* segmentbody17 = App->physics->AddBody(segment17, 0.0f);
	circuitbody_list.add(segmentbody17);

	//18thSegment
	Cube segment18(43.8f, 1, 9);
	mat4x4 g = segment18.transform.rotate(0, { 0, 1, 0 });
	mat4x4 h = segment18.transform.rotate(-19.315f, { 0, 0, 1 });
	mat4x4 i = segment18.transform.rotate(0, { 1, 0, 0 });
	segment18.transform = segment18.transform * g * h * i;
	segment18.SetPos(-28.5f, 10.265f, -105);
	segment18.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment18);
	PhysBody3D* segmentbody18 = App->physics->AddBody(segment18, 0.0f);
	circuitbody_list.add(segmentbody18);


	//19thSegment
	Cube segment19(18, 1, 18);
	segment19.SetPos(0, 3, -105);
	segment19.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment19);
	PhysBody3D* segmentbody19 = App->physics->AddBody(segment19, 0.0f);
	circuitbody_list.add(segmentbody19);

	//20thSegment
	Cube segment20(9, 1, 83);
	segment20.SetPos(0, 3, -56);
	segment20.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment20);
	PhysBody3D* segmentbody20 = App->physics->AddBody(segment20, 0.0f);
	circuitbody_list.add(segmentbody20);

	//21thSegment
	Cube segment21(5, 1, 9);
	segment21.SetPos(37, 37.92f, -34);
	segment21.SetRotation(-12, { 0, 0, 1 });
	segment21.color.Set(0.5f, 0, 0);
	circuitcube_list.add(segment21);
	PhysBody3D* segmentbody21 = App->physics->AddBody(segment21, 0.0f);
	circuitbody_list.add(segmentbody21);
}

