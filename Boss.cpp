#include "DXUT.h"
#include "Boss.h"
#include "Bullet.h"
#include "GameManager.h"
#include "Enemy.h"
#include "World.h"



Boss::Boss(UnitType type, int hp, float fire_delay, int speed, int damage)
	: Unit(ENEMY, type, hp, fire_delay, speed, damage), pattern(1)
{
}

Boss::~Boss()
{
}

void Boss::update(float dt)
{
	if (world.getKeyState('3') == 1)
		hp = 10;

	if (!is_die)
	{
		if (hp <= 0) {
			is_die = true;
			visible = false;
			gm.ingame->ui->boss_hp_frame->visible = gm.ingame->ui->boss_hp_bar->visible = false;
			gm.ingame->follow(gm.ingame->player, Vec2(640, 580) - gm.ingame->player->rect.center());
			draw_particle(type);

			explodeTimer.reset(0.1, 20);
			explodeTimer.onTick = [=]()
			{
				for (int i = 0; i < 3; i++)
				{
					Animation* a = new Animation("image/Effect/die/", 15, 20, 0, 1);
					gm.ingame->addChild(a);
					gm.ingame->effectList.push_back(a);
					a->setCenter(pos + Vec2(random(hitbox.left, hitbox.right), random(hitbox.top, hitbox.bottom)));

					for (int i = 0; i < 10; i++)
					{
						Particle* p = new Particle("image/Effect/xp.png", XP);
						gm.ingame->addChild(p);
						gm.ingame->particleList.push_back(p);
						p->pos = a->center();
						p->move_angle = random(i * 36, 36 + i * 36);
						p->is_move = true;
					}
				}
			};
			explodeTimer.onFinished = [=]()
			{
				deleting = true;
				if (type == ANGLER || type == PAGURIAN)
				{
					gm.ingame->stage_clear = true;
				}
			};
			for (int i = 0; i < 2; i++)
			{
				Item* s = new Item((ItemType)random(0, 3));
				gm.ingame->addChild(s);
				gm.ingame->itemList.push_back(s);
				s->setCenter(center() + Vec2(60 * i - 30, 0));
			}
			for (int i = -1; i < 2; i++)
			{
				Item* s = new Item(I_COIN);
				gm.ingame->addChild(s);
				gm.ingame->itemList.push_back(s);
				s->setCenter(center() + Vec2(60 * i, - 60));
			}
			gm.currentBoss = nullptr;
		}
		else
		{
			update_attack(dt);
			if (red_filter)
			{
				attack->color.g = attack->color.b = move->color.g = move->color.b += dt;
				if (move->color.g > 1)
					red_filter = false;
			}
		}
	}
}


Shark::Shark() : Boss(SHARK, 800, 3, 400, 15), attack_count(0), particleTimer(0)
{
	move = new Animation("image/Unit/Boss/Shark/move/", 21, 20);
	attack = new Animation("image/Unit/Boss/Shark/attack/", 11, 20);

	addChild(move);
	addChild(attack);

	attack->visible = false;
	hitbox = rect = move->rect;
	hitbox.left += 90;
	hitbox.right -= 90;
	hitbox.bottom -= 120;
	pattern = 1;
}

Shark::~Shark()
{
}

