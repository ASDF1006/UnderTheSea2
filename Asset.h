#pragma once
#include "Texture.h"

class Asset
{
public:
	Asset();
	~Asset();

	void init();
	void dispose();
	void loadNext();
	void ready(string rootFolderPath);
	Texture* getTexture(string p);
	CSound* getSound(wstring p);

	map<string, Texture*> textures;
	map<wstring, CSound*> sounds;
	CSoundManager _s;

	vector<string> paths;
	vector<string>::iterator pathIter;

	int fileLoaded;
	int fileToLoad;
};


extern Asset asset;