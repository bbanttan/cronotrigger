#pragma once
#include"character.h"
#include "Astar.h"

class crono : public character
{
private:
	float a, b, rich;
	vector<POINT> Path;
public:
	crono() {};
	~crono() {};

	HRESULT init();
	void release();
	void update();
	void render();

	void imgSetting();
	void aniSetting();
	void imgSwitch();		//이미지 보이는 방식이 들어간다
	void keySetting();	//키 입력 방식
	//void moveSwitch();	//이동시 스위치(벽부딪치기 등)
	void battleSwitch();	//배틀시 스위치(공격시 렉트 등)

};

