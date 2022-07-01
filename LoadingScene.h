#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Animation.h"

class LoadingScene :
	public Scene
{
public:
	LoadingScene();
	~LoadingScene();

	void update(float dt);

	Sprite* bg;
	Sprite* loading_frame;
	Sprite* loading_bar;
	Sprite* front;
	Animation* ani;
};

