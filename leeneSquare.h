#pragma once

//펜던트를 찾아주는 이벤트, 종

#include "gameScene.h"
#include "mapManager.h"
#include "player.h"

class leeneSquare: public mapManager
{
protected:

public:
	leeneSquare() {}
	~leeneSquare() override {}

	HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;
};