void Shark::update_attack(float dt)
{

	if (fire_delay > fire_delay_origin)
	{
		if (pattern == 1)
		{
			if (attack->currentFrame == 0)
			{
				move->visible = false;
				attack->visible = true;
			}

			attack->update(dt);

			if ((int)attack->currentFrame == 5)
			{
				for (int i = 0; i < 12; i++)
				{
					Bullet* b = new Bullet(team, B_SHARK, damage);
					gm.ingame->addChild(b);
					gm.ingame->bulletList.push_back(b);
					b->pos = Vec2(center().x, pos.y + rect.height());
					b->angleWithTarget = D3DXToRadian(i * 30 + attack_count * 10);
					b->speed = 400;
				}
				attack->currentFrame++;
			}

			else if (attack->is_end)
			{
				attack->is_end = false;
				attack->currentFrame = 0;
				attack_count++;
			}

			if (attack_count > 4)
			{
				move->visible = true;
				attack->visible = false;
				fire_delay = 0;
				attack_count = 0;
				pattern++;
			}
		}

		else if (pattern == 2)
		{
			move->fps = 40;
			move->update(dt);

			if (move->is_end)
			{
				move->is_end = false;
				move->currentFrame = 0;
			}

			particleTimer += dt;

			if (particleTimer > 0.1)
			{
				Particle* p = new Particle(move->texture, FADE);
				gm.ingame->addChild(p);
				gm.ingame->afterimageList.push_back(p);
				p->pos = pos;
				p->color.a = 0.6;
				particleTimer = 0;
			}

			pos.y += 800 * dt;

			if (pos.y > 720 - gm.ingame->pos.y)
			{
				move->fps = 20;
				pos.y = -gm.ingame->pos.y - rect.height();
				pattern = 1;
				fire_delay = 0;
			}
		}
	}
	else
	{
		if (pos.y < 250 - gm.ingame->pos.y - rect.height())
			pos.y += 400 * dt;
		if (center().x < gm.ingame->player->center().x - 10)
			pos.x += 200 * dt;
		else if(center().x > gm.ingame->player->center().x + 10)
			pos.x -= 200 * dt;

		move->update(dt);

		if (move->is_end)
		{
			move->is_end = false;
			move->currentFrame = 0;
			fire_delay++;
		}
	}
}


Angler::Angler() : Boss(ANGLER, 1600, 2, 400, 15), attack_count(0)
{
	move = new Animation("image/Unit/Boss/Angler/move/", 21, 20);
	attack = new Animation("image/Unit/Boss/Angler/attack/", 11, 20);

	addChild(move);
	addChild(attack);

	attack->visible = false;
	hitbox = rect = move->rect;

	hitbox.left += 120;
	hitbox.right -= 120;
	hitbox.bottom -= 50;
}

Angler::~Angler()
{
}

void Angler::update_attack(float dt)
{
	if (fire_delay > fire_delay_origin)
	{
		if (attack->currentFrame == 0)
		{
			move->visible = false;
			attack->visible = true;
			if (pattern == 2)
				attack->fps = 30;

			else if (pattern == 3)
				attack->fps = 10;
		}

		attack->update(dt);

		if ((int)attack->currentFrame == 6)
		{
			if (pattern == 1)
			{
				Bullet* b = new Bullet(team, B_HOMING, damage * 2);
				gm.ingame->addChild(b);
				gm.ingame->bulletList.push_back(b);
				b->pos = Vec2(center().x, center().y + 50);
				b->speed = 200;
				b->angleWithTarget = D3DXToRadian(90);
			}
			else if (pattern == 2)
			{
				for (int i = 0; i < 5; i++)
				{
					Bullet* b = new Bullet(team, B_RAIN, damage * 2);
					gm.ingame->addChild(b);
					gm.ingame->bulletList.push_back(b);
					b->pos = Vec2(center().x, center().y + 50);
					b->angleWithTarget = D3DXToRadian(random(220, 320));
					b->speed = 300;
					b->y_speed = sin(b->angleWithTarget) * 500;
				}
			}
			else if (pattern == 3)
			{
				for (int i = 0; i < 5; i++)
				{
					Bullet* b = new Bullet(team, B_BARRAGE, damage * 2);
					gm.ingame->addChild(b);
					gm.ingame->bulletList.push_back(b);
					b->pos = Vec2(center().x, center().y + 50);
					b->angleWithTarget = D3DXToRadian(random(220, 320));
					b->speed = 300;
					b->y_speed = sin(b->angleWithTarget) * 500;
				}
			}

			attack->currentFrame++;
		}

		else if (attack->is_end)
		{
			attack->is_end = false;
			attack->currentFrame = 0;
			if (pattern == 1)
			{
				move->visible = true;
				attack->visible = false;
				fire_delay = 0;
				pattern++;
			}
			else attack_count++;
		}

		if (pattern == 2)
		{
			if (attack_count > 4)
			{
				move->visible = true;
				attack->visible = false;
				attack->fps = 20;
				fire_delay = 0;
				attack_count = 0;
				pattern++;
			}
		}
		else if (pattern == 3)
		{
			if (attack_count > 3)
			{
				move->visible = true;
				attack->visible = false;
				attack->fps = 20;
				fire_delay = 0;
				attack_count = 0;
				pattern = 1;
			}
		}
	}
	else
	{
		if (pos.y < 300 - gm.ingame->pos.y - rect.height())
			pos.y += 300 * dt;

		move->update(dt);

		if (move->is_end)
		{
			move->is_end = false;
			move->currentFrame = 0;
			fire_delay++;
		}
	}
}



