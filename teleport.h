#pragma once

//텔레포트 이벤트

#include "gameScene.h"
#include "mapManager.h"
#include "player.h"

class teleport: public mapManager
{
protected:

public:
	teleport() {}
	~teleport() override {}

	HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

};

