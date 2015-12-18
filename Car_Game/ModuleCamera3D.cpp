#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	Move({ 0, 5, -100 });

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	//Default camera movement
	if (debug)
	{
		vec3 newPos(0, 0, 0);
		float speed = 10.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 20.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		Position += newPos;
		Reference += newPos;

		// Mouse motion ----------------
		//Default Camera rotation
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
	}

	
	//Camera position respect the player
	//WOW IT WORKS
	//OMG IT'S FUCKING AMAZING, THIS CAM FOR GOTY GAMES!
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
			first_person = !first_person;

		if (!first_person)
		{

			btTransform transform = App->player->vehicle->GetBody()->getWorldTransform();
			btVector3 vehicle_pos = (transform.getOrigin());

			Reference = { vehicle_pos.getX(), vehicle_pos.getY(), vehicle_pos.getZ() };
			LookAt(Reference);

			btQuaternion* quat = &(transform.getRotation());
			float angle = quat->getAngle();

			btVector3 a = quat->getAxis();
			vec3 axis = { a.getX(), a.getY(), a.getZ() };

			if (quat != q)
			{
				X = rotate(X, angle, axis);
				Y = rotate(Y, angle, axis);
				Z = rotate(Z, angle, axis);
				q = quat;
			}

			vec3 c_pos = Reference + Z * CAMERA_DISTANCE;

			if (c_pos.y < Reference.y + CAMERA_HEIGHT)
			{
				c_pos.y = Reference.y + CAMERA_HEIGHT;
			}

			Position = c_pos;

			/*
			// SHAME ON YOU, DAMMIT CAM FAILURES
			*/

			//------

			//Standard reference
			/*btTransform transform = App->player->vehicle->GetBody()->getWorldTransform();
			btVector3 vehicle_pos = (transform.getOrigin());

			vec3 camera_ref = { vehicle_pos.getX(), vehicle_pos.getY(), vehicle_pos.getZ() };
			App->camera->LookAt(camera_ref);

			//Lot's of wrong things

			vec3 camera_pos(0, 5, -13);

			App->camera->Position = camera_pos + camera_ref;*/

			//------------
			//Using car Z basis
			/*It also doesn't work using the Z of the vehicle basiss
			btVector3 vehicle_Z = (transform.getBasis().getColumn(2));

			btVector3 c_pos = vehicle_pos + vehicle_Z * -10;
			vec3 camera_pos = { c_pos.getX(), c_pos.getY(), c_pos.getZ() };
			//Camera height
			camera_pos.y += 5;
			App->camera->Position = camera_pos;

			//---------
			//Rotation alying a quaternion over the camera point
			//I can't do it to rotate with the car, ask ric how to do it
			/*c_pos = quatRotate(rotation, c_pos);
			vec3 camera_pos = { c_pos.getX(), c_pos.getX(), c_pos.getX() };
			App->camera->Position = camera_pos;*/

			//Rotation with QWERTY
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{

				float Sensitivity = 1.0f;
				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
					Sensitivity = -1.0f;

				Position -= Reference;

				float DeltaX = Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));

				Position = Reference + Z * length(Position);

				
			}
		}

		/*if (first_person)
		{
			btTransform transform = App->player->vehicle->GetBody()->getWorldTransform();
			btVector3 vehicle_pos = (transform.getOrigin());

			vec3 camera_ref = { vehicle_pos.getX(), vehicle_pos.getY(), vehicle_pos.getZ() };
			

			btVector3 vehicle_Z = (transform.getBasis().getColumn(2));
			btVector3 vehicle_X = (transform.getBasis().getColumn(0));
			btVector3 vehicle_Y = (transform.getBasis().getColumn(1));

			Z = { -vehicle_Z.getX(), -vehicle_Z.getY(), -vehicle_Z.getZ() };
			X = { vehicle_X.getX(), vehicle_X.getY(), vehicle_X.getZ() };
			Y = { vehicle_Y.getX(), vehicle_Y.getY(), vehicle_Y.getZ() };


			Position = Position = camera_ref + Z* -2;

			//ADD something for when it changes to third person from first person, because it's not smooth at all
		}*/
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}