Crab::Crab() : Boss(CRAB, 1000, 3, 400, 15), arm_back(false), arm_dir(0), attack_count(0)
{
	move = new Animation("image/Unit/Boss/Crab/move/", 21, 20);
	attack = new Animation("image/Unit/Boss/Crab/attack/", 11, 20);
	left_arm = new Animation("image/Unit/Boss/Crab/arm/left/", 11, 15, 1);
	right_arm = new Animation("image/Unit/Boss/Crab/arm/right/", 11, 15, 1);

	addChild(left_arm);
	addChild(right_arm);
	addChild(move);
	addChild(attack);

	left_arm->rotationCenter = Vec2(450, 150);
	right_arm->rotationCenter = Vec2(350, 150);
	left_arm->pos = Vec2(-400, 100);
	right_arm->pos = Vec2(200, 100);

	attack->visible = false;
	hitbox = rect = move->rect;
	hitbox.left += 50;
	hitbox.right -= 50;
	hitbox.bottom -= 100;

	left.range = 150;
	right.range = 150;
	left.angle = D3DXToRadian(120);
	right.angle = D3DXToRadian(60);
}

Crab::~Crab()
{
}

void Crab::update_attack(float dt)
{
	if (fire_delay > fire_delay_origin)
	{
		if (pattern == 1)
		{
			if (attack->currentFrame == 0)
			{
				move->visible = false;
				attack->visible = true;
			}

			attack->update(dt);
			
			if (attack_count > 1 && arm_dir == 0)
			{
				if (!arm_back)
				{
					left_arm->rotation -= 4 * dt;
					left.angle -= 4 * dt;

					if (left_arm->rotation < D3DXToRadian(-60))
					{
						arm_back = true;
					}
				}
				else
				{
					left_arm->rotation += 4 * dt;
					left.angle += 4 * dt;

					if (left_arm->rotation > 0)
					{
						arm_dir = random(0, 1);
						arm_back = false;
						attack->is_end = false;
						attack->currentFrame = 0;
						move->visible = true;
						attack->visible = false;
						attack_count = 0;
						fire_delay = 0;
						pattern++;
					}
				}
			}
			else if (attack_count > 1 && arm_dir == 1)
			{
				if (!arm_back)
				{
					right_arm->rotation += 4 * dt;
					right.angle += 4 * dt;

					if (right_arm->rotation > D3DXToRadian(60))
					{
						arm_back = true;
					}
				}
				else
				{
					right_arm->rotation -= 4 * dt;
					right.angle -= 4 * dt;

					if (right_arm->rotation < 0)
					{
						arm_dir = random(0, 1);
						arm_back = false;
						attack->is_end = false;
						attack->currentFrame = 0;
						move->visible = true;
						attack->visible = false;
						attack_count = 0;
						fire_delay = 0;
						pattern++;
					}
				}
			}

			if (attack_count == 0 && (int)attack->currentFrame == 6)
			{
				Particle* p = new Particle("image/UI/warning_point.png", FADE, 3, 1, true);
				gm.ingame->addChild(p);
				gm.ingame->particleList.push_back(p);
				p->setCenter(Vec2(arm_dir * 300 + 150, 700) + pos);
			}
			else if (attack_count <= 1 && attack->is_end)
			{
				attack->is_end = false;
				attack->currentFrame = 0;
				attack_count++;
			}
		}

		else if (pattern == 2)
		{
			if (attack->currentFrame == 0)
			{
				move->visible = false;
				attack->visible = true;
			}

			attack->update(dt);


			if ((int)attack->currentFrame == 6)
			{
				gm.ingame->shake(0.05, 70, 8);
				attackTimer.reset(0.2, 20);
				attackTimer.onTick = [=]()
				{
					Rock* e = new Rock();
					gm.ingame->addChild(e);
					gm.ingame->unitList.push_back(e);
					e->pos = Vec2(random(0, 1200), -gm.ingame->pos.y);
				};
				attack->currentFrame++;
			}

			else if (attack->is_end)
			{
				attack->is_end = false;
				attack->currentFrame = 0;
				move->visible = true;
				attack->visible = false;
				fire_delay = 0;
				pattern++;
			}
		}

		else if (pattern == 3)
		{
			if (attack->currentFrame == 0)
			{
				move->visible = false;
				attack->visible = true;
			}

			attack->update(dt);

			if ((int)attack->currentFrame == 6)
			{
				attackTimer.reset(0.05, 50);
				attackTimer.onTick = [=]() {
					Bullet* b = new Bullet(team, B_BUBBLE, damage);
					gm.ingame->addChild(b);
					gm.ingame->bulletList.push_back(b);
					b->setCenter(Vec2(center().x, pos.y + rect.height()));
					b->angleWithTarget = D3DXToRadian(random(40, 140));
					b->speed = random(200, 300);
				};
			}
			else if (attack->is_end)
			{
				attack->is_end = false;
				attack->currentFrame = 0;
				move->visible = true;
				attack->visible = false;
				fire_delay = 0;
				pattern = 1;
			}
		}
	}

	else
	{
		if (pos.y < 200 - gm.ingame->pos.y - rect.height())
			pos.y += 300 * dt;

		move->update(dt);

		if (move->is_end)
		{
			move->is_end = false;
			move->currentFrame = 0;
			fire_delay++;
		}
	}

	left_arm->update(dt);
	right_arm->update(dt);

	left.center = pos + Vec2(cos(left.angle), sin(left.angle)) * 480 + Vec2(50, 250);
	right.center = pos + Vec2(cos(right.angle), sin(right.angle)) * 480 + Vec2(550, 250);
	
	left.distance = distance(left.center, gm.ingame->player->center()) - left.range - 50;
	right.distance = distance(right.center, gm.ingame->player->center()) - right.range - 50;


	if (left.distance < 0 || right.distance < 0)
	{
		if (!gm.ingame->player->is_invincible)
		{
			if (gm.ingame->player->is_shield) {
				gm.ingame->player->is_shield = false;
				gm.ingame->player->shield->visible = false;
			}
			else {
				gm.ingame->player->hp -= 10;
			}
			gm.ingame->shake(0.03, 20, 10);
			gm.ingame->player->is_invincible = true;
			Particle* p = new Particle("image/UI/hit.png", FADE, 4, 1, true);
			gm.ingame->ui->addChild(p);
			gm.ingame->ui->fadeList.push_back(p);
		}
	}

	if (hp <= 0)
	{
		right_arm->visible = false;
		left_arm->visible = false;
	}

	if (red_filter)
	{
		right_arm->color = left_arm->color = move->color;
	}
}


