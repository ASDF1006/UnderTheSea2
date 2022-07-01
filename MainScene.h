#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Animation.h"

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	void update(float dt);

	Animation* bg;
	Sprite* title;
	Sprite* howto;
	Sprite* button[4];
	Sprite* focus[4];
};

