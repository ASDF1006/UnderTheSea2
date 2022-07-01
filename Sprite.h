#pragma once
#include "Entity.h"
#include "Texture.h"

class Sprite
	:public Entity
{
public:
	Sprite();
	Sprite(string path);
	virtual ~Sprite();

	void render();
	void setPath(string path);

	D3DXCOLOR color;
	Texture* texture;
};