Pagurian::Pagurian() 
	: Boss(PAGURIAN, 2000, 3, 2000, 20), arm_back(false), arm_dir(0), attack_count(0), 
	is_shoot_left(false), no_shoot_num(random(10, 50))
{
	move = new Animation("image/Unit/Boss/Pagurian/move/", 21, 20);
	attack = new Animation("image/Unit/Boss/Pagurian/attack/", 11, 20);
	left_arm = new Animation("image/Unit/Boss/Pagurian/arm/left/", 11, 20);
	right_arm = new Animation("image/Unit/Boss/Pagurian/arm/right/", 11, 20);
	range = new Sprite("image/Unit/Boss/Crab/range.png");

	addChild(left_arm);
	addChild(right_arm);
	addChild(move);
	addChild(attack);
	addChild(range);

	left_arm->rotationCenter = Vec2(450, 150);
	right_arm->rotationCenter = Vec2(350, 150);
	left_arm->pos = Vec2(-400, 300);
	right_arm->pos = Vec2(350, 300);


	attack->visible = false;
	hitbox = rect = move->rect;
	hitbox.left += 50;
	hitbox.right -= 50;
	hitbox.bottom -= 100;

	left.range = 150;
	right.range = 150;
	left.angle = D3DXToRadian(120);
	right.angle = D3DXToRadian(60);

	pattern = 3;
}


