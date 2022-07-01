#include "DXUT.h"
#include "Enemy.h"
#include "GameManager.h"
#include "Bullet.h"
#include "Particle.h"



Enemy::Enemy(UnitType type, int hp, float fire_delay, int speed, int damage)
	: Unit(ENEMY, type, gm.stage * 20 + hp, fire_delay, speed, damage), is_attack(false), in_screen(false)
{
}


Enemy::~Enemy()
{
}


void Enemy::update(float dt)
{
	if (!in_screen)
	{
		if (-gm.ingame->pos.y < pos.y + rect.height())
		{
			in_screen = true;
			gm.ingame->unitList.push_back(this);
		}
	}
	else
	{
		if (hp <= 0 && !is_die)
		{
			is_die = true;
		}

		update_attack(dt);

		if (is_die)
		{
			if (type != ROCK) {
				draw_particle();
				if (random(0, 2))
				{
					if (random(0, 3))
					{
						Item* i = new Item(I_COIN);
						gm.ingame->addChild(i);
						gm.ingame->itemList.push_back(i);
						i->setCenter(center());
					}
					else
					{
						Item* i = new Item((ItemType)random(0, 3));
						gm.ingame->addChild(i);
						gm.ingame->itemList.push_back(i);
						i->setCenter(center());
					}
				}
			}
			deleting = true;
		}

		if (pos.y + gm.ingame->pos.y > 720 || pos.x + rect.height() < 0 || pos.x > 1280)
			deleting = true;

		if (red_filter)
		{
			attack->color.g = attack->color.b = move->color.g = move->color.b += dt;
			if (move->color.g > 1)
				red_filter = false;
		}
	}
}


Puffer::Puffer() : Enemy(PUFFER, 60, 1, 50, 10)
{
	move = new Animation("image/Unit/Enemy/Puffer/" + to_string(gm.stage) + "/move/", 21, 20);
	attack = new Animation("image/Unit/Enemy/Puffer/" + to_string(gm.stage) + "/attack/", 11, 15);
	addChild(move);
	addChild(attack);

	attack->visible = false;
	hitbox = rect = move->rect;
}


Puffer::~Puffer()
{
}

void Puffer::update_attack(float dt)
{
	if (fire_delay > fire_delay_origin)
	{
		if (attack->currentFrame == 0)
		{
			move->visible = false;
			attack->visible = true;
			is_attack = true;
		}

		attack->update(dt);

		if ((int)attack->currentFrame == 5)
		{
			for (int i = 0; i < 8; i++)
			{
				Bullet* b = new Bullet(team, B_PUFFER, damage);
				gm.ingame->addChild(b);
				gm.ingame->bulletList.push_back(b);
				b->setCenter(center());
				b->angleWithTarget = D3DXToRadian(45 * i);
			}
			attack->currentFrame++;
		}
		if (attack->is_end)
		{
			fire_delay = 0;
			attack->is_end = false;
			attack->currentFrame = 0;
			move->visible = true;
			attack->visible = false;
			is_attack = false;
		}
	}
	else
	{
		pos.y += speed * dt;
		move->update(dt);

		if (move->is_end)
		{
			move->is_end = false;
			move->currentFrame = 0;
			fire_delay++;
		}
	}
}


Carp::Carp() : Enemy(CARP, 50, 3, 100, 10)
{
	move = new Animation("image/Unit/Enemy/Carp/" + to_string(gm.stage) + "/move/", 11, 20);
	attack = new Animation("image/Unit/Enemy/Carp/" + to_string(gm.stage) + "/attack/", 11, 15);
	addChild(move);
	addChild(attack);

	attack->visible = false;
	hitbox = rect = move->rect;
}


Carp::~Carp()
{
}


void Carp::update_attack(float dt)
{
	if (fire_delay > fire_delay_origin)
	{
		if (attack->currentFrame == 0)
		{
			move->visible = false;
			attack->visible = true;
		}

		attack->update(dt);

		if ((int)attack->currentFrame == 5)
		{
			Bullet* b = new Bullet(team, B_CARP, damage);
			gm.ingame->addChild(b);
			gm.ingame->bulletList.push_back(b);
			b->setCenter(center());
			b->angleWithTarget = angle(center(), gm.ingame->player->center());
			attack->currentFrame++;
		}
		else if (attack->is_end)
		{
			fire_delay = 0;
			attack->is_end = false;
			attack->currentFrame = 0;
			move->visible = true;
			attack->visible = false;
		}
	}
	else
	{
		pos.y += speed * dt;
		move->update(dt);

		if (move->is_end)
		{
			move->is_end = false;
			move->currentFrame = 0;
			fire_delay++;
		}
	}
}



Marlin::Marlin() : Enemy(MARLIN, 10, 5, 100, 10), angleWithTarget(0), particleTimer(0)
{
	move = new Animation("image/Unit/Enemy/Marlin/" + to_string(gm.stage) + "/move/", 11, 20, 1);
	attack = new Animation("image/Unit/Enemy/Marlin/" + to_string(gm.stage) + "/move/", 11, 30, 1);
	addChild(move);
	addChild(attack);

	attack->visible = false;

	hitbox = rect = move->rect;
	rotationCenter = rect.center();
}

