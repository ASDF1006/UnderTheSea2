#include "DXUT.h"
#include "Player.h"
#include "World.h"
#include "Bullet.h"
#include "GameManager.h"
#include "Addon.h"


Player::Player() 
	: Unit(ALLY, PLAYER, 200 + gm.upgrade[1] * 100, 0.2, 500, 10 + gm.upgrade[2] * 3), is_fade_in(true), torpedo_count(5), is_die_end(false),
	is_invincible(false), invincibleTimer(0), bubbleTimer(0), is_shield(false), is_shield_fade_in(true)
{
	move = new Animation("image/Unit/Player/move/", 32, 40);
	die = new Animation("image/Unit/Player/die/", 21, 20);
	shield = new Sprite("image/Item/shield.png");
	addChild(move);
	addChild(die);
	addChild(shield);

	move->currentFrame = 11;
	shield->visible = false;
	die->visible = false;
	rect = move->rect;

	hitbox = Rect(50, 20, 110, 140);
}


Player::~Player()
{
}


void Player::update(float dt)
{
	if (!is_die)
	{
		fire_delay += dt;
		bubbleTimer += dt;

		//move
		if (world.getKeyState(VK_UP) == 2)
		{
			pos.y -= speed * dt;

			if(pos.y < 0)
				pos.y -= speed * dt;

			if (gm.ingame->followTarget)
			{
				for (int i = 0; i < 3; i++)
				{
					gm.ingame->bg[i]->pos.y -= 150 * dt;
					if (gm.ingame->bg[i]->pos.y > -gm.ingame->pos.y + 800)
						gm.ingame->bg[i]->pos.y -= 2998;

				}
			}
		}
		else if (world.getKeyState(VK_DOWN) == 2)
		{
			pos.y += speed / 2 * dt;

			if(pos.y + hitbox.bottom > 25000)
				pos.y -= speed / 2 * dt;

			else if(gm.ingame->followTarget)
			{
				for (int i = 0; i < 3; i++)
				{
					gm.ingame->bg[i]->pos.y += 50 * dt;
					if (gm.ingame->bg[i]->pos.y < -gm.ingame->pos.y - 1800)
						gm.ingame->bg[i]->pos.y += 2998;
				}
			}
			else if(pos.y + hitbox.height() > -gm.ingame->pos.y + 720)
			{
				pos.y = -gm.ingame->pos.y + 720 - hitbox.height();
			}
		}

		if (world.getKeyState(VK_LEFT) == 1)
		{
			move->currentFrame = 11;
		}
		else if (world.getKeyState(VK_LEFT) == 2)
		{
			pos.x -= speed * dt;
			if (pos.x < 0)
				pos.x += speed * dt;
			move->update(-dt);
		}
		else if (world.getKeyState(VK_RIGHT) == 1)
		{
			move->currentFrame = 21;
		}
		else if (world.getKeyState(VK_RIGHT) == 2)
		{
			pos.x += speed * dt;
			if (pos.x + rect.right > 1500)
				pos.x -= speed * dt;
			move->update(dt);
		}
		else if (move->currentFrame < 22)
		{
			move->update(dt);
			if (move->currentFrame > 21)
				move->currentFrame = 11;
		}
		else if (move->currentFrame >= 22)
		{
			move->update(-dt);
		}


		//attack
		if (world.getKeyState('Z') == 2)
		{
			if (fire_delay > fire_delay_origin)
			{
				Bullet* b = new Bullet(team, B_PLAYER, damage);
				gm.ingame->addChild(b);
				gm.ingame->bulletList.push_back(b);
				b->angleWithTarget = D3DXToRadian(270);
				b->setCenter(center() - Vec2(0, 50));
				b->is_homing = gm.upgrade[0];
				b->speed = 1000;
				fire_delay = 0;
			}
		}
		if (world.getKeyState('X') == 1)
		{
			if (torpedo_count > 0)
			{
				Bullet* b = new Bullet(team, B_TORPEDO, 100);
				gm.ingame->addChild(b);
				gm.ingame->bulletList.push_back(b);
				b->angleWithTarget = D3DXToRadian(270);
				b->setCenter(center());
				b->speed = 800;
				torpedo_count--;
			}
		}


		//hit check
		for (Unit* u : gm.ingame->unitList)
		{
			if (u == this) continue;

			if (rectWithPos().intersects(u->hitbox.offset(u->pos)))
			{
				hit(20);

				if (u->type == ROCK){
					u->is_die = true;
				}
			}
		}

		if (gm.ingame->hitmap->getBit(pos.x + hitbox.right, pos.y + hitbox.top) ||
			gm.ingame->hitmap->getBit(pos.x + hitbox.right, pos.y + hitbox.bottom) ||
			gm.ingame->hitmap->getBit(pos.x + hitbox.left, pos.y + hitbox.top) ||
			gm.ingame->hitmap->getBit(pos.x + hitbox.left, pos.y + hitbox.bottom))
		{
			hit(10);
		}

		//invincible & shield
		if (is_invincible)
		{
			if (is_fade_in)
			{
				move->color.a += 6 * dt;
				if (move->color.a > 1)
					is_fade_in = false;
			}
			else
			{
				move->color.a -= 6 * dt;
				if (move->color.a < 0)
					is_fade_in = true;
			}
			invincibleTimer += dt;
			if (invincibleTimer > 1)
			{
				invincibleTimer = 0;
				move->color.a = 1;
				is_invincible = false;
			}
		}

		if (is_shield)
		{
			if (is_shield_fade_in)
			{
				shield->color.a += dt / 4;
				if (shield->color.a > shield_max_alpha)
				{
					is_shield_fade_in = false;
					shield_max_alpha = 0.8;
				}
			}
			else
			{
				shield->color.a -= dt / 4;
				if (shield->color.a < 0.4)
					is_shield_fade_in = true;
			}
		}


		//bubble particle
		if (bubbleTimer > 0.2)
		{
			for (int i = 0; i < 2; i++)
			{
				Particle* p = new Particle("image/Effect/bubble.png", FADE, 1);
				gm.ingame->addChild(p);
				gm.ingame->particleList.push_back(p);
				p->setCenter(Vec2(pos.x + random(hitbox.left, hitbox.right), pos.y + rect.height()));
				p->color.a = 0.7;
				p->is_move = true;
				p->move_angle = D3DXToRadian(random(70, 110));
				p->move_speed = 70;
			}
			bubbleTimer = 0;
		}

		if (hp <= 0)
		{
			is_die = true;
		}
	}
	else
	{
		if (!is_die_end)
		{
			if (die->currentFrame == 0)
			{
				die->visible = true;
				move->visible = false;
				explodeTimer.reset(0.1, 10);
				explodeTimer.onTick = [=]()
				{
					Animation* a = new Animation("image/Effect/Explode/3/", 11, 15);
					gm.ingame->addChild(a);
					gm.ingame->effectList.push_back(a);
					a->setCenter(pos + Vec2(random(hitbox.left, hitbox.right), random(hitbox.top, hitbox.bottom)));
				};
			}

			die->update(dt);

			if (die->is_end)
			{
				is_die_end = true;
				visible = false;
				die->visible = false;
				move->visible = true;
				die->is_end = false;
				die->currentFrame = 0;

				gm.ingame->ui->continue_scene->visible = true;
				gm.ingame->ui->continue_button[1]->visible = true;
				gm.ingame->ui->continue_button[2]->visible = true;
				gm.addon_count = 0;
				for (Addon* a : gm.ingame->addonList)
					a->deleting = true;
			}
		}
	}
}


void Player::hit(int damage)
{
	if (!is_invincible)
	{
		if (is_shield) {
			is_shield = false;
			shield->visible = false;
		}
		else {
			hp -= damage;
			gm.combo = 0;
		}
		gm.ingame->shake(0.0001f, 10, 10);
		is_invincible = true;
		float ratio = (float)hp / hp_origin / 2;
		Particle* p = new Particle("image/UI/hit.png", FADE, 4, 1 - ratio, true);
		gm.ingame->ui->addChild(p);
		gm.ingame->ui->fadeList.push_back(p);
	}
}