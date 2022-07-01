#pragma once
#include "Scene.h"
#include "Sprite.h"

class StoreScene :
	public Scene
{
public:
	StoreScene();
	~StoreScene();

	void update(float dt);

	Sprite* bg;
	Sprite* button[3];
	Sprite* click[3];
	Sprite* purchased[3];
	
	Sprite* coin_num[3];
};

