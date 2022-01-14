#include "stdafx.h"
#include "gameScene.h"
#include "player.h"
#include "millennialFair.h"
#include "bossGatoStage.h"
#include "teleport.h"
#include "leeneSquare.h"
#include "chronoHome.h"
#include "chronoHome2.h"
#include "mapManager.h"
#include "playerUiScene.h"


int gameScene::_countForReEnablingKeyInput;
BOOL gameScene::_mapChangeComplete;
player* gameScene::_p;
vector<shared_ptr<mapManager>> gameScene::_mapList;
shared_ptr<mapManager> gameScene::_currMap;
int gameScene::_prevMapNum, gameScene::_mapNum;

// 설명: gameScene을 상속받는 클래스의 객체가 생성될 때에도 gameScene()이 호출되기 때문에 아래 생성자를 별도로 정의하였다.
gameScene::gameScene(int anyNum)
{
	UNREFERENCED_PARAMETER(anyNum);

	
	_currOrg = _newOrg = { 0, 0 };

	_p = new player;
	_p->init();

	_mapList.emplace_back(new millennialFair);		//0
	_mapList.emplace_back(new leeneSquare);			//1
	_mapList.emplace_back(new bossGatoStage);		//2
	_mapList.emplace_back(new teleport);			//3
	_mapList.emplace_back(new chronoHome);			//4
	_mapList.emplace_back(new chronoHome2);			//5
													

	_totRegion = { 0, 0, 1024, 1024 };
	_camMovLim = { 0, 0, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };

	//_totRegion = { 0, 0, 3072, 1856 }; //테스트용도
	//_camMovLim = { _totRegion.left, _totRegion.top, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };
	
	_currMap = _mapList[4];
	_currMap->setLinkTo(_p);
	_currMap->init();

	_isChrUnmovable = TRUE;
	_countForReEnablingKeyInput = 12;
	_mapChangeComplete = FALSE;
}

gameScene::~gameScene() // 주의: 중복 호출이 되어도 문제가 발생하지 않을 것만 나열하여야 한다.
{
	if (_p != nullptr)
	{
		_p->release();
		SAFE_DEL(_p);
	}

	if (_currMap != nullptr)
	{
		_currMap->release();
		_currMap = nullptr;
	}
}

HRESULT gameScene::init() // 주의: gameScene에서 다른 장면으로 갔다 올 수도 있으므로 중복 호출 하여도 문제가 발생하지 않을 것만 나열하여야 한다.
{
	// 메뉴를 켤 때 움직이고 있었다면 아래 줄들이 있어야 캐릭터가 멈춘 상태로 다시 보이게 된다.
	if (_p->getCrono()->getState() != NORMAL_IDLE)
	{
		_p->getCrono()->setState(NORMAL_IDLE);
		_p->getCrono()->stopAni();
	}
	_p->update();

	// 시작 화면이 있다면 지운다.
	SC->delScene("시작 화면");

	// 이름 변경 화면이 있다면 지운다.
    SC->delScene("이름 변경 화면");
	return S_OK;
}
void gameScene::release() // 주의: gameScene에서 다른 장면으로 갔다 올 수도 있으므로 중복 호출 하여도 문제가 발생하지 않을 것만 나열하여야 한다.
{
}

void gameScene::update()
{

	if (KEY->down('D') && !_isInBattle && !_isChrUnmovable && _countForReEnablingKeyInput == 0)
	{
		reinterpret_cast<playerUiScene*>(SC->getScene("스텟 창"))->setLinkTo(_p);
		SC->changeScene("스텟 창");
	}
	
	if (TXT->getTextWindowState1() != TEXT_WINDOW_STATE::INVISIBLE
		|| TXT->getTextWindowState2() != TEXT_WINDOW_STATE::INVISIBLE || _isInBattle)
	{
		if (KEY->down('X')) TXT->toggleTextWindowPos();
	}

	_p->update();
	_currMap->update();


	if (_isChrUnmovable && !_isScrBlackingOut && !_mapChangeComplete)
	{
		if (_countForReEnablingKeyInput > 0) --_countForReEnablingKeyInput;
		else
		{
			_isChrUnmovable = FALSE;
			_mapChangeComplete = TRUE;
		}
	}

	// 출력할 글 갱신
	if (_isInBattle) TXT->updateBM();
	else if (_isMenuDisplayed) TXT->updateC();
	else TXT->updateL();
}

