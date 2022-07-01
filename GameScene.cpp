#include "DXUT.h"
#include "GameScene.h"
#include "GameManager.h"
#include "World.h"
#include "Boss.h"
#include "Enemy.h"
#include "InputScene.h"
#include "StoreScene.h"
#include "Asset.h"



GameScene::GameScene() : stage_clear(false), clearTimer(0)
{
	//renderChildrenEnabled = false;
	gm.ingame = this;
	gm.stage_init();

	pos = Vec2(0, -24000);

	for (int i = 0; i < 3; i++)
	{
		bg[i] = new Sprite("image/Map/background/" + to_string(gm.stage) + "/" + to_string(random(0, 1)) + ".png");
		addChild(bg[i]);
		bg[i]->pos = Vec2(0, -pos.y - 999 * i);
	}

	hitmap = new Hitmap();
	addChild(hitmap);

	ui = new UI();

	player = new Player();
	addChild(player);

	unitList.push_back(player);
	follow(player, Vec2(750, 580) - player->rect.center());
	player->setCenter(Vec2(750, 24500));
}


GameScene::~GameScene()
{
	delete ui;
}


void GameScene::render()
{
	Scene::render();

	for (auto i : addonList)
		i->render();
	for (auto i : afterimageList)
		i->render();
	for (auto i : unitList)
		i->render();
	for (auto i : bulletList)
		i->render();
	for (auto i : itemList)
		i->render();
	for (auto i : particleList)
		i->render();
	for (auto i : effectList)
		i->render();
	ui->render();
}


