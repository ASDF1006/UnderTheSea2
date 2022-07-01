#include "DXUT.h"
#include "World.h"
#include "Asset.h"
#include "LoadingScene.h"


World::World():currentScene(nullptr)
{
}


World::~World()
{
}


void World::init()
{
	asset.init();
	cout.precision(4);
	D3DXCreateSprite(DXUTGetD3D9Device(), &rootSprite);
	changeScene(new LoadingScene());
	ZeroMemory(lastKeys, sizeof(lastKeys));
	ZeroMemory(currentKeys, sizeof(currentKeys));
}


void World::update(float dt)
{
	if (currentScene) currentScene->update(dt);

	for (int i = 0; i < 256; i++)
	{
		lastKeys[i] = currentKeys[i];
		currentKeys[i] = GetAsyncKeyState(i) & 0x8000;
	}

	GetCursorPos(&mousePoint);
	ScreenToClient(DXUTGetHWND(), &mousePoint);

	for (auto timer : timers)
		timer->update(dt);
}


void World::render()
{
	if (currentScene) currentScene->render();
}


void World::dispose()
{
	asset.dispose();
	SAFE_RELEASE(rootSprite);
	SAFE_DELETE(currentScene);
}


void World::changeScene(Scene* scene)
{
	if(currentScene) SAFE_DELETE(currentScene);

	currentScene = scene;
}


int World::getKeyState(int vk)
{
	if (lastKeys[vk] && currentKeys[vk]) return 2;
	if (!lastKeys[vk] && currentKeys[vk]) return 1;
	if (lastKeys[vk] && !currentKeys[vk]) return -1;
	return -2;
}


Vec2 World::getMousePos()
{
	return Vec2(mousePoint.x, mousePoint.y);
}


World world;