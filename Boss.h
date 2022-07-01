#pragma once
#include "Unit.h"
#include "Timer.h"


class Boss :
	public Unit
{
public:
	Boss(UnitType type, int hp, float fire_delay, int speed, int damage);
	~Boss();

	void update(float dt);
	virtual void update_attack(float dt) {}
	void draw_particle(UnitType type);

	int pattern;
	Timer attackTimer;
	Timer explodeTimer;
};


class Shark :
	public Boss
{
public:
	Shark();
	~Shark();

	void update_attack(float dt);

	float particleTimer;
	int attack_count;
};


class Angler :
	public Boss
{
public:
	Angler();
	~Angler();

	void update_attack(float dt);

	int attack_count;
};


struct Circle
{
	Vec2 center;
	float range;
	float distance;
	float angle;
};

class Crab :
	public Boss
{
public:
	Crab();
	~Crab();

	void update_attack(float dt);

	Animation* left_arm;
	Animation* right_arm;
	int attack_count;
	int arm_dir;
	bool arm_back;
	Circle left;
	Circle right;
};


class Pagurian : 
	public Boss
{
public:
	Pagurian();
	~Pagurian();

	void update_attack(float dt);

	Animation* left_arm;
	Animation* right_arm;
	Sprite* range;
	int attack_count;
	int arm_dir;
	bool arm_back;
	bool is_shoot_left;
	int no_shoot_num;
	Circle left;
	Circle right;
};

