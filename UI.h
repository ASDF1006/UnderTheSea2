#pragma once
#include "Entity.h"
#include "Particle.h"

class UI :
	public Entity
{
public:
	UI();
	~UI();

	void update(float dt);

	Sprite* frame;
	Sprite* hp_bar;
	Sprite* boss_hp_bar;
	Sprite* boss_hp_frame;
	Sprite* torpedo_num[2];
	Sprite* coin_num[3];
	Sprite* addon_num;
	Sprite* score_num[5];
	Sprite* continue_scene;
	Sprite* continue_button[4];

	bool focus_yes;

	vector<Particle*>fadeList;
};

