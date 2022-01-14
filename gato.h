#pragma once
#include "enemy.h"

class player;
class gato : public enemy
{
private:
	int _T;	//방향 전환(turn)		0 : front / 1 : back / 2 : right / 3 : left
	//bool _up, _down, _right, _left;
	int _Move;
	float _speed;

	player* _pl;
public:
	gato();
	~gato();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void imgSetting();
	void aniSetting();
	void imgSwitch();
	void battleSwitch();

	void setLinkPl(player* pl) { _pl = pl; }


};