Marlin::~Marlin()
{
}


void Marlin::update_attack(float dt)
{
	if(fire_delay > fire_delay_origin)
	{
		move->visible = false;
		attack->visible = true;
		is_attack = true;
		fire_delay = 0;
	}
	else if(!is_attack)
	{
		fire_delay += dt;
		angleWithTarget = angle(Vec2(center().x, pos.y + rect.height()), gm.ingame->player->center());
		rotation = angleWithTarget - 1.57;

		pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;

		move->update(dt);
	}	
	else
	{
		pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * 5 * speed * dt;

		attack->update(dt);

		particleTimer += dt;

		if (particleTimer > 0.1)
		{
			Particle* p = new Particle(move->texture, FADE);
			gm.ingame->addChild(p);
			gm.ingame->afterimageList.push_back(p);
			p->pos = pos;
			p->rotationCenter = rotationCenter;
			p->color.a = 0.6;
			p->rotation = rotation;
			particleTimer = 0;
		}
	}
}


Octopus::Octopus() : Enemy(OCTOPUS, 50, 4, 100, 10)
{
	move = new Animation("image/Unit/Enemy/Octopus/" + to_string(gm.stage) + "/move/", 11, 20);
	attack = new Animation("image/Unit/Enemy/Octopus/" + to_string(gm.stage) + "/attack/", 11, 15);
	addChild(move);
	addChild(attack);

	attack->visible = false;
	hitbox = rect = move->rect;
}

Octopus::~Octopus()
{
}


void Octopus::update_attack(float dt)
{
	if (fire_delay > fire_delay_origin)
	{
		if (attack->currentFrame == 0)
		{
			move->visible = false;
			attack->visible = true;
		}

		attack->update(dt);

		if ((int)attack->currentFrame == 5)
		{
			Bullet* b = new Bullet(team, B_OCTOPUS, damage);
			gm.ingame->addChild(b);
			gm.ingame->bulletList.push_back(b);
			b->setCenter(center());
			b->angleWithTarget = angle(center(), gm.ingame->player->center());
			attack->currentFrame++;
		}
		else if (attack->is_end)
		{
			fire_delay = 0;
			attack->is_end = false;
			attack->currentFrame = 0;
			move->visible = true;
			attack->visible = false;
		}
	}
	else
	{
		pos.y += speed * dt;
		move->update(dt);

		if (move->is_end)
		{
			move->is_end = false;
			move->currentFrame = 0;
			fire_delay++;
		}
	}
}


Rock::Rock() : Enemy(ROCK, 0, 0, 600, 0), afterTimer(0)
{
	move = new Animation("image/Unit/Enemy/Rock/", 1, 1, 0, 1);

	addChild(move);

	hitbox = rect = move->rect;
	hitbox.left += 30;
	hitbox.right -= 30;
	hitbox.bottom -= 30;
}


Rock::~Rock()
{
}


void Rock::update_attack(float dt)
{
	if (is_die)
	{
		for (int i = 0; i < 10; i++)
		{
			Particle* p = new Particle("image/Unit/Enemy/Rock/0.png", FADE);
			gm.ingame->addChild(p);
			gm.ingame->particleList.push_back(p);
			p->pos = pos;
			p->scale = Vec2(1, 1) * random(0.1f, 0.4f);
			p->is_move = true;
			p->move_speed = 300;
			p->move_angle = random(0.f, 6.28f);
		}
		deleting = true;
	}
	else
	{
		pos.y += speed * dt;

		afterTimer += dt;

		if (afterTimer > 0.1)
		{
			Particle* p = new Particle("image/Unit/Enemy/Rock/0.png", FADE, 3.5);
			gm.ingame->addChild(p);
			gm.ingame->particleList.push_back(p);
			p->pos = pos;
			p->color.a = 0.6;
			afterTimer = 0;
		}
	}
}


void Enemy::draw_particle()
{
	Particle* s;
	switch (type)
	{
	case CARP:
		s = new Particle("image/Unit/Enemy/Carp/" + to_string(gm.stage) + "/move/0.dds", FADE);
		break;
	case PUFFER:
		s = new Particle("image/Unit/Enemy/Puffer/" + to_string(gm.stage) + "/move/0.dds", FADE);
		break;
	case MARLIN:
		s = new Particle("image/Unit/Enemy/Marlin/" + to_string(gm.stage) + "/move/0.dds", FADE);
		break;
	case OCTOPUS:
		s = new Particle("image/Unit/Enemy/Octopus/" + to_string(gm.stage) + "/move/0.dds", FADE);
		break;
	}
	gm.ingame->addChild(s);
	gm.ingame->particleList.push_back(s);
	s->pos = pos;

	Animation* a = new Animation("image/Effect/die/", 15, 20, 0, 1);
	gm.ingame->addChild(a);
	gm.ingame->effectList.push_back(a);
	a->setCenter(center());

	for (int i = 0; i < 10; i++)
	{
		Particle* p = new Particle("image/Effect/xp.png", XP);
		gm.ingame->addChild(p);
		gm.ingame->particleList.push_back(p);
		p->pos = center();
		p->move_angle = random(i * 36, 36 + i * 36);
		p->is_move = true;
	}
}