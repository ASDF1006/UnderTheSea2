#include "DXUT.h"
#include "InputScene.h"
#include "World.h"
#include "RankingScene.h"


InputScene::InputScene()
{
	bg = new Sprite("image/Ranking/name.png");
	text = new Text("", 60, D3DXCOLOR(1, 1, 1, 1));
	text->setCenter(Vec2(700, 320));

	addChild(bg);
	addChild(text);
}


InputScene::~InputScene()
{
}


void InputScene::update(float dt)
{
	Scene::update(dt);

	if (world.getKeyState(VK_SPACE) == -1 || world.getKeyState(VK_RETURN) == -1) {
		if (text->text != "") {
			world.changeScene(new RankingScene(text->text));
			return;
		}
	}

	for (int i = 0; i < 256; i++) {
		if (world.getKeyState(i) == -1) {
			if (i == VK_BACK) {
				if (text->text.size() > 0)
					text->text.pop_back();
			}
			else if (text->text.size() > 5) {
				return;
			}
			else {
				text->text += i;
			}
			printf("%s\n", text->text.data());
		}
	}

	int text_width = text->text.size() * 17;
	text->setCenter(Vec2(640 - text_width, 320));
}