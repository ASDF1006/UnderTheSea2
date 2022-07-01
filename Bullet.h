#pragma once
#include "Entity.h"
#include "Sprite.h"
#include "Unit.h"

class Bullet :
	public Entity
{
public:
	Bullet(Team team, BulletType type, int damage);
	~Bullet();

	void update(float dt);

	Team team;
	BulletType type;
	Sprite* sprite;
	int y_speed;
	int speed;
	int damage;
	float gravity;
	float angleWithTarget;
	float homingTimer;
	float particleTimer;
	bool is_barrage;
	bool is_spawn_bullet;
	bool is_homing;
	Unit* target;
};