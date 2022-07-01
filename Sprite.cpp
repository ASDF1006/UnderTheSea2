#include "DXUT.h"
#include "Sprite.h"
#include "Asset.h"
#include "World.h"


Sprite::Sprite(): color(1,1,1,1)
{
	texture = nullptr;
}


Sprite::Sprite(string path) : color(1, 1, 1, 1)
{
	setPath(path);
}

Sprite::~Sprite()
{
}


void Sprite::setPath(string path)
{
	texture = asset.getTexture(path);

	visibleRect = rect = Rect(0, 0, texture->info.Width, texture->info.Height);
}




void Sprite::render()
{
	if (!visible || !texture) return;

	Entity::render();

	world.rootSprite->Begin(D3DXSPRITE_ALPHABLEND);
	world.rootSprite->SetTransform(&matrix);
	world.rootSprite->Draw(texture->d3dTexture, &visibleRect, NULL, NULL, color);
	world.rootSprite->End();
}