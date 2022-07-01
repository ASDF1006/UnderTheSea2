#pragma once
#include "Define.h"

class Timer
{
public:
	Timer();
	~Timer();
	void update(float dt);
	void reset(float duration, int count, bool autoStart = true);

	function<void()> onTick;
	function<void()> onFinished;

	pair<float, float> duration;
	pair<int, int> count;

	bool active;
};

