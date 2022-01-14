#pragma once
#include "gameScene.h"
#include "mapManager.h"
#include "player.h"

class chronoHome : public mapManager
{
protected:
	
public:
	chronoHome() {}
	~chronoHome() override {}

	virtual HRESULT init() override;
	virtual void update() override;
	virtual void release() override;
	virtual void render() override;


};

