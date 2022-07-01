#pragma once
#include "Scene.h"
#include "Text.h"
#include "Sprite.h"

class InputScene :
	public Scene
{
public:
	InputScene();
	~InputScene();

	void update(float dt);

	Sprite* bg;
	Text* text;
};

