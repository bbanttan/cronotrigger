#include "stdafx.h"
#include "playerUiScene.h"
#include "gameScene.h"
#include "player.h"
#include "item.h"

#define T 64
#define E 48
playerUiScene::playerUiScene() 
{

}

playerUiScene::~playerUiScene()
{
}

HRESULT playerUiScene::init()
{
	_item = new item;
	_item->itemlist("WoodSword");
	_vItem.push_back(_item);

	_item = new item;
	_item->itemlist("HideTunic");
	_vItem.push_back(_item);

	_item = new item;
	_item->itemlist("hideCap");
	_vItem.push_back(_item);

	_item = new item;
	_item->itemlist("Ribbon");
	_vItem.push_back(_item);

	_shouldRenderUsingWindowCoords = FALSE;
	
	one1 = MakeRct(64, 64, 64 * 6, 64 * 3);
	pos = { 64, WINH - 64 };
	sT = false;
	appear = 0, left = 0, gun = (2 * T) + (4 * E);
	cS = category_select;
	csn = 0;//categorysettingnumber
	_num = 0, characterNum = 0;
	select = 0; 
	//_num =
	playerMoney = pl->getCrono()->getMoney();
	switch (characterNum)
	{
	case 0:
		_chrFaceImg = IMG->find("Crono");
		playerName = _playChrNames[0];
		p1E.weapon = "Wood Sword";
		p1E.helmet = "Hide Cap";
		p1E.armor = "Hide Tunic";
		p1E.acc = "Bandana";
		playerWeapon = p1E.weapon;
		playerHelmet = p1E.helmet;
		playerArmor = p1E.armor;
		playerAcc = p1E.acc;
		playerLevel = to_string(pl->getCrono()->getLV());
		playerPow = pl->getCrono()->getPower();
		playerSpd = pl->getCrono()->getSpeed();
		playerHit = pl->getCrono()->getHit();
		playerEv = pl->getCrono()->getEvasion();
		playerMag = pl->getCrono()->getMagic();
		playerStam = pl->getCrono()->getStamina();
		playerMdef = 2; //magic.Defence
		playerExp = pl->getCrono()->getExp();
		playerMaxExp = pl->getCrono()->getMaxExp();
		
	break;
	case 1:
		_chrFaceImg = IMG->find("Lucca");
		playerName = _playChrNames[1];
		playerWeapon = p2E.weapon;
		playerHelmet = p2E.helmet;
		playerArmor = p2E.armor;
		playerAcc = p2E.acc;
		playerLevel = to_string(pl->getLucca()->getLV());
		playerPow = pl->getLucca()->getPower();
		playerSpd = pl->getLucca()->getSpeed();
		playerHit = pl->getLucca()->getHit();
		playerEv = pl->getLucca()->getEvasion();
		playerMag = pl->getLucca()->getMagic();
		playerStam = pl->getLucca()->getStamina();
		playerMdef = 2; //magic.Defence
		playerExp = pl->getCrono()->getExp();
		playerMaxExp = pl->getLucca()->getMaxExp();
	break;
	case 2:
		_chrFaceImg = IMG->find("Frog");
		playerName = _playChrNames[3];
		playerWeapon = p3E.weapon;
		playerHelmet = p3E.helmet;
		playerArmor = p3E.armor;
		playerAcc = p3E.acc;
		playerLevel = to_string(pl->getFrog()->getLV());
		playerPow = pl->getFrog()->getPower();
		playerSpd = pl->getFrog()->getSpeed();
		playerHit = pl->getFrog()->getHit();
		playerEv = pl->getFrog()->getEvasion();
		playerMag = pl->getFrog()->getMagic();
		playerStam = pl->getFrog()->getStamina();
		playerMdef = 2; //magic.Defence
		playerExp = pl->getFrog()->getExp();
		playerMaxExp = pl->getFrog()->getMaxExp();
	break;
	}
	
	return S_OK;
}

void playerUiScene::release()
{
	for (size_t i = 0; i < _vItem.size(); ++i) SAFE_DEL(_vItem[i]);
	_vItem.clear();
}

void playerUiScene::update()
{
	
	if (KEY->down('D'))
	{
		SC->changeScene("게임 장면");
	}
	switch (select)
	{
	case 0:
		itemCategory = "Weapon";
		break;

	case 1:
		itemCategory = "Helmet";
		break;

	case 2:
		itemCategory = "Armor";
		break;

	case 3:
		itemCategory = "Acc";
		break;
	}
	cateControl();
	
}