void gameScene::render()
{
	PatBlt(getMemDC(), 0, 0, WINW, WINH, BLACKNESS);
	_currMap->render();
	_p->render();
	IMG->execZ();

	// 글 출력
	if (TXT->getTextWindowState1() != TEXT_WINDOW_STATE::INVISIBLE
		|| TXT->getTextWindowState2() != TEXT_WINDOW_STATE::INVISIBLE)
	{
		if (_isInBattle) TXT->renderBM(getMemDC());
		else TXT->renderL(getMemDC());
	}
	else if (_isMenuDisplayed) TXT->renderC(getMemDC(), _menuMsgPos.x, _menuMsgPos.y);
	
#ifdef _DEBUG
	char str[256];
	//if (KEY->isToggledOn(VK_TAB)) DrawRct(getMemDC(), _p->getCrono()->getPos().x - 32, _p->getCrono()->getPos().y - 8, 64, 8);
		{
			if (KEY->isToggledOn(VK_SCROLL))
			{
				//sprintf_s(str, "Player pos: %d, %d", _p->getCrono()->getPos().x, _p->getCrono()->getPos().y);
				//TextOut(getMemDC(), 0, 48, str, static_cast<int>(strlen(str)));

				sprintf_s(str, "Pointer pos: %d, %d (%d, %d)", _mouse.x + _currOrg.x, _mouse.y + _currOrg.y, _mouse.x, _mouse.y);
				TextOut(getMemDC(), 0, 64, str, static_cast<int>(strlen(str)));

				if (_mouse.x + 42 < WINW)
				{
					Rectangle(getMemDC(), _mouse.x + 9, _mouse.y + 9, _mouse.x + 10 + 33, _mouse.y + 10 + 33);
					StretchBlt(getMemDC(), _mouse.x + 10, _mouse.y + 10, 32, 32, getMemDC(), _mouse.x - 8, _mouse.y - 8, 16, 16, SRCCOPY);
					DrawLine(getMemDC(), _mouse.x + 26 + _currOrg.x, _mouse.y + 26 + _currOrg.y, _mouse.x + 26 + _currOrg.x, _mouse.y + 30 + _currOrg.y);
					DrawLine(getMemDC(), _mouse.x + 26 + _currOrg.x, _mouse.y + 26 + _currOrg.y, _mouse.x + 30 + _currOrg.x, _mouse.y + 26 + _currOrg.y);
				}
				else
				{
					Rectangle(getMemDC(), _mouse.x - 43, _mouse.y + 9, _mouse.x + 10 - 19, _mouse.y + 10 + 33);
					StretchBlt(getMemDC(), _mouse.x - 42, _mouse.y + 10, 32, 32, getMemDC(), _mouse.x - 8, _mouse.y - 8, 16, 16, SRCCOPY);
					DrawLine(getMemDC(), _mouse.x - 26 + _currOrg.x, _mouse.y + 26 + _currOrg.y, _mouse.x - 26 + _currOrg.x, _mouse.y + 30 + _currOrg.y);
					DrawLine(getMemDC(), _mouse.x - 26 + _currOrg.x, _mouse.y + 26 + _currOrg.y, _mouse.x - 22 + _currOrg.x, _mouse.y + 26 + _currOrg.y);
				}
			}
		}
#endif
}

