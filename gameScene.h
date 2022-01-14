#pragma once
#include "gameNode.h"
#include <memory>

constexpr int VIEWPORT_UPDATE_OFFSET = 2;

class mapManager;
class player;

class gameScene : public gameNode
{
private:
	static int _countForReEnablingKeyInput;
	static BOOL _mapChangeComplete;
	static player* _p;
	static vector<shared_ptr<mapManager>> _mapList;
	static shared_ptr<mapManager> _currMap;
	static int currPlPos;
	static int _prevMapNum, _mapNum;
protected:
	int probeX = 0;
	int probeY = 0;

public:
	gameScene() = default;
	gameScene(int anyNum);
	~gameScene() override;

	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	void updateViewport(int x, int y);
	void setViewport(int x, int y);

	static void setMapNum(int num) { _mapNum = num; } //맵
	static int getPrevMapNum() { return _prevMapNum; }
	static void goToMap(int num);
};


