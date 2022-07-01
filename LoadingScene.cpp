#include "DXUT.h"
#include "LoadingScene.h"
#include "World.h"
#include "IntroScene.h"
#include "Asset.h"


LoadingScene::LoadingScene()
{
	bg = new Sprite("image/Loading/bg.png");
	front = new Sprite("image/Loading/front.png");
	loading_frame = new Sprite("image/Loading/bar1.png");
	loading_bar = new Sprite("image/Loading/bar2.png");
	ani = new Animation("image/Loading/ani/", 21, 10, 1);

	addChild(bg);
	addChild(ani);
	addChild(front);
	addChild(loading_frame);
	addChild(loading_bar);

	loading_bar->pos = Vec2(271, 602);

	asset.ready("image/");
	asset.getSound(L"sound/ingameBGM.wav");
}


LoadingScene::~LoadingScene()
{
}


void LoadingScene::update(float dt)
{
	Scene::update(dt);

	for (int i = 0; i < 10; i++)
	{
		if (asset.fileLoaded == asset.fileToLoad)
		{
			world.changeScene(new IntroScene());
			return;
		}
		asset.loadNext();
		loading_bar->visibleRect.right = (float)asset.fileLoaded / (float)asset.fileToLoad * loading_bar->rect.right;
	}
}