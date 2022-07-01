#pragma once
#include "Unit.h"
#include "Timer.h"

class Player :
	public Unit
{
public:
	Player();
	~Player();

	void update(float dt);
	void hit(int damage);

	Animation* die;
	Sprite* shield;

	bool is_shield_fade_in;
	bool is_shield;
	bool is_invincible;
	bool is_fade_in;
	bool is_die_end;
	int torpedo_count;
	float invincibleTimer;
	float shield_max_alpha;
	float bubbleTimer;
	Timer explodeTimer;
};