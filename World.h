#pragma once
#include "Define.h"
#include "Timer.h"
#include "Scene.h"

class World
{
public:
	World();
	~World();
	void update(float dt);
	void render();
	void init();
	void dispose();
	void changeScene(Scene* scene);
	int getKeyState(int vk);
	Vec2 getMousePos();

	list<Timer*>timers;
	Scene* currentScene;
	POINT mousePoint;
	LPD3DXSPRITE rootSprite;
	bool lastKeys[256];
	bool currentKeys[256];
};


extern World world;