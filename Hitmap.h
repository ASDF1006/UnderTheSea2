#pragma once
#include "Entity.h"
#include "Sprite.h"

class Hitmap :
	public Entity
{
public:
	Hitmap();
	~Hitmap();

	void init();
	void mapSetting(D3DCOLOR color, int x, int y);
	void setBit(int x, int y, bool b);
	bool getBit(int x, int y);
	void spawn_enemy(UnitType type, int x, int y);

	int* mapBit;
	int mapWidth;

	Sprite* hitmap[25];
	Sprite* bg[25];
};

