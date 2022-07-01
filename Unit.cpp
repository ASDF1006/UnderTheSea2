#include "DXUT.h"
#include "Unit.h"
#include "GameManager.h"


Unit::Unit(Team team, UnitType type, int hp, float fire_delay, int speed, int damage)
	:team(team), type(type), hp(hp), hp_origin(hp), fire_delay(fire_delay - 1), fire_delay_origin(fire_delay), 
	speed(speed), damage(damage), is_die(false), red_filter(false)
{
}


Unit::~Unit()
{
}


void Unit::update(float dt)
{
}
