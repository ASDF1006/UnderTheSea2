#include "DXUT.h"
#include "Bullet.h"
#include "GameManager.h"
#include "Particle.h"


Bullet::Bullet(Team team, BulletType type, int damage)
	: team(team), type(type), damage(damage), angleWithTarget(0), speed(600), homingTimer(0), y_speed(0), gravity(0), is_barrage(false),
	is_spawn_bullet(false), is_homing(false), target(nullptr)
{
	if (type == B_PLAYER) {
		sprite = new Sprite("image/Bullet/player.png");
	}
	else if (type == B_ADDON) {
		sprite = new Sprite("image/Bullet/player.png");
		sprite->scale = Vec2(0.8, 0.8);
	}
	else if (type == B_TORPEDO){
		sprite = new Animation("image/Bullet/Torpedo/", 11, 20, 1, 1);
	}
	else if (type == B_PUFFER) {
		sprite = new Sprite("image/Bullet/shark.png");
	}
	else if (type == B_CARP){
		sprite = new Sprite("image/Bullet/carp.png");
	}
	else if (type == B_OCTOPUS) {
		sprite = new Sprite("image/Bullet/octopus.png");
	}
	else if (type == B_HOMING) {
		sprite = new Sprite("image/Bullet/homing.png");
	}
	else if (type == B_RAIN){
		sprite = new Sprite("image/Bullet/rain.png");
	}
	else if (type == B_BARRAGE) {
		sprite = new Sprite("image/Bullet/rain.png");
	}
	else if (type == B_BUBBLE) {
		sprite = new Sprite("image/Bullet/bubble.png");
	}
	else if (type == B_WAVE) {
		sprite = new Particle("image/Bullet/bubble.png", FADE, 3, 1, true);
	}
	else if (type == B_SHARK) {
		sprite = new Sprite("image/Bullet/shark.png");
	}
	else{
		sprite = new Sprite("image/Bullet/player.png");
	}

	addChild(sprite);
	rect = sprite->rect;
	scaleCenter = rotationCenter = center();
}


Bullet::~Bullet()
{
}


