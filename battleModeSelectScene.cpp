#include "stdafx.h"
#include "battleModeSelectScene.h"

battleModeSelectScene::battleModeSelectScene(): _isWaitSelected(FALSE)
{
}

battleModeSelectScene::~battleModeSelectScene()
{
}

HRESULT battleModeSelectScene::init()
{
	SC->delScene("시작 화면"); // 시작 화면으로 돌아갈 일이 없으므로 매니저에 등록된 시작 화면을 제거한다.
	TXT->enqueueC("Enemies will attack even\nif you're not ready!");
	return S_OK;
}

void battleModeSelectScene::release()
{
}

void battleModeSelectScene::update()
{
	if (!_isScrBlackingOut)
	{
		if (KEY->down(VK_DOWN) && !_isWaitSelected)
		{
			_isWaitSelected = TRUE;
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
			TXT->enqueueC("Take your time to choose\nitems and fight strategically.");
		}
		else if (KEY->down(VK_UP) && _isWaitSelected)
		{
			_isWaitSelected = FALSE;
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
			TXT->enqueueC("Enemies will attack even\nif you're not ready!");
		}
		TXT->updateC();
	}
}

void battleModeSelectScene::render()
{
	PatBlt(getMemDC(), 0, 0, WINW, WINH, BLACKNESS);

	_shouldRenderUsingWindowCoords = TRUE;

	IMG->render("전투 모드 로고", getMemDC(), 328, 96);

	IMG->render("전투 모드 선택 메시지 창 스킨", getMemDC(), 64, 512);

	TXT->render(getMemDC(), "ACTIVE", 416, 336);
	TXT->render(getMemDC(), "WAIT", 448, 400);

	IMG->frameRender("위치 표시 타일셋", getMemDC(), 352, 332 + 64 * _isWaitSelected, 0, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 352, 332 + 64 * _isWaitSelected, 0, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 352 + 32, 332 + 64 * _isWaitSelected, 1, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 352, 332 + 64 * _isWaitSelected + 32, 0, 1);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 352 + 32, 332 + 64 * _isWaitSelected + 32, 1, 1);

	TXT->renderC(getMemDC(), 64 + 96, 512 + 40);

	if (KEY->down('V'))
	{
		_isActiveBattle = !_isWaitSelected;
		_isScrBlackingOut = TRUE;
		SC->changeScene("이름 변경 화면");
	}

	_shouldRenderUsingWindowCoords = FALSE;
}
