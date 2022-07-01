#include "DXUT.h"
#include "GameManager.h"


GameManager::GameManager()
{
	init();
	stage_init();
}


GameManager::~GameManager()
{
}


void GameManager::init()
{
	ingame = nullptr;
	currentBoss = nullptr;
	stage = 1;
	coin = 0;
	score = 0;
	for (int i = 0; i < 3; i++)
	{
		upgrade[i] = false;
	}
}


void GameManager::stage_init()
{
	for (int i = 0; i < 2; i++) {
		boss_spawn[i] = false;
		boss_ready[i] = false;
	}
	for (int i = 1; i < 25; i++)
	{
		map_type[i] = 5 - (i - 1) % 6;
	}
	map_type[0] = 0;
	combo = 0;
	addon_count = 0;
}


GameManager gm;