#pragma once
#include "Entity.h"
#include "Animation.h"


class Unit :
	public Entity
{
public:
	Unit(Team team, UnitType type, int hp, float fire_delay, int speed, int damage);
	~Unit();

	void update(float dt);
	
	Team team;
	UnitType type;
	int hp, hp_origin;
	float fire_delay, fire_delay_origin;
	int speed;
	int damage;

	Animation* move;
	Animation* attack;
	Rect hitbox;

	bool is_die;
	bool red_filter;
};