void playerUiScene::render()
{
	PatBlt(getMemDC(), 0, 0, WINW, WINH, BLACKNESS);
	_shouldRenderUsingWindowCoords = TRUE;
	IMG->render("스텟창 뒷 배경", getMemDC(), 0, 0);
	categoryList(csn);
	if (cS == category_select)
	{
		
		IMG->render("카테고리선택", getMemDC(), 256, 384);
		if(csn == 0)IMG->render("카테고리아이콘", getMemDC(), 320, 448, 0, T, 64, 64);
		else IMG->render("카테고리아이콘", getMemDC(), 320, 448, 0, 0, 64, 64);
		if (csn == 1)IMG->render("카테고리아이콘", getMemDC(), 320 + T, 448, T, T, 64, 64);
		else IMG->render("카테고리아이콘", getMemDC(), 320 + T, 448, T, 0, 64, 64);
		if(csn == 2)IMG->render("카테고리아이콘", getMemDC(), 320 + (2 * T), 448, 2 * T, T, 64, 64);
		else IMG->render("카테고리아이콘", getMemDC(), 320 + (2 * T), 448, 2 * T, 0, 64, 64);
		if(csn == 3)IMG->render("카테고리아이콘", getMemDC(), 320 + (3 * T), 448, 3 * T, T, 64, 64);
		else IMG->render("카테고리아이콘", getMemDC(), 320 + (3 * T), 448, 3 * T, 0, 64, 64);
		if(csn == 4 )IMG->render("카테고리아이콘", getMemDC(), 320 + (4 * T), 448, 4 * T, T, 64, 64);
		else IMG->render("카테고리아이콘", getMemDC(), 320 + (4 * T), 448, 4 * T, 0, 64, 64);
		if(csn == 5)IMG->render("카테고리아이콘", getMemDC(), 320 + (5 * T), 448, 5 * T, T, 64, 64);
		else IMG->render("카테고리아이콘", getMemDC(), 320 + (5 * T), 448, 5 * T, 0, 64, 64);
		
		
		IMG->frameRender("위치 표시 타일셋", getMemDC(), 5 * T + (csn*T), 6 * T, 2, 1);
		IMG->frameRender("위치 표시 타일셋", getMemDC(), 5 * T + (csn*T), 8 * T, 2, 0);
	}
	_shouldRenderUsingWindowCoords = FALSE;
}

