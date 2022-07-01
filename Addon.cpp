#include "DXUT.h"
#include "Addon.h"
#include "GameManager.h"


Addon::Addon() : Unit(ALLY, ADDON, 5, 1, 200, 5), move_angle(0)
{
	move = new Animation("image/Unit/Addon/move/", 22, 40);
	addChild(move);

	move->currentFrame = 11;
	rect = move->rect;
	followTarget = gm.ingame->player;
}


Addon::~Addon()
{
}


void Addon::update(float dt)
{
	Unit::update(dt);
	fire_delay += dt;

	move_angle += dt;
	if (move_angle > 360)
		move_angle -= 360;

	offset = Vec2(cos(move_angle), sin(move_angle)) * 150;


	if (fire_delay > fire_delay_origin)
	{
		Bullet* b = new Bullet(team, B_ADDON, damage);
		gm.ingame->addChild(b);
		gm.ingame->bulletList.push_back(b);
		b->setCenter(center());
		b->angleWithTarget = D3DXToRadian(270);
		fire_delay = 0;
	}
	last_x = pos.x;

	pos += (followTarget->center() + offset - center()) * 3 * dt;


	if (last_x < pos.x)
	{
		move->update(dt);
	}
	else if(last_x >= pos.x)
	{
		move->update(-dt);
	}
}