void Bullet::update(float dt)
{
	rotation = angleWithTarget;

	if (type == B_HOMING)
	{
		homingTimer += dt;

		if (homingTimer < 2)
			angleWithTarget = angle(center(), gm.ingame->player->center());

		pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;
	}
	else if (type == B_RAIN)
	{
		pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;
		pos.x += cos(angleWithTarget) * speed * dt;
		gravity += 1000 * dt;
		pos.y += (y_speed + gravity) * dt;
		Particle* p = new Particle("image/Bullet/rain.png", FADE, 5);
		gm.ingame->addChild(p);
		gm.ingame->particleList.push_back(p);
		p->pos = pos;
	}
	else if (type == B_BARRAGE)
	{
		if (!is_barrage)
		{
			pos.x += cos(angleWithTarget) * speed * dt;
			gravity += 1000 * dt;
			pos.y += (y_speed + gravity) * dt;

			if ((y_speed + gravity) > 0 && pos.y + gm.ingame->pos.y - 100 > 0)
			{
				angleWithTarget = angle(center(), gm.ingame->player->center());
				is_barrage = true;
				speed = 1000;
			}
		}
		else
		{
			pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;
		}

		pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;
		Particle* p = new Particle("image/Bullet/rain.png", FADE, 5);
		gm.ingame->addChild(p);
		gm.ingame->particleList.push_back(p);
		p->pos = pos;
		p->color.a = 1;
	}
	else if (type == B_BUBBLE)
	{
		pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;
		speed -= 70 * dt;
		if (speed < 0)
		{
			deleting = true;
			Particle* p = new Particle("image/Bullet/bubble.png", FADE, 3);
			gm.ingame->addChild(p);
			gm.ingame->particleList.push_back(p);
			p->pos = pos;
		}
	}
	else if(type == B_WAVE)
	{
		if (sprite->color.a > 0.7 && !is_spawn_bullet && !deleting)
		{
			Bullet* b = new Bullet(team, B_WAVE, damage);
			gm.ingame->addChild(b);
			gm.ingame->bulletList.push_back(b);
			b->angleWithTarget = angleWithTarget;
			b->pos = pos + Vec2(cos(angleWithTarget), sin(angleWithTarget)) * 30;
			is_spawn_bullet = true;
		}
		deleting = sprite->deleting;
	}
	else
	{ 
		pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;
	}

	if (type == B_PLAYER || type == B_ADDON)
	{
		if (is_homing)
		{
			float d = 500;
			for (Unit* u : gm.ingame->unitList)
			{
				if (team == u->team) continue;

				if(d > distance(center(), u->center()))
				{
					target = u;
					d = distance(center(), u->center());
				}
			}
			if (target)
			{
				angleWithTarget = angle(center(), target->center());
			}
		}
		if ( pos.y < -gm.ingame->pos.y - rect.height())
		{
			deleting = true;
			return;
		}
	}
	else if (pos.x - rect.width() < 0 || pos.x > 1500 || pos.y > -gm.ingame->pos.y + 720)
	{
		deleting = true;
		return;
	}

	if (gm.ingame->hitmap->getBit(center().x, center().y))
	{
		if (type == B_TORPEDO)
		{
			for (int i = 0; i < 15; i++)
			{
				Animation* a = new Animation("image/Effect/die/", 15, 20, 0, 1);
				gm.ingame->addChild(a);
				gm.ingame->effectList.push_back(a);
				a->setCenter(center() + Vec2(random(-200, 200), random(-200, 200)));
			}
			for (Unit* u : gm.ingame->unitList) {
				if (team == u->team) continue;
				u->hp -= damage;
			}
			deleting = true;
			return;
		}

		else
		{
			Animation* a;

			if (type == B_PLAYER || type == B_ADDON)
			{
				a = new Animation("image/Effect/Explode/1/", 6, 15, 0, 1);
			}
			else
			{
				a = new Animation("image/Effect/Explode/2/", 11, 15, 0, 1);
			}

			gm.ingame->addChild(a);
			gm.ingame->effectList.push_back(a);
			a->scaleCenter = a->rect.center();
			a->scale = Vec2(0.5, 0.5);
			a->setCenter(center());
			deleting = true;
			return;
		}
	}


	for (Unit* u : gm.ingame->unitList)
	{
		if (team == u->team) continue;

		if (rectWithPos().intersects(u->hitbox.offset(u->pos)))
		{
			if (u->type == PLAYER)
			{
				gm.ingame->player->hit(damage);
			}
			else
			{
				u->hp -= damage;
				if (u->type != ROCK)
				{
					u->attack->color.b = u->attack->color.g = u->move->color.b = u->move->color.g = 0.6;
					u->red_filter = true;
				}
			}

			if (type == B_PLAYER)
			{
				gm.combo++;
				float combo_angle = random(D3DXToRadian(240), D3DXToRadian(300));

				Particle* p = new Particle("image/UI/Combo/combo.png", COMBO);
				gm.ingame->addChild(p);
				gm.ingame->particleList.push_back(p);
				p->setCenter(center() + Vec2(cos(combo_angle), sin(combo_angle)) * 100);
				p->scale = Vec2(1.2, 1.2);
				p->move_angle = combo_angle;

				int combo_tmp = gm.combo;
				bool combo_visible = true;

				for (int i = 0; i < 3; i++)
				{
					if (combo_tmp <= 0 && !(i == 0))
						combo_visible = false;

					Particle* s = new Particle("image/UI/Combo/Num/" + to_string(combo_tmp % 10) + ".png", COMBO);
					gm.ingame->addChild(s);
					gm.ingame->particleList.push_back(s);
					s->setCenter(p->center() + Vec2(i * -24 - 60 , 0));
					p->scale = Vec2(1.2, 1.2);
					s->visible = combo_visible;
					s->move_angle = combo_angle;

					combo_tmp /= 10;
				}

			}
			if (type == B_TORPEDO)
			{
				for (int i = 0; i < 15; i++)
				{
					Animation* a = new Animation("image/Effect/die/", 15, 20, 0, 1);
					gm.ingame->addChild(a);
					gm.ingame->effectList.push_back(a);
					a->setCenter(center() + Vec2(random(-200, 200), random(-200, 200)));
				}
				for (Unit* u : gm.ingame->unitList) {
					if (team == u->team) continue;
					u->hp -= damage;
				}
			}
			else if (team == ALLY)
			{
				Animation* a = new Animation("image/Effect/Explode/1/", 6, 15, 0, 1);
				gm.ingame->addChild(a);
				gm.ingame->effectList.push_back(a);
				a->scaleCenter = a->rect.center();
				a->setCenter(center());
				if (type == B_PLAYER) {
					a->scale = Vec2(0.5, 0.5);
					gm.ingame->shake(0.0001, 3, 5);
				}
				else
					a->scale = Vec2(0.3, 0.3);
			}
			else if (type == B_OCTOPUS)
			{
				Particle* p = new Particle("image/UI/black.png", FADE, 3, 3, true);
				gm.ingame->ui->addChild(p);
				gm.ingame->ui->fadeList.push_back(p);
			}
			else
			{
				Animation* a = new Animation("image/Effect/Explode/2/", 11, 15, 0, 1);
				gm.ingame->addChild(a);
				gm.ingame->effectList.push_back(a);
				a->scaleCenter = a->rect.center();
				a->scale = Vec2(0.5, 0.5);
				a->setCenter(center());
			}

			deleting = true;
			return;
		}
	}

	Entity::update(dt);
}