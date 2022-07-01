#include "DXUT.h"
#include "Particle.h"
#include "GameManager.h"


Particle::Particle(string path, ParticleType type, float fade_speed, float max_alpha, bool is_fade_in)
	: Sprite(path), type(type), fade_speed(fade_speed), max_alpha(max_alpha), is_fade_in(is_fade_in),
	is_move(false), move_angle(0), move_speed(300)
{
	if (is_fade_in)
		color.a = 0;
	rotationCenter = center();
}


Particle::Particle(Texture* t, ParticleType type, float fade_speed, float max_alpha, bool is_fade_in)
	: type(type), fade_speed(fade_speed), max_alpha(max_alpha), is_fade_in(is_fade_in),
	is_move(false), move_angle(0), move_speed(300)
{
	texture = t;
	visibleRect = rect = Rect(0, 0, texture->info.Width, texture->info.Height);

	if (is_fade_in)
		color.a = 0;
	rotationCenter = center();
}


Particle::~Particle()
{
}


void Particle::update(float dt)
{
	Sprite::update(dt);

	if (type == FADE)
	{
		if (is_fade_in)
		{
			color.a += fade_speed * dt;
			if (color.a > max_alpha)
				is_fade_in = false;
		}
		else
		{
			color.a -= fade_speed * dt;
			if (color.a < 0)
				deleting = true;
		}
		if (is_move)
		{
			pos += Vec2(cos(move_angle), sin(move_angle)) * move_speed * dt;
		}
	}
	else if (type == XP)
	{
		if (is_move)
		{
			pos += Vec2(cos(move_angle), sin(move_angle)) * move_speed * dt;
			move_speed += 120 * dt;
			if (move_speed > 340)
			{
				is_move = false;
				move_speed = 1000;
			}
		}
		else
		{
			move_angle = angle(center(), gm.ingame->player->center());
			pos += Vec2(cos(move_angle), sin(move_angle)) * move_speed * dt;
			if (gm.ingame->player->hitbox.offset(gm.ingame->player->pos).contain(center()) && !deleting) {
				gm.score += 10 + gm.combo / 10.f;
				deleting = true;
			}
		}
	}
	else if (type == TORNADE)
	{
		rotation += 20 * dt;
		if (rotation > D3DXToRadian(270))
		{
			for (int i = 0; i < 10; i++)
			{
				Particle* p = new Particle("image/Effect/xp.png", XP);
				gm.ingame->addChild(p);
				gm.ingame->particleList.push_back(p);
				p->pos = center();
				p->move_angle = random(i * 36, 36 + i * 36);
				p->is_move = true;
			}

			deleting = true;
		}
	}
	else if (type == COMBO)
	{
		if (scale.x > 1)
		{
			scale -= Vec2(1, 1) * dt;
		}
		else if(color.a > 0)
		{
			color.a -= 3 * dt;
			if (color.a <= 0)
				deleting = true;
		}

		pos.x += cos(move_angle) * 100 * dt;
	}
}