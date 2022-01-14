#include "stdafx.h"
#include "teleport.h"
#include "player.h"

HRESULT teleport::init()
{
	setMapNum(4);
	currPlPos = &pl->getCrono()->getPos();
	if (getPrevMapNum() == 2) *currPlPos = { 548, 746 };
	else *currPlPos = { 548, 746 };
	exit.push_back({ 444, 974 , 444 + 196, 974 +46 });

	lucPos = &pl->getLucca()->getPos();
	*lucPos = *currPlPos;
	frogPos = &pl->getFrog()->getPos();
	*frogPos = *currPlPos;
	marPos = &pl->getMarle()->getPos();
	*marPos = *currPlPos;

	prevPlPos = *currPlPos;
	gameScene::setViewport(currPlPos->x, currPlPos->y);
	return S_OK;
}

void teleport::release()
{
	exit.clear();
}

void teleport::update()
{
	for (size_t i = 0; i < exit.size(); ++i)
	{
		if (PtInRect(&exit[0], *currPlPos))
		{
			gameScene::goToMap(2);
			return;
		}
	}

	gameScene::updateViewport(currPlPos->x, currPlPos->y);
	prevPlPos = *currPlPos;
}

void teleport::render()
{
	IMG->render("teleport", getMemDC(), _currOrg.x, _currOrg.y, _currOrg.x, _currOrg.y, WINW, WINH);
#ifdef _DEBUG
	{
		if (KEY->isToggledOn(VK_CAPITAL))
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
			HPEN hOPen = (HPEN)SelectObject(getMemDC(), hPen);
			HBRUSH hOBrush = (HBRUSH)SelectObject(getMemDC(), hBrush);
			for (size_t i = 0; i < cO.size(); ++i)
			{
				DrawRct(getMemDC(), cO[i]);
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
