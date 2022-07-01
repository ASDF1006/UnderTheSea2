#pragma once
#include "DXUT.h"
#include "Enum.h"


#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


using namespace std;
using namespace std::filesystem;

using Vec2 = D3DXVECTOR2;


class Rect : public RECT
{
public:
	Rect()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	Rect(int l, int t, int r, int b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}

	bool intersects(Rect r)
	{
		Rect tmp;
		return IntersectRect(&tmp, this, &r);
	}

	bool contain(Vec2 pos)
	{
		return left < pos.x && pos.x < right && top < pos.y && pos.y < bottom;
	}

	int width()
	{
		return right - left;
	}

	int height()
	{
		return bottom - top;
	}

	Vec2 center()
	{
		return Vec2(width(), height()) / 2;
	}

	Rect offset(Vec2 pos)
	{
		Rect r = *this;
		r.left += pos.x;
		r.right += pos.x;
		r.top += pos.y;
		r.bottom += pos.y;
		return r;
	}
};


inline float angle(Vec2 p1, Vec2 p2)
{
	return atan2(p2.y - p1.y, p2.x - p1.x);
}


inline int random(int from, int to)
{
	static default_random_engine random(timeGetTime());
	uniform_int_distribution<int> d(from, to);
	return d(random);
}


inline float random(float from, float to)
{
	static default_random_engine random(timeGetTime());
	uniform_real_distribution<float> d(from, to);
	return d(random);
}


inline float distance(Vec2 p1, Vec2 p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}