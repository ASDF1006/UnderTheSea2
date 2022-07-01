#include "DXUT.h"
#include "StoreScene.h"
#include "World.h"
#include "GameScene.h"
#include "GameManager.h"


StoreScene::StoreScene()
{
	bg = new Sprite("image/Store/shop.png");
	addChild(bg);

	for (int i = 0; i < 3; i++)
	{
		button[i] = new Sprite("image/Store/0.png");
		click[i] = new Sprite("image/Store/1.png");
		purchased[i] = new Sprite("image/Store/2.png");

		addChild(button[i]);
		addChild(click[i]);
		addChild(purchased[i]);

		purchased[i]->pos = click[i]->pos = button[i]->pos = Vec2(234 + i * 322, 477);

		click[i]->visible = false;
		purchased[i]->visible = false;

		coin_num[i] = new Sprite("image/UI/ScoreNum/0.png");
		addChild(coin_num[i]);

		coin_num[i]->pos = Vec2(1030 - i * 25, 110);
	}
}


StoreScene::~StoreScene()
{
}


void StoreScene::update(float dt)
{
	Scene::update(dt);

	if (world.getKeyState(VK_RETURN) == -1)
	{
		world.changeScene(new GameScene());
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		if (!purchased[i]->visible && world.getKeyState(VK_LBUTTON) == 1)
		{
			if (button[i]->rectWithPos().contain(world.getMousePos()))
			{
				button[i]->visible = false;
				click[i]->visible = true;
			}
		}
		else if (!purchased[i]->visible && world.getKeyState(VK_LBUTTON) == -1)
		{
			if (button[i]->rectWithPos().contain(world.getMousePos()))
			{
				click[i]->visible = false;
				purchased[i]->visible = true;
				gm.upgrade[i] = true;
				gm.coin -= 100;
			}
			else
			{
				button[i]->visible = true;
				click[i]->visible = false;
			}
		}
	}

	if (gm.coin < 100)
	{
		for (int i = 0; i < 3; i++)
		{
			purchased[i]->visible = true;
			button[i]->visible = false;
			click[i]->visible = false;
		}
	}


	int coin_tmp = gm.coin;
	bool coin_visible = true;

	for (int i = 0; i < 3; i++)
	{
		if (coin_tmp <= 0 && !(i == 0))
			coin_visible = false;
		coin_num[i]->setPath("image/UI/ScoreNum/" + to_string(coin_tmp % 10) + ".png");
		coin_num[i]->visible = coin_visible;
		coin_tmp /= 10;
	}
}