void GameScene::update(float dt)
{
	if (dt > 1)
	{
		cout << dt << endl;
		return;
	}
	//if (!asset.sounds[L"sound/ingameBGM.wav"]->IsSoundPlaying())
	//	asset.sounds[L"sound/ingameBGM.wav"]->Play();

	Scene::update(dt);
	ui->update(dt);

	pos.x -= (player->center().x - offset.x + pos.x) * 5 * dt;

	if (pos.x > 0)
		pos.x = 0;
	else if (pos.x < -220)
		pos.x = -220;


	if (pos.y > -13200 && !gm.boss_spawn[0])
	{
		if (!gm.boss_ready[0])
		{
			Particle* p = new Particle("image/UI/warning.png", FADE, 3, 2, true);
			ui->addChild(p);
			ui->fadeList.push_back(p);
			gm.boss_ready[0] = true;
		}
		player->pos.y -= player->speed * 2 * dt;
		if (gm.ingame->followTarget)
		{
		for (int i = 0; i < 3; i++)
		{
			gm.ingame->bg[i]->pos.y -= 300 * dt;
		}
		}
	}
	if (pos.y > -12000 && !gm.boss_spawn[0])
	{
		Boss* b;
		if (gm.stage == 1)
			b = new Shark();
		else
			b = new Crab();

		addChild(b);
		unitList.push_back(b);
		b->setCenter(Vec2(750, 11400));
		gm.boss_spawn[0] = true;
		unfollow();
		pos.y = -12000;
		gm.currentBoss = b;
		gm.boss_spawn[0] = true;
		ui->boss_hp_frame->visible = ui->boss_hp_bar->visible = true;
	}

	if (pos.y > -1200 && !gm.boss_spawn[1])
	{
		if (!gm.boss_ready[1])
		{
			Particle* p = new Particle("image/UI/warning.png", FADE, 3, 2, true);
			ui->addChild(p);
			ui->fadeList.push_back(p);
			gm.boss_ready[1] = true;
		}
		player->pos.y -= player->speed * 2 * dt;
		if (gm.ingame->followTarget)
		{
			for (int i = 0; i < 3; i++)
			{
				gm.ingame->bg[i]->pos.y -= 300 * dt;
			}
		}
	}
	if (pos.y == 0 && !gm.boss_spawn[1])
	{
		Boss* b;
		if (gm.stage == 1)
			b = new Angler();
		else
			b = new Pagurian();

		addChild(b);
		unitList.push_back(b);
		b->setCenter(Vec2(750, -600));
		gm.boss_spawn[1] = true;
		unfollow();
		pos.y = 0;
		gm.currentBoss = b;
		ui->boss_hp_frame->visible = ui->boss_hp_bar->visible = true;
	}

	if (stage_clear)
	{
		if (clearTimer == 0)
		{
			clear = new Particle("image/UI/clear.png", FADE, 1, 1, true);
			ui->addChild(clear);
			ui->fadeList.push_back(clear);
		}
		clearTimer += dt;
		if (clearTimer > 3)
		{
			if (gm.stage == 1)
			{
				gm.addon_count = 0;
				gm.stage++;
				world.changeScene(new StoreScene());
				return;
			}
			else
			{
				gm.stage = 1;
				world.changeScene(new InputScene());
				return;
			}
		}
	}


	if (gm.score > 99999)
		gm.score = 99999;
	if (gm.combo > 999)
		gm.combo = 999;


	for (auto i = itemList.begin(); i != itemList.end(); i++)
	{
		float move_angle = D3DXToRadian(90);

		if (distance((*i)->center(), player->center()) < 200)
		{
			move_angle = angle((*i)->center(), player->center());
			(*i)->pos += Vec2(cos(move_angle), sin(move_angle)) * 200 * dt;
		}
		(*i)->pos += Vec2(cos(move_angle), sin(move_angle)) * 200 * dt;

		if ((*i)->rectWithPos().intersects(player->hitbox.offset(player->pos)))
		{
			if ((*i)->type == I_HP)
			{
				player->hp += 30;
				if (player->hp > player->hp_origin) player->hp = player->hp_origin;
				for (int i = 0; i < 30; i++)
				{
					Particle* p = new Particle("image/Effect/heal.png", FADE, 1);
					addChild(p);
					particleList.push_back(p);
					p->pos = player->pos + Vec2(random(player->hitbox.left, player->hitbox.right), random(player->hitbox.top, player->hitbox.bottom));
					p->is_move = true;
					p->move_angle = random(0.f, 6.28f);
					p->move_speed = 100;
				}
			}
			else if ((*i)->type == I_ADDON)
			{
				if (addonList.size() < 4)
				{
					Addon* a = new Addon();
					addChild(a);
					addonList.push_back(a);
					int i = 0;
					gm.addon_count++;
					for (Addon* iter : addonList)
					{
						i++;
						iter->move_angle = D3DXToRadian(i * 360 / addonList.size());
					}
					a->pos = player->center() - a->rect.center() + Vec2(cos(a->move_angle), sin(a->move_angle)) * 150;
				}

			}
			else if ((*i)->type == I_TORPEDO)
			{
				player->torpedo_count++;
			}
			else if ((*i)->type == I_SHIELD)
			{
				player->shield_max_alpha = 1;
				player->is_shield = true;
				player->shield->visible = true;
			}
			else if ((*i)->type == I_COIN)
			{
				gm.coin += 5;
			}

			(*i)->deleting = true;
		}

		if ((*i)->deleting)
		{
			removeChild(*i);
			i = itemList.erase(i);
			if (i == itemList.end())
				break;
		}
	}

	unitList.erase(remove_if(unitList.begin(), unitList.end(),
		[&](Unit* u) {
		if (u->deleting)
			removeChild(u);
		return u->deleting;
	}), unitList.end());

	addonList.erase(remove_if(addonList.begin(), addonList.end(),
		[&](Addon* a) {
		if (a->deleting)
			removeChild(a);
		return a->deleting;
	}), addonList.end());

	bulletList.erase(remove_if(bulletList.begin(), bulletList.end(),
		[&](Bullet* b) {
		if (b->deleting)
			removeChild(b);
		return b->deleting;
	}), bulletList.end());

	effectList.erase(remove_if(effectList.begin(), effectList.end(),
		[&](Animation* a) {
		if (a->is_end)
			removeChild(a);
		return a->is_end;
	}), effectList.end());

	particleList.erase(remove_if(particleList.begin(), particleList.end(),
		[&](Particle* p) {
		if (p->deleting)
			removeChild(p);
		return p->deleting;
	}), particleList.end());

	if (world.getKeyState('1') == 1)
	{
		Item* i = new Item((ItemType)random(0, 4));
		addChild(i);
		itemList.push_back(i);
		i->pos = -pos + Vec2(500, 0);
	}
	else if (world.getKeyState('2') == 1)
	{
		Boss* b = new Shark();
		addChild(b);
		unitList.push_back(b);
		b->pos = -pos - Vec2(-300, 300);
	}
	else if (world.getKeyState('3') == 1)
	{
		Boss* b = new Angler();
		addChild(b);
		unitList.push_back(b);
		b->pos = -pos - Vec2(-300, 300);
	}
	else if (world.getKeyState('4') == 1)
	{
		Boss* b = new Crab();
		addChild(b);
		unitList.push_back(b);
		b->pos = -pos - Vec2(-300, 300);
	}
	else if (world.getKeyState('5') == 1)
	{
		Boss* b = new Pagurian();
		addChild(b);
		unitList.push_back(b);
		b->pos = -pos - Vec2(-300, 300);
	}
	else if (world.getKeyState('6') == 2)
	{
		stage_clear = true;
	}
}