Pagurian::~Pagurian()
{
}


void Pagurian::update_attack(float dt)
{
	if (fire_delay > fire_delay_origin)
	{
		if (pattern == 1)
		{
			if (attack->currentFrame == 0)
			{
				move->visible = false;
				attack->visible = true;
			}

			attack->update(dt);


			if (arm_dir == 0)
			{
				if (!arm_back)
				{
					left_arm->rotation -= 4 * dt;
					left.angle -= 4 * dt;

					if (left_arm->rotation < D3DXToRadian(-60))
					{
						arm_back = true;
					}
				}
				else
				{
					left_arm->rotation += 4 * dt;
					left.angle += 4 * dt;

					if (left_arm->rotation > 0)
					{
						arm_dir = random(0, 1);
						arm_back = false;
						attack->is_end = false;
						attack->currentFrame = 0;
						move->visible = true;
						attack->visible = false;
						fire_delay = 0;
						pattern++;
					}
				}
			}
			else if (arm_dir == 1)
			{
				if (!arm_back)
				{
					right_arm->rotation += 4 * dt;
					right.angle += 4 * dt;

					if (right_arm->rotation > D3DXToRadian(60))
					{
						arm_back = true;
					}
				}
				else
				{
					right_arm->rotation -= 4 * dt;
					right.angle -= 4 * dt;

					if (right_arm->rotation < 0)
					{
						arm_dir = random(0, 1);
						arm_back = false;
						attack->is_end = false;
						attack->currentFrame = 0;
						move->visible = true;
						attack->visible = false;
						fire_delay = 0;
						pattern++;
					}
				}
			}
		}

		else if (pattern == 2)
		{
			if (attack->currentFrame == 0)
			{
				move->visible = false;
				attack->visible = true;
			}

			attack->update(dt);


			if ((int)attack->currentFrame == 6)
			{
				gm.ingame->shake(0.05, 70, 8);
				attackTimer.reset(0.2, 20);
				attackTimer.onTick = [=]()
				{
					Rock* e = new Rock();
					gm.ingame->addChild(e);
					gm.ingame->unitList.push_back(e);
					e->pos = Vec2(random(0, 1200), -gm.ingame->pos.y);
				};
				attack->currentFrame++;
			}

			else if (attack->is_end)
			{
				attack->is_end = false;
				attack->currentFrame = 0;
				move->visible = true;
				attack->visible = false;
				fire_delay = 0;
				pattern++;
			}
		}

		else if (pattern == 3)
		{
			if (attack->currentFrame == 0)
			{
				attack->fps = 13;
				move->visible = false;
				attack->visible = true;
			}

			attack->update(dt);

			if (attack_count == 0 && (int)attack->currentFrame == 6)
			{
				is_shoot_left = random(0, 1);

				attackTimer.reset(0.05, 60);
				attackTimer.onTick = [=]() {
					if (10 <= attackTimer.count.first - no_shoot_num || attackTimer.count.first - no_shoot_num < 0)
					{
						Bullet* b = new Bullet(team, B_WAVE, damage);
						gm.ingame->addChild(b);
						gm.ingame->bulletList.push_back(b);
						b->setCenter(Vec2(center().x, pos.y + rect.height()));
						if (is_shoot_left)
							b->angleWithTarget = D3DXToRadian(180 - attackTimer.count.first * 3);
						else
							b->angleWithTarget = D3DXToRadian(attackTimer.count.first * 3);
						b->speed = 700;
					}
				};
			}

			else if (attack->is_end)
			{
				attack->is_end = false;
				attack->currentFrame = 0;
				attack_count++;
			}

			if (attack_count > 3)
			{
				attack->fps = 20;
				move->visible = true;
				attack->visible = false;
				fire_delay = 0;
				attack_count = 0;
				pattern = 1;
			}
		}
	}

	else
	{
		if (pos.y < 200 - gm.ingame->pos.y - rect.height())
			pos.y += 300 * dt;

		move->update(dt);

		if (move->is_end)
		{
			move->is_end = false;
			move->currentFrame = 0;
			fire_delay++;
		}
	}

	left_arm->update(dt);
	right_arm->update(dt);

	left.center = pos + Vec2(cos(left.angle), sin(left.angle)) * 500 + Vec2(50, 450);
	right.center = pos + Vec2(cos(right.angle), sin(right.angle)) * 500 + Vec2(700, 450);

	left.distance = distance(left.center, gm.ingame->player->center()) - left.range - 50;
	right.distance = distance(right.center, gm.ingame->player->center()) - right.range - 50;


	if (left.distance < 0 || right.distance < 0)
	{
		if (!gm.ingame->player->is_invincible)
		{
			if (gm.ingame->player->is_shield) {
				gm.ingame->player->is_shield = false;
				gm.ingame->player->shield->visible = false;
			}
			else {
				gm.ingame->player->hp -= 10;
			}
			gm.ingame->shake(0.03, 20, 10);
			gm.ingame->player->is_invincible = true;
			Particle* p = new Particle("image/UI/hit.png", FADE, 4, 1, true);
			gm.ingame->ui->addChild(p);
			gm.ingame->ui->fadeList.push_back(p);
		}
	}

	if (hp <= 0)
	{
		right_arm->visible = false;
		left_arm->visible = false;
	}

	if (red_filter)
	{
		right_arm->color = left_arm->color = move->color;
	}
}


