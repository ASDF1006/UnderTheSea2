#pragma once
#include "Entity.h"
#include "Animation.h"

class Item :
	public Entity
{
public:
	Item(ItemType type);
	~Item();

	//void update(float dt);

	Animation* ani;
	ItemType type;
};

