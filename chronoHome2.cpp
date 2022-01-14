#include "stdafx.h"
#include "chronoHome2.h"
#include "player.h"
HRESULT chronoHome2::init()
{
	load2("res/mapInfo/크로노의방/saveMap2.map");
	currPlPos = &pl->getCrono()->getPos();
	if (getPrevMapNum() == 5) *currPlPos = { 630, 425 };
	else if (getPrevMapNum() == 2) *currPlPos = { 710, 930 };
	else *currPlPos = { 765, 385 };

	lucPos = &pl->getLucca()->getPos();
	*lucPos = *currPlPos;
	frogPos = &pl->getFrog()->getPos();
	*frogPos = *currPlPos;
	marPos = &pl->getMarle()->getPos();
	*marPos = *currPlPos;

	setMapNum(6);
	// ========================================================

	exit.push_back({ 575, 100, 595, 400 });
	exit.push_back({ 640, 1005, 640 + 200, 1005 + 40 });
	prevPlPos = *currPlPos;
	gameScene::setViewport(currPlPos->x, currPlPos->y);
	return S_OK;
}

void chronoHome2::update()
{
	if (PtInRect(&exit[0], *currPlPos))
	{
		gameScene::goToMap(5);
		return;
	}
	else if (PtInRect(&exit[1], *currPlPos))
	{
		gameScene::goToMap(1);
		return;
	}

	zorderUpdate();
	mapCollision();
	gameScene::updateViewport(currPlPos->x, currPlPos->y);
	prevPlPos = *currPlPos;
}

void chronoHome2::release()
{
	cO.clear();
	eR.clear();
	exit.clear();
	//_tile.clear();

}

void chronoHome2::render()
{
	//IMG->render("크로노집아래", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);

	IMG->renderZ(zGrid, "CronoRoomZ3", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);
	IMG->renderZ(0, "CronoRoom3", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);

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