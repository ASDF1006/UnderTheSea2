#include "DXUT.h"
#include "IntroScene.h"
#include "World.h"
#include "MainScene.h"


IntroScene::IntroScene(): getKey(false)
{
	bg = new Animation("image/Intro/", 31, 20);
}


IntroScene::~IntroScene()
{
}


void IntroScene::render()
{
	bg->render();
}


void IntroScene::update(float dt)
{
	Scene::update(dt);

	if (world.getKeyState(VK_RETURN) == -1)
	{
		getKey = true;
	}

	if (getKey)
	{
		bg->update(dt);
		if (bg->is_end)
		{
			world.changeScene(new MainScene());
			return;
		}
	}
}