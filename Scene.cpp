#include "DXUT.h"
#include "Scene.h"


Scene::Scene() : followTarget(nullptr), offset(0, 0)
{
}


Scene::~Scene()
{
}


void Scene::follow(Entity* target, Vec2 offset)
{
	followTarget = target;
	this->offset = offset;
}


void Scene::unfollow()
{
	followTarget = nullptr;
}


void Scene::shake(float duration, int count, float range)
{
	pos_origin = pos;
	shakeTimer.reset(duration, count);
	shakeTimer.onTick = [=]() {
		pos += Vec2(random(-range, range), random(-range, range));
	};
	shakeTimer.onFinished = [=]() {
		if (!followTarget) {
			pos.y = pos_origin.y;
		}
	};
}


void Scene::update(float dt)
{
	Entity::update(dt);

	if (followTarget) {
		pos.y -= (followTarget->pos.y - offset.y + pos.y) * 5 * dt;
	}


	if (pos.y > 0)
		pos.y = 0;
	else if (pos.y < -24280)
		pos.y = -24280;
}