void playerUiScene::categoryList(int num)
{
	switch (num)
	{
	case 0:		//케릭터창
		_textWindowSkin = 1;
		IMG->setWindowSkin("글 출력 창 스킨 타일셋", "캐릭터창", 384, 192);//캐릭터 hp mp
		IMG->setWindowSkin("글 출력 창 스킨 타일셋", "장비창", 384, 128); //아이템 장비 카테고리 출력
		IMG->setWindowSkin("글 출력 창 스킨 타일셋", "목록", 384, 448);
		IMG->setWindowSkin("글 출력 창 스킨 타일셋", "0번 스텟창 캐릭터", 512, 768);
		IMG->render("캐릭터창", getMemDC(), T, T);
		IMG->render("캐릭터창", getMemDC(), T, 256);
		IMG->render("캐릭터창", getMemDC(), T, 448);
		IMG->render("캐릭터창", getMemDC(), T, 640);
		IMG->render("0번 스텟창 캐릭터", getMemDC(), 7 * T, T);
		
		TXT->render(getMemDC(), _playChrNames[0] + " LV \t " + to_string(pl->getCrono()->getLV()), T + T / 2, T + T / 2, 0, 0);
		TXT->render(getMemDC(), "HP \t" + to_string(pl->getCrono()->getHP()) +"/ \t" + to_string(pl->getCrono()->getMaxHP()), T + T / 2, T + (T/2) + E, 0, 0);
		TXT->render(getMemDC(), "MP \t" + to_string(pl->getCrono()->getMP()) + "/ \t" + to_string(pl->getCrono()->getMaxMP()), T + T / 2, T + (T / 2) + 2 *E, 0, 0);
		//===========================
		_chrFaceImg->render(getMemDC(), 8 * T - T/2, T + T / 2);
		if (characterNum == 0)IMG->render("속성",getMemDC(), 11 * T, T + T / 2, 0, 0, 192, 64);
		if (characterNum == 1)IMG->render("속성", getMemDC(), 11 * T, T + T / 2, 0, T, 192, 64);
		if (characterNum == 2)IMG->render("속성", getMemDC(), 11 * T, T + T / 2, 0, 2*T, 192, 64);
		TXT->render(getMemDC(), playerName, 11 * T, (2*T) +E , 0, 0);
		TXT->render(getMemDC(), "LEVEL  " + playerLevel, 11 * T, (2 * T) + (2*E) + (T/4), 0, 0);
		
		IMG->frameRender("아이템아이콘", getMemDC(), 8 * T +(T/2), (2 * T) + (4 * E), 0, 0);
		TXT->render(getMemDC(), playerWeapon , 9 * T, (2 * T) + (4 * E) , 0, 0);

		IMG->frameRender("아이템아이콘", getMemDC(), 8 * T + (T / 2), (2 * T) + (5 * E) , 7, 0);
		TXT->render(getMemDC(), playerHelmet, 9 * T, (2 * T) + (5 * E), 0, 0);

		IMG->frameRender("아이템아이콘", getMemDC(), 8 * T + (T / 2), (2 * T) + (6 * E) , 6, 0);
		TXT->render(getMemDC(), playerArmor, 9 * T, (2 * T) + (6 * E) , 0, 0);

		IMG->frameRender("아이템아이콘", getMemDC(), 8 * T + (T / 2), (2 * T) + (7 * E), 8, 0);
		TXT->render(getMemDC(), playerAcc, 9 * T, (2 * T) + (7 * E) , 0, 0);

		TXT->render(getMemDC(), "PWR. " + to_string(playerPow), 8 * T - T / 2, (2 * T) + (8 * E));
		TXT->render(getMemDC(), "SPD. " + to_string(playerSpd), 12 * T - E, (2 * T) + (8 * E) );
		TXT->render(getMemDC(), "HIT. " + to_string(playerHit), 8 * T - T / 2, (2 * T) + (9 * E) );
		TXT->render(getMemDC(), "EV.   " + to_string(playerEv), 12 * T - E, (2 * T) + (9 * E));
		TXT->render(getMemDC(), "MAG. " + to_string(playerMag), 8 * T - T / 2, (2 * T) + (10 * E) );
		TXT->render(getMemDC(), "STAM. " + to_string(playerStam), 12 * T - E, (2 * T) + (10 * E));
		TXT->render(getMemDC(), "M. DEF. \t  " + to_string(playerMdef), 8 * T - T / 2, (2 * T) + (11 * E));
		TXT->render(getMemDC(), "EXP \t\t\t" + to_string(playerExp), 8 * T - T / 2, (2 * T) + (12 * E) +16);
		TXT->render(getMemDC(), "NEXT \t\t  " + to_string(playerMaxExp), 8 * T - T / 2, (2 * T) + (13 * E)+16);
		if(left == 1)
		{
		IMG->frameRender("지건", getMemDC(), ( T/ 2) ,2 * T, 0, 0);
		}
		
		if (sT == false)
		{
			TXT->render(getMemDC(), "TIME", 2 * T - T / 2, (2 * T) + (12 * E));
			TXT->render(getMemDC(), to_string(playerMoney) + "G", 6 * T - T / 2, (2 * T) + (13 * E) + 16);

		}
		if (appear >= 1)
		{
			
			IMG->render("장비창", getMemDC(), pos.x, pos.y);
			IMG->render("목록", getMemDC(), pos.x, pos.y + 128);
			TXT->render(getMemDC(), itemCategory, pos.x+(T)+E, pos.y+(T));
			
			
			for (int i = 0; i < _vItem.size(); i++)
			{
				if (_vItem[i]->getCategory() == 1) // 카테고리 1 번 즉 장비품일떄일때 텍스트출력
				{
					if (select == 0)
					{
						if (_vItem[i]->getEquipCategory() == 0)
						{
							TXT->render(getMemDC(), _vItem[i]->getName(), pos.x + (T), pos.y + 160 + (i*T));
						}
					}
				}
			}
			for (int i = 0; i < _vItem.size(); i++)
			{
				if (_vItem[i]->getCategory() == 1) // 카테고리 1 번 즉 장비품일떄일때 텍스트출력
				{
					if (select == 1)
					{
						if (_vItem[i]->getEquipCategory() == 1)
						{
							TXT->render(getMemDC(), _vItem[i]->getName(), pos.x + (T), pos.y + 160);
						}
					}
				}
			}
			for (int i = 0; i < _vItem.size(); i++)
			{
				if (_vItem[i]->getCategory() == 1) // 카테고리 1 번 즉 장비품일떄일때 텍스트출력
				{
					if (select == 2)
					{
						if (_vItem[i]->getEquipCategory() == 2)
						{
							TXT->render(getMemDC(), _vItem[i]->getName(), pos.x + (T), pos.y + 160 );
						}
					}
				}
			}
			for (int i = 0; i < _vItem.size(); i++)
			{
				if (_vItem[i]->getCategory() == 1) // 카테고리 1 번 즉 장비품일떄일때 텍스트출력
				{
					if (select == 3)
					{
						if (_vItem[i]->getEquipCategory() == 3)
						{
							TXT->render(getMemDC(), _vItem[i]->getName(), pos.x + (T), pos.y + 160);
						}
					}
				}		
			}
			IMG->frameRender("지건", getMemDC(), (8 * T) - E, gun, 0, 0);
		}
		IMG->render("가림막", getMemDC(), 0, WINH - 64);
	break;

	case 1:		//아이템창
		IMG->render("아이템창1", getMemDC(), T, T);
		IMG->render("아이템창1", getMemDC(), T, 3*T);
		IMG->render("소지아이템창", getMemDC(),T, 5 * T);
		for (int i = 0; i < _vItem.size(); i++)
		{
			TXT->render(getMemDC(), _vItem[i]->getName(), T + (T / 4), (5 * T) + (T / 4) + (i * T));
		}
	break;

	case 2:		//스킬창
		IMG->render("스킬창1", getMemDC(), T, T);
		IMG->render("스킬창2", getMemDC(), 8*T, T);
		IMG->render("스킬창3", getMemDC(), 8 * T, 3 * T);
		IMG->render("스킬창4", getMemDC(), T, 10*T);
	break;

	case 3:		//설정창
		IMG->render("설정창1", getMemDC(), T, T);
		IMG->render("설정창2", getMemDC(), 7 * T, T);
		IMG->render("설정창3", getMemDC(), T, 10 * T);
	break;

	case 4:		//선두전환창
		IMG->render("위치창", getMemDC(), 4*T, T);
		IMG->render("위치창", getMemDC(), 4 * T, 4 * T);
		IMG->render("위치창", getMemDC(), 4 * T, 7 * T);
		IMG->render("위치창", getMemDC(), 4 * T, 10 *T);
	break;

	case 5:		//세이브로드
		IMG->render("세이브번호", getMemDC(), T, T);
		IMG->render("세이브번호", getMemDC(), T, 3 *T);
		IMG->render("세이브번호", getMemDC(), T, 5 * T);
		IMG->render("세이브창", getMemDC(), 3 * T, T);
		IMG->render("세이브창", getMemDC(), 3 * T, 3 * T);
		IMG->render("세이브창", getMemDC(), 3 * T, 5 * T);
		IMG->render("내용물1", getMemDC(), T, 8 * T);
		IMG->render("내용물2", getMemDC(), 7* T, 8 * T);
	break;
	}
}

