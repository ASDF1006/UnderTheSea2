#pragma once
#include "Sprite.h"

class Particle :
	public Sprite
{
public:
	Particle(string path, ParticleType type, float fade_speed = 3, float max_alpha = 1.f, bool is_fade_in = false);
	Particle(Texture* t, ParticleType type, float fade_speed = 3, float max_alpha = 1.f, bool is_fade_in = false);
	virtual ~Particle();

	void update(float dt);

	ParticleType type;
	float fade_speed;
	float max_alpha;
	float move_angle;
	bool is_fade_in;
	bool is_move;
	int move_speed;
};