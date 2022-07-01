#pragma once
#include "Scene.h"
#include "Animation.h"

class IntroScene :
	public Scene
{
public:
	IntroScene();
	~IntroScene();

	void render();
	void update(float dt);

	Animation* bg;
	bool getKey;
};

