#include "stdafx.h"
#include "item.h"

HRESULT item::init(string _name)
{
	return S_OK;
}

void item::release()
{
}

void item::update()
{
}

void item::render()
{
}

void item::itemlist(string _name)
{
	itemName = _name;
	//equip 장비템
	if (itemName == "WoodSword")
	{
		iC = Equipment;
		eC = weapon;
		pC = Crono;
		attack = 3;
		sell = 50;
	}
	if (itemName == "IronBlade")
	{
		iC = Equipment;
		eC = weapon;
		pC = Crono;

		money = 350;
		sell = 175;

		attack = 7;
	}
	if (itemName == "SteelSaber")
	{
		iC = Equipment;
		eC = weapon;
		pC = Crono;

		money = 800;
		sell = 400;

		attack = 15;
	}
	if (itemName == "LodeSword")
	{
		iC = Equipment;
		eC = weapon;
		pC = Crono;

		money = 4000;
		sell = 2000;

		attack = 20;
	}
	if (itemName == "RedKatana")
	{
		iC = Equipment;
		eC = weapon;
		pC = Crono;

		money = 4500;
		sell = 2250;

		attack = 30;
		magic = 2;
	}
	//=======================================marle====
	if (itemName == "BronzeBow")
	{
		iC = Equipment;
		eC = weapon;
		pC = Marle;

		money = 0;
		sell = 40;

		attack = 3;
	}

	if (itemName == "IronBow")
	{
		iC = Equipment;
		eC = weapon;
		pC = Marle;

		money = 850;
		sell = 425;

		attack = 15;
	}

	if (itemName == "LodeBow")
	{
		iC = Equipment;
		eC = weapon;
		pC = Marle;

		money = 0;
		sell = 850;

		attack = 20;
	}

	if (itemName == "RobinBow")
	{
		iC = Equipment;
		eC = weapon;
		pC = Marle;

		money = 2850;
		sell = 1425;

		attack = 25;
	}

	//==============================Lucca

	if (itemName == "AirGun")
	{
		iC = Equipment;
		eC = weapon;
		pC = Lucca;

		money = 0;
		sell = 150;

		attack = 5;
	}

	if (itemName == "DartGun")
	{
		iC = Equipment;
		eC = weapon;
		pC = Lucca;

		money = 800;
		sell = 400;

		attack = 7;
	}

	if (itemName == "AutoGun")
	{
		iC = Equipment;
		eC = weapon;
		pC = Lucca;

		money = 1200;
		sell = 600;

		attack = 15;
	}

	if (itemName == "PlasmaGun")
	{
		iC = Equipment;
		eC = weapon;
		pC = Lucca;

		money = 3200;
		sell = 1600;

		attack = 25;
	}

	//============================Robo

	if (itemName == "TinArm")
	{
		iC = Equipment;
		eC = weapon;
		pC = Robo;

		money = 0;
		sell = 500;

		attack = 20;
	}

	if (itemName == "HameerArm")
	{
		iC = Equipment;
		eC = weapon;
		pC = Robo;

		money = 3500;
		sell = 1750;

		attack = 25;
	}

	if (itemName == "MirageHand")
	{
		iC = Equipment;
		eC = weapon;
		pC = Robo;

		money = 0;
		sell = 2750;

		attack = 30;
	}

	//========================================Frog===
	if (itemName == "BronzeEdge")
	{
		iC = Equipment;
		eC = weapon;
		pC = Frog;

		money = 0;
		sell = 175;

		attack = 6;
	}
	if (itemName == "IronSword")
	{
		iC = Equipment;
		eC = weapon;
		pC = Frog;

		money = 0;
		sell = 550;

		attack = 10;
	}
	if (itemName == "Masamune")
	{
		iC = Equipment;
		eC = weapon;
		pC = Frog;

		money = 0;
		sell = 0;

		attack = 75;
	}

	//================================Ayla===============
	if (itemName == "Fist")
	{
		iC = Equipment;
		eC = weapon;
		pC = Ayla;

		//아일라의 무기는 레벨에 따라 자동변환 1~71
	}
	if (itemName == "IronFist")
	{
		iC = Equipment;
		eC = weapon;
		pC = Ayla;

		//아일라의 무기는 레벨에 따라 자동변환 72~95
		//크리티컬 확률 증가
		//혼란 부여 확정
	}
	if (itemName == "BronzeFist")
	{
		iC = Equipment;
		eC = weapon;
		pC = Ayla;

		//아일라의 무기는 레벨에 따라 자동변환 96~99
		//최고 대미지 9999
	}
	//==================Magus ======================
	if (itemName == "DarkScythe")
	{
		iC = Equipment;
		eC = weapon;
		pC = Magus;

		money = 0;
		sell = 10000;

		attack = 120;
	}
	if (itemName == "Hurricane")
	{
		iC = Equipment;
		eC = weapon;
		pC = Magus;

		money = 35000;
		sell = 17500;

		attack = 135;
	}
	if (itemName == "StarScythe")
	{
		iC = Equipment;
		eC = weapon;
		pC = Magus;


		sell = 21000;

		attack = 150;
	}
	//===================================Armor
	if (itemName == "HideTunic")
	{
		iC = Equipment;
		eC = armor;
		pC = All;

		sell = 40;

		defense = 5;
	}
	if (itemName == "karateGi")
	{
		iC = Equipment;
		eC = armor;
		pC = All;

		money = 300;
		sell = 150;

		defense = 10;
	}
	if (itemName == "BronzeMail")
	{
		iC = Equipment;
		eC = armor;
		pC = Crono, Robo, Frog, Magus;

		money = 520;
		sell = 260;

		defense = 16;
	}
	if (itemName == "MaidenSuit")
	{
		iC = Equipment;
		eC = armor;
		pC = Marle, Lucca, Ayla;

		money = 560;
		sell = 280;

		defense = 18;
	}
	if (itemName == "IronSuit")
	{
		iC = Equipment;
		eC = armor;
		pC = All;

		money = 800;
		sell = 400;

		defense = 25;
	}
	if (itemName == "TitanVest")
	{
		iC = Equipment;
		eC = armor;
		pC = All;

		money = 1200;
		sell = 600;

		defense = 32;
	}

	if (itemName == "GoldSuit")
	{
		iC = Equipment;
		eC = armor;
		pC = All;

		money = 1300;
		sell = 650;

		defense = 39;
	}
	//=================helmet=============
	if (itemName == "hideCap")
	{
		iC = Equipment;
		eC = helmet;
		pC = All;

		money = 0;
		sell = 25;

		defense = 3;
	}

	if (itemName == "BronzeHelm")
	{
		iC = Equipment;
		eC = helmet;
		pC = All;

		money = 200;
		sell = 100;

		defense = 8;
	}

	if (itemName == "IronHelm")
	{
		iC = Equipment;
		eC = helmet;
		pC = All;

		money = 500;
		sell = 250;

		defense = 14;
	}
	if (itemName == "Beret")
	{
		iC = Equipment;
		eC = helmet;
		pC = Marle, Lucca, Ayla;

		money = 700;
		sell = 350;

		defense = 17;
	}
	if (itemName == "GoldHelm")
	{
		iC = Equipment;
		eC = helmet;
		pC = Crono, Robo, Frog, Magus;

		money = 0;
		sell = 600;

		defense = 18;
	}
	//==============================acce======
	if (itemName == "Bandana")
	{
		iC = Equipment;
		eC = accessory;
		pC = All;

		speed = 1;

		itemTip = "Speed + 1";
	}
	if (itemName == "Ribbon")
	{
		iC = Equipment;
		eC = accessory;
		pC = All;

		hit = 2;

		itemTip = "Hit + 1";
	}
	if (itemName == "PowerGlove")
	{
		iC = Equipment;
		eC = accessory;
		pC = All;

		power = 2;

		itemTip = "power + 2";
	}
	if (itemName == "Amulet")
	{
		iC = Equipment;
		eC = accessory;
		pC = All;
		//모든 상태이상 면역

		itemTip = "prevents all status debuff";
	}


	//Expendable 소모템
	if (itemName == "Tonic")
	{
		iC = Expendable;
		money = 10;
		sell = 5;
		recoveryHp = 50;
		itemTip = "Restores 50 HP";
	}
	if (itemName == "Mid Tonic")
	{
		iC = Expendable;
		money = 100;
		sell = 50;
		recoveryHp = 200;
		itemTip = "Restores 200HP";
	}
	if (itemName == "Full Tonic")
	{
		iC = Expendable;
		money = 700;
		sell = 350;
		recoveryHp = 500;
		itemTip = "Restores 200HP";
	}
	if (itemName == "Ether")
	{
		iC = Expendable;
		money = 800;
		sell = 400;
		recoveryMp = 10;
		itemTip = "Restores 10MP";
	}
	if (itemName == "Mid Ether")
	{
		iC = Expendable;
		money = 2000;
		sell = 1000;
		recoveryMp = 30;
		itemTip = "Restores 30MP";
	}
	if (itemName == "Full Ether")
	{
		iC = Expendable;
		money = 6000;
		sell = 3000;
		recoveryMp = 60;
		itemTip = "Restores 60MP";
	}
	if (itemName == "Elixer")
	{
		iC = Expendable;
		money = 800;
		sell = 400;
		recoveryHp = maxHp;
		recoveryMp = maxMp;
		itemTip = "Restores Full Mp,Hp";
	}
	if (itemName == "powerTab")
	{
		iC = Expendable;
		power = 1;
		sell = 5;
		itemTip = "Power + 1";
	}
	if (itemName == "magicTab")
	{
		iC = Expendable;
		magic = 1;
		sell = 5;
		itemTip = "Magic + 1";
	}
	if (itemName == "speedTab")
	{
		iC = Expendable;
		speed = 1;
		sell = 5;
		itemTip = "speed + 1";
	}

	//etc 기타템및 evet템

	if (itemName == "Pendant")
	{
		iC = Event;
		//퀘스트용 팬던트
		itemTip = "give to Marle";
	}
	if (itemName == "SpecialMeal")
	{
		// 이벤트용 피 다채워주는 음식
		iC = Event;
	}


}

