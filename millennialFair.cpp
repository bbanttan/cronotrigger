#include "stdafx.h"
#include "millennialFair.h"	//메인광장
#include "player.h"
#include "characterNamingScene.h"

millennialFair::millennialFair(): event1(0), event2(0)
{
}

HRESULT millennialFair::init()
{
	load2("res/mapInfo/린광장/saveMap1.map");
	currPlPos = &pl->getCrono()->getPos();
	if (getPrevMapNum() == 6) *currPlPos = { 1560, 1733 };
	else if (getPrevMapNum() == 3)
	{
		*currPlPos = { 70, 432 };
		pl->getCrono()->setT(2);
	}
	else *currPlPos = { 1578, 1334 };

	lucPos = &pl->getLucca()->getPos();
	*lucPos = *currPlPos;
	frogPos = &pl->getFrog()->getPos();
	*frogPos = *currPlPos;
	marPos = &pl->getMarle()->getPos();
	*marPos = *currPlPos;

	setMapNum(1);

	eR.push_back({ 464, 1080, 464 + 61, 1080 + 61 }); // 음식 보따리 이벤트
	eR.push_back({ 1854, 510, 1854 + 61, 510 + 61 }); // Marle 목걸이 이벤트

	exit.push_back({ 1440, 1410 , 1440 + 256, 1410 + 64});
	exit.push_back({ 0, 320 , 64, 320 + 256 });
	prevPlPos = *currPlPos;
	gameScene::setViewport(currPlPos->x, currPlPos->y);

	event2MsgDelay = 30;
	return S_OK;
}

