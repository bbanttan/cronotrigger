#pragma once
#include "gameNode.h"
enum equipCategory // 장비 카테고리 equipCategory
{
	weapon,
	helmet,
	armor,
	accessory
};
enum playerCategory
{
	Crono,
	Marle,
	Lucca,
	Robo,
	Frog,
	Ayla,
	Magus,
	All
};
enum category
{
	Expendable,			//0 소모품
	Equipment,			//1 장비품
	Etc,				//2 기타
	Event				//3 이벤트용
};

class item : public gameNode
{
protected:
	equipCategory eC;
	playerCategory pC;
	category iC;
	string itemName;
	string itemTip;
	vector<item*> _vItem;
	int attack;			//장비공격력
	int defense;		//장비 방어력
	int power;			//물리공격데미지관련	
	int magic;			//마법공격데미지관련	
	int speed;			//속도타이먼관련
			
	int hit;			//명중률
	int evasion;		//회피율	

	int stamina;		//물리방어관련
	int magicDefense;	//마법방어관련

	int recoveryHp;		//체력회복
	int maxHp;			//최대체력증가
	int recoveryMp;		//마나회복
	int maxMp;			//최대마나증가
	int sell;			//판매 가치
	int money;			//가격
public:
	virtual HRESULT init(string _name);
	virtual void release();
	virtual void update();
	virtual void render();
	void itemlist(string _name);


	string getName() { return itemName; }
	int getMoney() { return money; }
	int getPower() { return power; }
	int getMagic() { return magic; }
	int getSpeed() { return speed; }
	int getHit() { return hit; }
	int getEvasion() { return evasion; }
	int getStamina() { return stamina; }
	int getMagicDefense() { return magicDefense; }
	int getRecoveryHp() { return recoveryHp; }
	int getMaxHp() { return maxHp; }
	int getRecoveryMp() { return recoveryMp; }
	int getMaxMp() { return maxMp; }
	int getCategory() { return iC; }
	int getEquipCategory() { return eC; }
	
};

