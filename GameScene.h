#pragma once
#include "Scene.h"
#include "Hitmap.h"
#include "Player.h"
#include "Bullet.h"
#include "Item.h"
#include "Hitmap.h"
#include "Particle.h"
#include "Addon.h"
#include "UI.h"


class GameScene
	:public Scene
{
public:
	GameScene();
	~GameScene();

	void render();
	void update(float dt);

	UI* ui;
	Sprite* bg[3];
	Hitmap* hitmap;
	Player* player;
	Particle* clear;
	float clearTimer;

	vector<Addon*>addonList;
	vector<Particle*>afterimageList;
	vector<Unit*> unitList;
	vector<Bullet*> bulletList;
	vector<Item*> itemList;
	vector<Animation*> effectList;
	vector<Particle*> particleList;

	bool stage_clear;
};