void playerUiScene::cateControl()
{
	
	if (cS == category_select)
	{
		if (KEY->down(VK_RIGHT) && (csn <= 4))
		{
			csn += 1;
		}
		if (KEY->down(VK_LEFT) && (csn != 0))
		{
			csn -= 1;
		}

		if (KEY->down('C'))
		{
			
			SC->changeScene("게임 장면");
			
		}
		if (KEY->down('V'))
		{
			cS = category_in;
			left = 1;
		}
	}
	//=========================================================
	else if ((cS == category_in) && (cst == 0))
	{
		
		if ((left == 1) && (KEY->down('C'))) cS = category_select;
		if ((cS != category_select) && (KEY->down('V') || KEY->down(VK_RIGHT)))
		{
			sT = true;
			left = 0;
			appear = 1;
		}
		if ((cS != category_select) && (KEY->down('C') || (KEY->down(VK_LEFT))))
		{
			sT = false;
		}

		if (sT == false)
		{
			if (!(pos.y >= WINH - 64)) pos.y += 16;
			else if (pos.y + 16 >= WINH - 64)									
			{
				appear = 0;
				pos.y = WINH - 64;
				left = 1;
			}
		}
		if (sT == true)
		{
			if (!(PtInRect(&one1, pos))) pos.y -= 10;
			else if (PtInRect(&one1, pos) && (pos.y + 16 <= 64 * 4)) pos.y = 64 * 4;

		}
		
		if ((appear == 1) && (KEY->down(VK_DOWN)) && select != 3)
		{
			select += 1;
			gun += E;
		}
		if ((appear == 1) && (KEY->down(VK_UP)) && select != 0)
		{
			select -= 1;
			gun -= E;
		}
	}
	//=========================
	
}


void playerUiScene::equipItemValues(int charNum, string Iname)
{
	item* it;
	it = new item;
	it->itemlist(Iname);
	if(charNum == 0)
	{
		playerPow += it->getPower(); 
		playerSpd += it->getSpeed();
		playerHit += it->getHit();
		playerEv += it->getEvasion();
		playerMag += it->getMagic();
		playerStam += it->getStamina();
		playerMdef += it->getMagicDefense();
		playerExp += 0;
		playerMaxExp += 0;
	}
	delete it;
}

void playerUiScene::unEquipItemValuse(int charNum, string Iname)
{
	item* it;
	it = new item;
	it->itemlist(Iname);
	if (charNum == 0)
	{
		playerPow -= it->getPower();
		playerSpd -= it->getSpeed();
		playerHit -= it->getHit();
		playerEv -= it->getEvasion();
		playerMag -= it->getMagic();
		playerStam -= it->getStamina();
		playerMdef -= it->getMagicDefense();
		playerExp -= 0;
		playerMaxExp -= 0;
	}
	delete it;
}
