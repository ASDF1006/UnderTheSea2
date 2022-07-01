#pragma once
#include "Unit.h"

class Addon :
	public Unit
{
public:
	Addon();
	~Addon();

	void update(float dt);

	float move_angle;
	Entity* followTarget;
	Vec2 offset;
	float last_x;
};