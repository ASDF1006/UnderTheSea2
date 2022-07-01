#include "DXUT.h"
#include "Timer.h"
#include "World.h"



Timer::Timer(): active(false)
{
	onTick = []() {};
	onFinished = []() {};

	world.timers.push_back(this);
}


Timer::~Timer()
{
	world.timers.remove(this);
}


void Timer::reset(float duration, int count, bool autoStart)
{
	active = autoStart;
	this->duration.first = 0;
	this->duration.second = duration;
	this->count.first = 0;
	this->count.second = count;
}


void Timer::update(float dt)
{
	if (active)
	{
		duration.first += dt;

		if (duration.first > duration.second)
		{
			onTick();
			duration.first = 0;
			if (++count.first >= count.second)
			{
				active = false;
				onFinished();
				return;
			}
		}
	}
}