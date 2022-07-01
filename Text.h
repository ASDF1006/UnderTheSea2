#pragma once
#include "Entity.h"
class Text :
	public Entity
{
public:
	Text(string text, int size, D3DXCOLOR color);
	~Text();
	void render();

	LPD3DXFONT font;
	string text;
	int size;
	D3DCOLOR color;
};

