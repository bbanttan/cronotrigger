#pragma once
#include "gameScene.h"
#include "mapManager.h"
#include "player.h"

class chronoHome2 : public mapManager
{
protected:

public:
	chronoHome2() {}
	~chronoHome2() override {}

	int  event1;
	int  event2;

	HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

};
