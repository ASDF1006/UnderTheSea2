#include "DXUT.h"
#include "EndingScene.h"
#include "World.h"
#include "InputScene.h"


EndingScene::EndingScene(): getKey(false)
{
	bg = new Animation("image/Ending/", 31, 20);

	addChild(bg);
}


EndingScene::~EndingScene()
{
}


void EndingScene::update(float dt)
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
			world.changeScene(new InputScene());
			return;
		}
	}
}