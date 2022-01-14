#pragma once
#include "gameNode.h"

class player;
class item;

struct equip
{
	string weapon;
	string helmet;
	string armor;
	string acc;
};
enum sceneState
{
	category_select,
	category_in,
};

class playerUiScene : public gameNode
{
private:
	player* pl;
	POINT pos;
	item* _item;
	vector<item*> _vItem; //아이템창

	image* _chrFaceImg;
	image* _chrType;
	string playerName, playerLevel, playerWeapon, playerHelmet,
		playerArmor, playerAcc, itemCategory;

	int playerPow, playerSpd, playerHit, playerEv, playerMag, playerStam, playerMdef,
		playerExp, playerMaxExp,playerMoney, left, gun;

	string itemName;
	string level;
	equip p1E;
	equip p2E;
	equip p3E;
	RECT one1, one2;
	bool sT;
	int _num, characterNum;
	int select;
	int _categorySelcetPos, _cateIndex;
	int cst , csn; //csn 횟수세기
	int appear;
	sceneState cS;//categoryState
public:
	
	playerUiScene();
	~playerUiScene() override;

	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	void categoryList(int num);
	void setLinkTo(player* p) { pl = p; }
	void cateControl();
	void equipItemValues(int charNum, string Iname);
	void unEquipItemValuse(int charNum, string Iname);
};


