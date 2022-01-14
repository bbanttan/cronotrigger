#pragma once
#include "singleton.h"

#include <map>

class effect;

class effectManager : public singleton<effectManager>
{
private:
	typedef vector<effect*> sameEffects;
	typedef vector<effect*>::iterator sameEffectsIter;

	typedef map<string, sameEffects> effectGroups;
	typedef map<string, sameEffects>::iterator effectGroupsIter;

	typedef vector<map<string, sameEffects>> totalEffects;
	typedef vector<map<string, sameEffects>>::iterator totalEffectsIter;

private:
	totalEffects _totEffects;

public:
	effectManager();
	~effectManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void add(string effectName, const char* imageName, int imageWidth, int imageHeight,
		int effectWidth, int effectHeight, int fps, float elapsedTime, int buffer);

	void play(string effectName, int x, int y);
	void killAll();
};

