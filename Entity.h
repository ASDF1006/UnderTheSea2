#pragma once
#include "Define.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();
	virtual void update(float dt);
	virtual void render();
	void addChild(Entity* child);
	void removeChild(Entity* child);
	void setCenter(Vec2 pos);
	Vec2 center();
	Rect rectWithPos();

	list<Entity*>children;
	D3DXMATRIX matrix;
	Entity* parent;
	Rect rect;
	Rect visibleRect;
	Vec2 pos;
	Vec2 scale;
	Vec2 scaleCenter;
	float rotation;
	Vec2 rotationCenter;
	bool visible;
	bool updateEnabled;
	bool renderChildrenEnabled;
	bool removing;
	bool deleting;
};

