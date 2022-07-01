#pragma once
#include "Scene.h"
#include "Animation.h"


class EndingScene :
	public Scene
{
public:
	EndingScene();
	~EndingScene();

	void update(float dt);

	Animation* bg;
	bool getKey;
};