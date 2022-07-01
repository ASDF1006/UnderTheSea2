#include "DXUT.h"
#include "UI.h"
#include "GameManager.h"
#include "World.h"


UI::UI() : focus_yes(true)
{
	frame = new Sprite("image/UI/ui.png");
	addChild(frame);

	hp_bar = new Sprite("image/UI/p_hp.png");
	addChild(hp_bar);
	hp_bar->pos = Vec2(100, 38);

	boss_hp_frame = new Sprite("image/UI/b_frame.png");
	boss_hp_bar = new Sprite("image/UI/b_hp.png");

	addChild(boss_hp_frame);
	addChild(boss_hp_bar);

	boss_hp_bar->pos = Vec2(270, 602);
	boss_hp_frame->visible = boss_hp_bar->visible = false;


	addon_num = new Sprite("image/UI/AddonNum/0.png");
	addChild(addon_num);
	addon_num->pos = Vec2(98, 644);

	for (int i = 0; i < 2; i++)
	{
		torpedo_num[i] = new Sprite("image/UI/TorpedoNum/0.png");

		addChild(torpedo_num[i]);

		torpedo_num[i]->pos = Vec2(150 - i * 23, 120);
	}
	for (int i = 0; i < 3; i++)
	{
		coin_num[i] = new Sprite("image/UI/CoinNum/0.png");

		addChild(coin_num[i]);
		coin_num[i]->pos = Vec2(150 - i * 23, 72);
	}
	for (int i = 0; i < 5; i++)
	{
		score_num[i] = new Sprite("image/UI/ScoreNum/0.png");
		addChild(score_num[i]);
		score_num[i]->pos = Vec2(1180 - i * 26, 70);
	}
	continue_scene = new Sprite("image/UI/Continue/0.png");
	addChild(continue_scene);
	continue_scene->visible = false;

	for (int i = 0; i < 4; i++)
	{
		continue_button[i] = new Sprite("image/UI/Continue/" + to_string(i+1) + ".png");
		addChild(continue_button[i]);
		if (i % 2 == 0)
			continue_button[i]->setCenter(Vec2(500, 400));
		else
			continue_button[i]->setCenter(Vec2(800, 400));
		continue_button[i]->visible = false;
	}
}


UI::~UI()
{
}


void UI::update(float dt)
{
	Entity::update(dt);

	hp_bar->visibleRect.right = (float)gm.ingame->player->hp / gm.ingame->player->hp_origin * hp_bar->rect.right;

	if (boss_hp_bar->visible)
	{
		boss_hp_bar->visibleRect.right = (float)gm.currentBoss->hp / gm.currentBoss->hp_origin * boss_hp_bar->rect.right;
	}


	addon_num->setPath("image/UI/AddonNum/" + to_string(gm.addon_count) + ".png");

	int torpedo_tmp = gm.ingame->player->torpedo_count;
	bool torpedo_visible = true;

	for (int i = 0; i < 2; i++)
	{
		if (torpedo_tmp <= 0 && !(i == 0))
			torpedo_visible = false;
		torpedo_num[i]->setPath("image/UI/TorpedoNum/" + to_string(torpedo_tmp % 10) + ".png");
		torpedo_num[i]->visible = torpedo_visible;
		torpedo_tmp /= 10;
	}

	int coin_tmp = gm.coin;
	bool coin_visible = true;

	for (int i = 0; i < 3; i++)
	{
		if (coin_tmp <= 0 && !(i == 0))
			coin_visible = false;
		coin_num[i]->setPath("image/UI/CoinNum/" + to_string(coin_tmp % 10) + ".png");
		coin_num[i]->visible = coin_visible;
		coin_tmp /= 10;
	}

	int score_tmp = gm.score;
	bool score_visible = true;

	for (int i = 0; i < 5; i++)
	{
		if (score_tmp <= 0 && !(i== 0))
			score_visible = false;

		score_num[i]->setPath("image/UI/ScoreNum/" + to_string(score_tmp % 10) + ".png");

		score_num[i]->visible = score_visible;
		score_tmp /= 10;
	}



	if (continue_scene->visible)
	{
		if (focus_yes && world.getKeyState(VK_RIGHT) == 1)
		{
			continue_button[0]->visible = true;
			continue_button[1]->visible = false;
			continue_button[2]->visible = false;
			continue_button[3]->visible = true;
			focus_yes = false;
		}
		else if (!focus_yes && world.getKeyState(VK_LEFT) == 1)
		{
			continue_button[0]->visible = false;
			continue_button[1]->visible = true;
			continue_button[2]->visible = true;
			continue_button[3]->visible = false;
			focus_yes = true;
		}
		if (world.getKeyState(VK_RETURN) == 1)
		{
			for (int i = 0; i < 4; i++)
				continue_button[i]->visible = false;
			continue_scene->visible = false;

			if (focus_yes)
			{
				gm.ingame->player->visible = true;
				gm.ingame->player->is_die = false;
				gm.ingame->player->hp = gm.ingame->player->hp_origin;
				gm.ingame->player->is_die_end = false;
				gm.ingame->player->is_invincible = true;
				gm.ingame->player->is_shield = false;
			}
			else
			{
				focus_yes = true;
			}
		}
	}

	fadeList.erase(remove_if(fadeList.begin(), fadeList.end(),
		[&](Particle* p) {
		if (p->deleting)
			removeChild(p);
		return p->deleting;
	}), fadeList.end());
}