void millennialFair::update()
{

	if (PtInRect(&exit[0], *currPlPos))
	{
		gameScene::goToMap(1);
		return;
	}
	else if (PtInRect(&exit[1], *currPlPos))
	{
		gameScene::goToMap(3);
		return;
	}
	else if (currPlPos->y > 32 + 1856) currPlPos->y = 32 + 1856;
	else if (PtInRect(&eR[0], *currPlPos) && event1 < 1 && KEY->down('V')) event1 = 1; // 보따리 이벤트 진행
	else if (PtInRect(&eR[1], *currPlPos) && event2 < 1 && KEY->down('V')) event2 = 1; // 펜던트 이벤트 진행

	// 보따리 이벤트 내용
	if (event1 == 1 && PtInRect(&eR[0], *currPlPos) && KEY->down('V'))
	{
		_isChrUnmovable = TRUE;
		pl->getCrono()->stopAni();
		pl->getCrono()->setState(NORMAL_IDLE);
		if (!TXT->getTextWindowPos()) TXT->toggleTextWindowPos();
		TXT->enqueueL("Eat it?\n\t\tYes.\n\t\tNo.<c>", 0);
		TXT->enqueueL("\n\t\tYum!\n\tTastes like chicken!", 1);
		++event1;
	}
	else if (event1 == 2 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		if (TXT->getChoiceSelected() == 1)
		{
			pl->getCrono()->hitDamage(pl->getCrono()->getHP() - pl->getCrono()->getMaxHP());
			pl->getCrono()->minusMP(pl->getCrono()->getMP() - pl->getCrono()->getMaxMP());
			pl->getLucca()->hitDamage(pl->getLucca()->getHP() - pl->getLucca()->getMaxHP());
			pl->getLucca()->minusMP(pl->getLucca()->getMP() - pl->getLucca()->getMaxMP());
			pl->getFrog()->hitDamage(pl->getFrog()->getHP() - pl->getFrog()->getMaxHP());
			pl->getFrog()->minusMP(pl->getFrog()->getMP() - pl->getFrog()->getMaxMP());
			++event1;
		}
		else
		{
			event1 = 0;
		}

		TXT->resetChoiceSelected();
		_isChrUnmovable = FALSE;
		TXT->toggleTextWindowPos();
		TXT->resetChoiceSelected();
	}

	// 펜던트 이벤트 내용
	if ((event2 == 1 || event2 == 2) && PtInRect(&eR[1], *currPlPos) && KEY->down('V') && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE) // 리마인더: 위치 조건 추가 필요
	{
		_isChrUnmovable = TRUE;
		pl->getCrono()->stopAni();
		pl->getCrono()->setState(NORMAL_IDLE);
		if (!TXT->getTextWindowPos()) TXT->toggleTextWindowPos();
		TXT->enqueueL("GIRL: Oh, thank goodness!\n\tMy pendant!\n\tIt has a lot of sentimental value!", 0);
		TXT->enqueueL("\tMay I have it back?\n\t\tYes.\n\t\tNo.<c>", 0);
		event2 = 3;
	}
	else if (event2 == 3)
	{
		if (KEY->down('V') && TXT->getIfWholeCurrStrIsShown1())
		{
			if (TXT->getChoiceSelected() == 2)
			{
				TXT->resetChoiceSelected();
				_isChrUnmovable = FALSE;
			}
			else if (TXT->getChoiceSelected() == 1) { ++event2; TXT->resetChoiceSelected(); }
		}
		else if (TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
		{
			TXT->toggleTextWindowPos();
			event2 = 2;
		}
	}
	else if (event2 == 4 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		TXT->enqueueL("\tI came to see the Festival!\n\tYou}live in this town, don't you?!\n\tI feel a little out of place here.");
		++event2;
	}
	else if (event2 == 5)
	{
		TXT->enqueueL("\tWould you mind if I walked around\n\twith you for a while?\n\t\tSure.\n\t\tSorry.<c>");
		++event2;
	}
	else if (event2 == 6 && KEY->down('V') && TXT->getIfWholeCurrStrIsShown1())
	{
		if (TXT->getChoiceSelected() == 2) { event2 = 7; TXT->resetChoiceSelected(); }
		else if (TXT->getChoiceSelected() == 1) { event2 = 8; TXT->resetChoiceSelected(); }
	}
	else if (event2 == 7 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		TXT->enqueueL("GIRL: Oh, come on! I don't know\n\tanyone around here! Please!\n\t\tOk.\n\t\tNo.<c>");
		event2 = 9;
	}
	else if (event2 == 8 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		TXT->enqueueL("GIRL: You're a true gentleman!");
		event2 = 10;
	}
	else if (event2 == 9 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		if (TXT->getChoiceSelected() == 2) { event2 = 7; TXT->resetChoiceSelected(); }
		else if (TXT->getChoiceSelected() == 1) { event2 = 8; TXT->resetChoiceSelected(); }
	}
	else if (event2 == 10 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		TXT->enqueueL("GIRL: Oh, by the way}\n\tMy name is, er}");
		SC->addScene("이름 변경 화면", new characterNamingScene(2)); // Marle
		event2++;
	}
	else if (event2 == 11 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		SC->changeScene("이름 변경 화면");
		event2++;
	}
	else if (event2 == 12 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE && _blackScreenAlpha == 0x00)
	{
		if (--event2MsgDelay == 0)
		{
			TXT->enqueueL("GIRL: }um, " + _playChrNames[2] + "!\n\tAnd you're?");
			event2++;
		}
	}
	else if (event2 == 13 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE)
	{
		TXT->enqueueL(_playChrNames[2] + ": " + _playChrNames[0] + "?\n\tWhat a nice name!\n\tPleased to meet you!\n\tNow, lead on!");
		event2++;
	}

	zorderUpdate();
	mapCollision();
	gameScene::updateViewport(currPlPos->x, currPlPos->y);
	prevPlPos = *currPlPos;
}

void millennialFair::release()
{
	cO.clear();
	eR.clear();
	exit.clear();
	//_tile.clear();

	event1 = 0; // 보따리 초기화
}

void millennialFair::render()
{
	IMG->renderZ(zGrid, "Leene_SquareZ2", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);
	IMG->renderZ(0, "Leene_Square2", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);

	if (event1 < 3) IMG->renderZ(currPlPos->y - 1, "보따리", getMemDC(), eR[0].left, eR[0].top - 64);

	if (event2 < 1) IMG->renderZ(currPlPos->y - 1, "펜던트", getMemDC(), eR[1].left, eR[1].top);
	else if (event2 == 14 && TXT->getTextWindowState1() == TEXT_WINDOW_STATE::INVISIBLE) // 펜던트 이벤트가 종료되면
	{
		TXT->toggleTextWindowPos();
		_isChrUnmovable = FALSE;
	}

#ifdef _DEBUG
	{
		if (KEY->isToggledOn(VK_CAPITAL))
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
			HPEN hOPen = (HPEN)SelectObject(getMemDC(), hPen);
			HBRUSH hOBrush = (HBRUSH)SelectObject(getMemDC(), hBrush);
			for (int y = 0; y < _crtYsize; ++y)
			{
				for (int x = 0; x < _crtXsize - 18; ++x)
				{
					if (!_tile[y][x].rectYes) continue;
					DrawRct(getMemDC(), _tile[y][x].rect);
				}
			}
			DeleteObject(SelectObject(getMemDC(), hOPen));
			DeleteObject(SelectObject(getMemDC(), hOBrush));
		}
		if (KEY->isToggledOn(VK_CAPITAL))
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 255));
			HPEN hOPen = (HPEN)SelectObject(getMemDC(), hPen);
			HBRUSH hOBrush = (HBRUSH)SelectObject(getMemDC(), hBrush);
			for (size_t i = 0; i < eR.size(); ++i)
			{
				DrawRct(getMemDC(), eR[i]);
			}
			DeleteObject(SelectObject(getMemDC(), hOPen));
			DeleteObject(SelectObject(getMemDC(), hOBrush));
		}
		if (KEY->isToggledOn(VK_CAPITAL))
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
			HPEN hOPen = (HPEN)SelectObject(getMemDC(), hPen);
			HBRUSH hOBrush = (HBRUSH)SelectObject(getMemDC(), hBrush);
			for (size_t i = 0; i < exit.size(); ++i)
			{
				DrawRct(getMemDC(), exit[i]);
			}
			DeleteObject(SelectObject(getMemDC(), hOPen));
			DeleteObject(SelectObject(getMemDC(), hOBrush));
		}
	}

#endif
}

