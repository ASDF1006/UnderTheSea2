#include "DXUT.h"
#include "RankingScene.h"
#include "World.h"
#include "MainScene.h"
#include "GameManager.h"


RankingScene::RankingScene(string name)
{
	addChild(new Sprite("image/Ranking/ranking.png"));
	//addChild(new Sprite());

	ifs = ifstream("image/data.txt");
	ofs = ofstream("image/data.txt", ios::app | ios::end);

	for (int i = 0; i < 5; i++) {
		Name[i] = new Text("", 40, D3DXCOLOR(1, 1, 1, 1));
		Name[i]->pos = Vec2(330, 215 + 83 * i);
		addChild(Name[i]);
		Score[i] = new Text("", 40, D3DXCOLOR(1, 1, 1, 1));
		Score[i]->pos = Vec2(800, 215 + 83 * i);
		addChild(Score[i]);
	}

	if (name != "") {
		ofs << name << " " << (int)gm.score << endl;
	}
	ofs.close();

	Rank r;
	while (ifs >> r.name >> r.score) {
		ranks.push_back(r);
	}

	ranks.sort([](Rank r1, Rank r2) {
		return r1.score > r2.score;
	});

	int i = 0;
	for (auto iter = ranks.begin(); iter != ranks.end(); iter++)
	{
		Name[i]->text = (*iter).name;
		Score[i]->text = to_string((*iter).score);
		if (++i == 5) break;
	}
}


RankingScene::~RankingScene()
{
}

void RankingScene::update(float dt)
{
	Scene::update(dt);

	if (world.getKeyState(VK_SPACE) == -1 || world.getKeyState(VK_RETURN) == -1) {
		world.changeScene(new MainScene());
		return;
	}
}