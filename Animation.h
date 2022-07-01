#pragma once
#include "Sprite.h"

class Animation :
	public Sprite
{
public:
	Animation(const string& path, int frameNum, float fps, bool loop = false, bool png = false);
	~Animation();

	void update(float dt);

	vector<Texture*> textures;
	float currentFrame;
	float fps;
	bool loop;
	bool is_end;
};

