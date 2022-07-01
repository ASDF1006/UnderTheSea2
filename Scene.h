#pragma once
#include "Entity.h"
#include "Timer.h"

class Scene :
	public Entity
{
public:
	Scene();
	~Scene();
	
	void update(float dt);
	void shake(float duration, int count, float range);
	void follow(Entity* target, Vec2 offset = Vec2(1280, 720) / 2);
	void unfollow();

	Entity* followTarget;
	Vec2 offset;
	Timer shakeTimer;
	Vec2 pos_origin;
};

