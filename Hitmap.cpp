#include "DXUT.h"
#include "Hitmap.h"
#include "GameManager.h"
#include "Enemy.h"


Hitmap::Hitmap()
{
	for (int i = 0; i < 25; i++)
	{
		hitmap[i] = new Sprite("image/Map/"+ to_string(gm.stage) + "/hitmap/" + to_string(gm.map_type[i]) + ".png");
		bg[i] = new Sprite("image/Map/" + to_string(gm.stage) + "/" + to_string(gm.map_type[i]) + ".png");
		addChild(bg[i]);
		bg[i]->pos.y = hitmap[i]->pos.y = hitmap[i]->rect.height() * i;
	}

	init();
}


Hitmap::~Hitmap()
{
	for (int i = 0; i < 25; i++)
	{
		SAFE_DELETE(hitmap[i]);
	}
	delete mapBit;
}


void Hitmap::init()
{
	DWORD* data;
	D3DLOCKED_RECT lockRect;

	int x = 1500;
	int y = 1000 * 25;
	mapWidth = x;
	int totalBit = x * y;

	mapBit = new int[totalBit / 32 + (totalBit % 32 ? 1 : 0)];
	ZeroMemory(mapBit, sizeof(int) * totalBit / 32 + (totalBit % 32 ? 1 : 0));

	for (int i = 0; i < 25; i++)
	{
		LPDIRECT3DSURFACE9 surface;
		hitmap[i]->texture->d3dTexture->GetSurfaceLevel(0, &surface);
		surface->LockRect(&lockRect, 0, 0);
		data = (DWORD*)lockRect.pBits;

		for (int j = 0; j < hitmap[0]->rect.bottom; j++)
		{
			for (int k = 0; k < x; k++)
			{
				int index = j * lockRect.Pitch / 4 + k;
				mapSetting(data[index], k, j + 1000 * i);
			}
		}

		surface->UnlockRect();
		SAFE_RELEASE(surface);
	}
}

void Hitmap::mapSetting(D3DCOLOR color, int x, int y)
{
	switch (color)
	{
	case D3DCOLOR_XRGB(0, 0, 0): setBit(x, y, true); break;
	case D3DCOLOR_XRGB(255, 0, 0): spawn_enemy(CARP, x, y); break;
	case D3DCOLOR_XRGB(0, 255, 0) : spawn_enemy(PUFFER, x, y); break;
	case D3DCOLOR_XRGB(0, 0, 255) : spawn_enemy(OCTOPUS, x, y); break;
	case D3DCOLOR_XRGB(255, 255, 0) : spawn_enemy(MARLIN, x, y); break;
	case D3DCOLOR_XRGB(0, 255, 255) : spawn_enemy(ROCK, x, y); break;
	}
}


void Hitmap::setBit(int x, int y, bool b)
{
	if (getBit(x, y) == b) return;
	int Pitch = y * mapWidth + x;
	mapBit[Pitch / 32] ^= (1 << Pitch % 32);
}


bool Hitmap::getBit(int x, int y)
{
	if (x < 0 || x > 1500) return false;
	if (y < 0 || y > 25000) return false;
	int Pitch = y * mapWidth + x;
	return mapBit[Pitch / 32] & (1 << Pitch % 32);
}


void Hitmap::spawn_enemy(UnitType type, int x, int y)
{
	Enemy* e;
	switch (type)
	{
	case ROCK: e = new Rock();
		break;
	case CARP: e = new Carp();
		break;
	case PUFFER: e = new Puffer();
		break;
	case MARLIN: e = new Marlin();
		break;
	case OCTOPUS: e = new Octopus();
		break;
	}

	gm.ingame->addChild(e);
	e->setCenter(Vec2(x, y));
}