#include "DXUT.h"
#include "Animation.h"
#include "Asset.h"


Animation::Animation(const string& path, int frameNum, float fps, bool loop, bool png)
	:fps(fps), currentFrame(0), loop(loop), is_end(false)
{
	for (int i = 0; i < frameNum; i++)
	{
		Texture* t = asset.getTexture(path + to_string(i) + (png ? ".png" : ".dds"));
		textures.push_back(t);
	}
	texture = textures[0];

	visibleRect = rect = Rect(0, 0, texture->info.Width, texture->info.Height);
}


Animation::~Animation()
{
}


void Animation::update(float dt)
{
	Sprite::update(dt);

	currentFrame += fps * dt;


	if ((int)currentFrame >= textures.size())
	{
		if (loop)
			currentFrame = 0;
		else
		{
			currentFrame = textures.size() - 1;
			is_end = true;
		}
	}
	else if (currentFrame <= 0)
	{
		if (loop)
			currentFrame = textures.size() - 1;
		else
		{
			currentFrame = 0;
			is_end = true;
		}
	}

	texture = textures[(int)currentFrame];
}
