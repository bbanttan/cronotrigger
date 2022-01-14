#pragma once
#include "gamenode.h"

class exampleitem
{
private:
	string itemName;
	int attack;			//장비 공격력
	int defense;		//장비 방어력
	int power;			//물리공격데미지관련	
	int magic;			//마법공격데미지관련	
	int speed;			//속도타이먼관련

	int hit;			//명중률
	int evasion;		//회피율	

	int magicDefense;	//마법방어관련

	int recoveryHp;		//체력회복
	int maxHp;			//최대체력증가
	int recoveryMp;		//마나회복
	int maxMp;			//최대마나증가
	int money;			//가격
public:

	HRESULT init(string _name);
	void release();
	void update();
	void render();
	void itemList(string name);

	string getName() { return itemName; }
	int getMoney() { return money; }
	int getPower() { return power; }
	int getMagic() { return magic; }
	int getSpeed() { return speed; }

	int getHit() { return hit; }
	int getEvasion(){return evasion;}

	int getStamina() { return stamina; }
	int getMagicDefense() { return magicDefense; }

	int getRecoveryHp() { return recoveryHp; }
	int getMaxHp() { return maxHp; }
	int getRecoveryMp() { return recoveryMp; }
	int getMaxMp() { return maxMp; }
	int getMoney() { return money; }
};

