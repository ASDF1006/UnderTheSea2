#include "DXUT.h"
#include "MainScene.h"
#include "World.h"
#include "GameScene.h"
#include "RankingScene.h"
#include "InputScene.h"
#include "EndingScene.h"
#include "GameManager.h"
#include "StoreScene.h"


MainScene::MainScene()
{
	gm.init();

	bg = new Animation("image/Main/bg/", 20, 10, 1);
	title = new Sprite("image/Main/title.png");
	howto = new Sprite("image/Main/howto.png");

	addChild(bg);
	addChild(title);
	addChild(howto);

	howto->visible = false;

	for (int i = 0; i < 4; i++)
	{
		button[i] = new Sprite("image/Main/Button/" + to_string(i) + ".png");
		focus[i] = new Sprite("image/Main/Button/" + to_string(i + 4) + ".png");

		addChild(button[i]);
		addChild(focus[i]);

		focus[i]->pos = button[i]->pos = Vec2(1000, 300 + i * 70);
		focus[i]->visible = false;
	}
}


MainScene::~MainScene()
{
}


void MainScene::update(float dt)
{
	Scene::update(dt);


	if (!howto->visible)
	{
		for (int i = 0; i < 4; i++)
		{
			if (button[i]->rectWithPos().contain(world.getMousePos()))
			{
				button[i]->visible = false;
				focus[i]->visible = true;
				if (world.getKeyState(VK_LBUTTON) == 1)
				{
					if (i == 0)
					{
						world.changeScene(new GameScene());
						return;
					}
					else if (i == 1)
					{
						bg->visible = false;
						title->visible = false;
						howto->visible = true;
						for (int j = 0; j < 4; j++)
						{
							button[j]->visible = focus[j]->visible = false;
						}
						return;
					}
					else if (i == 2)
					{
						//world.changeScene(new StoreScene());
						world.changeScene(new RankingScene());
						return;
					}
					else if (i == 3)
					{
						world.changeScene(new EndingScene());
						return;
						PostQuitMessage(0);
					}
				}
			}
			else
			{
				button[i]->visible = true;
				focus[i]->visible = false;
			}
		}
	}
	else
	{
		if (world.getKeyState(VK_RETURN) == -1)
		{
			howto->visible = false;
			bg->visible = true;
			title->visible = true;
			for (int j = 0; j < 4; j++)
			{
				button[j]->visible = true;
				focus[j]->visible = false;
			}
		}
	}
}