void gameScene::goToMap(int num)
{
	_isChrUnmovable = TRUE;
	_isScrBlackingOut = TRUE;
	_mapChangeComplete = FALSE;
	_p->getCrono()->setState(NORMAL_IDLE);

	_currMap->release();
	_countForReEnablingKeyInput = 12;

	_prevMapNum = _mapNum;

	switch (num)
	{
	case 1: //메인광장
		_totRegion = { 0, 0, 3072, 1856 };
		_camMovLim = { 0, 0, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };
		_currMap = _mapList[1];
		break;

	case 2: //마루 만나는 곳
		_totRegion = { 0, 0, 3072, 1468 };
		_camMovLim = { 0, 0, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };
		_currMap = _mapList[0];
		break;

	case 3: //보스 가토
		_totRegion = { 0, 0, 1024, 1024 };
		_camMovLim = { 0, 0, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };
		_currMap = _mapList[2];
		break;

	case 4: //텔레포트 이벤트
		_totRegion = { 0, 0, 1024, 1024 };
		_camMovLim = { 0, 0, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };
		_currMap = _mapList[3];
		break;

	case 5://크로노위층
		_totRegion = { 0, 0, 1024, 1024 };
		_camMovLim = { 0, 0, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };
		_currMap = _mapList[4];
		break;

	case 6://아랫층
		_totRegion = { 0, 0, 1024, 1024 };
		_camMovLim = { 0, 0, _totRegion.right - _totRegion.left - WINW, _totRegion.bottom - _totRegion.top - WINH };
		_currMap = _mapList[5];
		break;
	}
	_currMap->setLinkTo(_p);
	_currMap->init();
}



void gameScene::updateViewport(int x, int y)
{
	if (y - VIEWPORT_UPDATE_OFFSET > _currOrg.y + WINH / 2)
	{
		if (_currOrg.y < _camMovLim.bottom)
			_newOrg.y = _currOrg.y + static_cast<int>(static_cast<float>(_camMovLim.bottom - _currOrg.y) / 12.f) + 1;
		if (y < _newOrg.y + WINH / 2) _newOrg.y = y - WINH / 2;
	}
	else if (y + VIEWPORT_UPDATE_OFFSET < _currOrg.y + WINH / 2)
	{
		if (_currOrg.y > _camMovLim.top)
			_newOrg.y = _currOrg.y - static_cast<int>(static_cast<float>(_currOrg.y - _camMovLim.top) / 12.f) - 1;
		if (y > _newOrg.y + WINH / 2) _newOrg.y = y - WINH / 2;
	}
	if (x - VIEWPORT_UPDATE_OFFSET > _currOrg.x + WINW / 2)
	{
		if (_currOrg.x < _camMovLim.right)
			_newOrg.x = _currOrg.x + static_cast<int>(static_cast<float>(_camMovLim.right - _currOrg.x) / 12.f) + 1;
		if (x < _newOrg.x + WINW / 2) _newOrg.x = x - WINW / 2;
	}
	else if (x + VIEWPORT_UPDATE_OFFSET < _currOrg.x + WINW / 2)
	{
		if (_currOrg.x > _camMovLim.left)
			_newOrg.x = _currOrg.x - static_cast<int>(static_cast<float>(_currOrg.x - _camMovLim.left) / 12.f) - 1;
		if (x > _newOrg.x + WINW / 2) _newOrg.x = x - WINW / 2;
	}
	if (_newOrg.x != _currOrg.x || _newOrg.y != _currOrg.y)
	{
		_currOrg.x = _newOrg.x;
		_currOrg.y = _newOrg.y;
	}
}

void gameScene::setViewport(int x, int y)
{
	if (y > WINH / 2) _newOrg.y = min(y - WINH / 2, _camMovLim.bottom);
	else _newOrg.y = max(y - WINH / 2, _camMovLim.top);

	if (x > WINW / 2) _newOrg.x = min(x - WINW / 2, _camMovLim.right);
	else _newOrg.x = max(x - WINW / 2, _camMovLim.left);

	_currOrg.x = _newOrg.x;
	_currOrg.y = _newOrg.y;
}
