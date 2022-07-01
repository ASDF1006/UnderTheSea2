#pragma once
#include "Unit.h"

class Enemy :
	public Unit
{
public:
	Enemy(UnitType type, int hp, float fire_delay, int speed, int damage);
	~Enemy();

	void update(float dt);
	virtual void update_attack(float dt) {}
	void draw_particle();

	bool in_screen;
	bool is_attack;
};


class Puffer : 
	public Enemy
{
public:
	Puffer();
	~Puffer();

	void update_attack(float dt);
};


class Carp :
	public Enemy
{
public:
	Carp();
	~Carp();

	void update_attack(float dt);
};


class Marlin :
	public Enemy
{
public:
	Marlin();
	~Marlin();

	void update_attack(float dt);

	float angleWithTarget;
	float particleTimer;
};


class Octopus :
	public Enemy
{
public:
	Octopus();
	~Octopus();

	void update_attack(float dt);
};


class Rock :
	public Enemy
{
public:
	Rock();
	~Rock();

	void update_attack(float dt);

	float afterTimer;
};