void Boss::draw_particle(UnitType type)
{
	if (type == CRAB)
	{
		Particle* l = new Particle("image/Unit/Boss/Crab/arm/left/0.dds", FADE, 1);
		gm.ingame->addChild(l);
		gm.ingame->afterimageList.push_back(l);
		l->pos = Vec2(-400, 100) + pos;

		Particle* r = new Particle("image/Unit/Boss/Crab/arm/right/0.dds", FADE, 1);
		gm.ingame->addChild(r);
		gm.ingame->afterimageList.push_back(r);
		r->pos = Vec2(200, 100) + pos;
	}
	else if (type == PAGURIAN)
	{
		Particle* l = new Particle("image/Unit/Boss/Pagurian/arm/left/0.dds", FADE, 1);
		gm.ingame->addChild(l);
		gm.ingame->afterimageList.push_back(l);
		l->pos = Vec2(-400, 300) + pos;

		Particle* r = new Particle("image/Unit/Boss/Pagurian/arm/right/0.dds", FADE, 1);
		gm.ingame->addChild(r);
		gm.ingame->afterimageList.push_back(r);
		r->pos = Vec2(350, 300) + pos;
	}

	Particle* p;
	switch (type)
	{
	case SHARK: p = new Particle("image/Unit/Boss/Shark/move/0.dds", FADE, 1);
		break;
	case ANGLER: p = new Particle("image/Unit/Boss/ANGLER/move/0.dds", FADE, 1);
		break;
	case CRAB: p = new Particle("image/Unit/Boss/CRAB/move/0.dds", FADE, 1);
		break;
	case PAGURIAN: p = new Particle("image/Unit/Boss/PAGURIAN/move/0.dds", FADE, 1);
		break;
	}
	gm.ingame->addChild(p);
	gm.ingame->afterimageList.push_back(p);
	p->pos = pos;

}