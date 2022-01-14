#include "stdafx.h"
#include "exampleitem.h"
#include "character.h"

HRESULT exampleitem::init(string _name)
{
	string itemName;
	int attack			= 0;		//장비 공격력
	int defense			= 0;		//장비 방어력
	int power			= 0;		//물리공격데미지관련	
	int magic			= 0;		//마법공격데미지관련	
	int speed			= 0;		//속도타이먼관련

	int hit				= 0;		//명중률
	int evasion			= 0;		//회피율	
	int magicDefense	= 0;		//마법방어관련

	int recoveryHp		= 0;		//체력회복
	int maxHp			= 0;		//최대체력증가
	int recoveryMp		= 0;		//마나회복
	int maxMp			= 0;		//최대마나증가
	int money			= 0;		//가격

	itemList(_name);
	return S_OK;
}

void exampleitem::release()
{

}

void exampleitem::update()
{

}

void exampleitem::render()
{

}

void exampleitem::itemList(string name)
{
	if (name == "WoodenSword")
	{
		attack= 3;
	}
	if (name == "BasicArmor")
	{
		defense = 2;
	}
	if (name == "Bandana")
	{
		speed = 1;
	}
}
