#include "stdafx.h"
#include "leeneSquare.h"
#include "player.h"

HRESULT leeneSquare::init()
{
	load2("res/mapInfo/밀레광장/saveMap1.map");
	currPlPos = &pl->getCrono()->getPos();
	if (getPrevMapNum() == 6) *currPlPos = { 1495 + 100, 1750 };
	else if (getPrevMapNum() == 1) *currPlPos = { 1600, 144 }; 
	else *currPlPos = { 1600, 1400 };
	lucPos = &pl->getLucca()->getPos();
	frogPos = &pl->getFrog()->getPos();

	lucPos = &pl->getLucca()->getPos();
	*lucPos = *currPlPos;
	frogPos = &pl->getFrog()->getPos();
	*frogPos = *currPlPos;
	marPos = &pl->getMarle()->getPos();
	*marPos = *currPlPos;

	setMapNum(2);

	exit.push_back({ 1444, 0, 1696, 30 });
	exit.push_back({ 1495, 1866, 1680, 1866 + 63 });

	prevPlPos = *currPlPos;
	gameScene::setViewport(currPlPos->x, currPlPos->y);
	return S_OK;
}

void leeneSquare::release()
{
	cO.clear();
	eR.clear();
	exit.clear();
	//_tile.clear();

}

void leeneSquare::update()
{
	//if (currPlPos->x - 32 < 0 && currPlPos->y >= 300 && currPlPos->y <= 580) gameScene::goToMap(3); //가토 + &&y?? 의 위치.
	//if (currPlPos->y + 32 > 1500 && currPlPos->x >= 1400 && currPlPos->x <= 1600) gameScene::goToMap(1); //메인 광장가는 위치 &&x는?
	//if (currPlPos->y - 32 < 0) gameScene::goToMap(4); //텔레포트 가는 방향

	zorderUpdate();
	if (PtInRect(&exit[0], *currPlPos))
	{
		gameScene::goToMap(2);
		return;
	}
	else if (PtInRect(&exit[1], *currPlPos))
	{
		gameScene::goToMap(6);
		return;
	}

	mapCollision();
	gameScene::updateViewport(currPlPos->x, currPlPos->y);
	prevPlPos = *currPlPos;
}

void leeneSquare::render()
{
	IMG->renderZ(zGrid, "Leene_SquareZ", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);
	IMG->renderZ(0, "Leene_Square", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);

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
