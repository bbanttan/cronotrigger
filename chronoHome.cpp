#include "stdafx.h"
#include "chronoHome.h"
#include "player.h"
HRESULT chronoHome::init()
{
	load2("res/mapInfo/크로노의방/saveMap1.map");
	currPlPos = &pl->getCrono()->getPos();
	if (getPrevMapNum() == 6) *currPlPos = { 450, 995 };
	else *currPlPos = { 560, 679 };

	lucPos = &pl->getLucca()->getPos();
	*lucPos = *currPlPos;
	frogPos = &pl->getFrog()->getPos();
	*frogPos = *currPlPos;
	marPos = &pl->getMarle()->getPos();
	*marPos = *currPlPos;

	setMapNum(5);
	
	// ========================================================

	exit.push_back({ 495, 1005, 495 + 100, 1005 + 100 });

	prevPlPos = *currPlPos;
	gameScene::setViewport(currPlPos->x, currPlPos->y);
	return S_OK;
}

void chronoHome::update()
{
	if (PtInRect(&exit[0], *currPlPos))
	{
		gameScene::goToMap(6);
		return;
	}

	zorderUpdate();
	mapCollision();
	gameScene::updateViewport(currPlPos->x, currPlPos->y);
	prevPlPos = *currPlPos;
}

void chronoHome::release()
{
	cO.clear();
	eR.clear();
	exit.clear();
	//_tile.clear();

}

void chronoHome::render()
{
	IMG->renderZ(zGrid,"CronoRoomZ1", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);
	IMG->renderZ(0, "CronoRoom1", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);

	//IMG->execZ();

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