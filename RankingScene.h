#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Text.h"

struct Rank {
	string name;
	int score;
};

class RankingScene :
	public Scene
{
public:
	RankingScene(string name = "");
	~RankingScene();

	void update(float dt);

	Text* Name[5];
	Text* Score[5];

	ifstream ifs;
	ofstream ofs;

	list<Rank>ranks;

	int score;
};