#pragma once
#include "gameNode.h"

class videoPlayer;

class startScene : public gameNode
{
private:
	videoPlayer* vP;

public:
	startScene();
	~startScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

