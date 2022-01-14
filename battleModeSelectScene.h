#pragma once
#include "gameNode.h"

class battleModeSelectScene : public gameNode
{
	private:
	BOOL _isWaitSelected;

	public:
	battleModeSelectScene();
	~battleModeSelectScene() override;

	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

