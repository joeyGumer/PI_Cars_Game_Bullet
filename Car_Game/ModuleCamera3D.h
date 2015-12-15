#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#define CAMERA_DISTANCE 20
#define CAMERA_HEIGHT 10

class btQuaternion;
class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	bool debug = false;
	btQuaternion* q;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};