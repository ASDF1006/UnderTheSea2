#pragma once
#include "GameScene.h"
#include "Boss.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void init();
	void stage_init();

	GameScene* ingame;
	Boss* currentBoss;
	int stage;
	int coin;
	int combo;
	float score;
	int addon_count;
	int map_type[25];
	bool boss_spawn[2];
	bool boss_ready[2];
	bool upgrade[3];
};


extern GameManager gm;