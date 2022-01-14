#pragma once
#include "gameScene.h"

class player;

class mapManager: public gameScene
{
protected:
	player* pl;
	bool eventtrigger = false;

	vector <RECT> cO; // collision object 충돌용 옵젝
	vector <RECT> eR; // event 활성화 여부 확인해주는 렉트
	vector <RECT> exit; // 나가버리기
	POINT prevPlPos;

	POINT* currPlPos;
	POINT* lucPos;
	POINT* frogPos;
	POINT* marPos;

	RECT tempRct;
	vector<vector<tileInfo>> tile;

	int zGrid;

public:
	mapManager() 
	{ 
		/*probeX = 32;  probeY = 10; //x 탐지 범위 y 탐지 범위 통일화 */ // 이걸로도 값 정해두긴되네
	}
	~mapManager() override{}

	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	void zorderUpdate();

	
	void setTrigger(bool trigger) { eventtrigger = trigger; }
	void setLinkTo(player* p) { pl = p; }
	void mapCollision();

	void addXtile();
	void removeXtile();
	void addYtile();
	void removeYtile();
	void load();
	void load2(const char* _name);
	void tileMapLoad(string strKey, int width, int height);

};

