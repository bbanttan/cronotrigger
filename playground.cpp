#include "stdafx.h"
#include "playground.h"
#include "loadingScene.h"
#include "playerUiScene.h"
#include "item.h"

playground::playground()
{
}


playground::~playground()
{
}

HRESULT playground::init()
{
	gameNode::init(true);
	keysToCheck = { VK_RETURN, VK_SPACE, VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP, VK_OEM_3, VK_LSHIFT, 'A', 'S', 'D', 'X', 'C', 'V' }; // 입력을 확인할 키 나열(토글 키 제외)

#ifdef _DEBUG
	vector<int> debugKeysToCheck = { VK_ADD, VK_SUBTRACT, VK_ESCAPE, VK_LBUTTON, VK_RBUTTON,
		VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
		'Q', 'W', 'E' }; // 입력을 확인할 키 나열(토글 키 제외, 디버그 전용)
	keysToCheck.insert(end(keysToCheck), begin(debugKeysToCheck), end(debugKeysToCheck));
#endif

	SC->addScene("로딩 장면", new loadingScene);

	_blackScreenAlpha = 0x00;
	_fadeCount = 0;
	_currMasterVolume = 1.f;
	_currBGMVolume = _currSFXVolume = 0.5f;
	_isScrBlackingOut = FALSE;

	SC->changeScene("로딩 장면");


	return S_OK;
}


void playground::release()
{
	gameNode::release();
}

void playground::update()
{
	KEY->updateKeyState(keysToCheck);

#ifdef _DEBUG
	if (KEY->down(VK_SUBTRACT))
	{
		_currMasterVolume = max(0.f, _currMasterVolume - 0.1f);
		SND->updateMasterVolume();
	}
	else if (KEY->down(VK_ADD))
	{
		_currMasterVolume = min(1.f, _currMasterVolume + 0.1f);
		SND->updateMasterVolume();
	}
#endif

	gameNode::update();

	if (_isScrBlackingOut)
	{
		// 페이드아웃에 맞게 알파 값을 변경한다.
		if (_blackScreenAlpha < 0xFF) _blackScreenAlpha += min(0x22, 0xFF - _blackScreenAlpha);
		if (_blackScreenAlpha == 0xFF) {
			if (_fadeCount++ == 24)
			{
				_isScrBlackingOut = FALSE;
				_fadeCount = 0;
			}
		}
	}
	else if (_blackScreenAlpha != 0x00)
	{
		// 페이드인에 맞게 알파 값을 변경한다.
		if (_blackScreenAlpha > 0x00) _blackScreenAlpha -= min(0x22, _blackScreenAlpha);
	}

	SC->update();
}


void playground::render()
{
	// PatBlt(getMemDC(), 0, 0, WINW, WINH, BLACKNESS);

	if (!_isScrBlackingOut) SC->render();

	_prevRenderMode = _shouldRenderUsingWindowCoords;
	_shouldRenderUsingWindowCoords = TRUE;
	if (_blackScreenAlpha > 0x00) IMG->alphaRender("검은 화면", getMemDC(), 0, 0, _blackScreenAlpha);
	
	if (KEY->isToggledOn(VK_SCROLL))
	{
		TIME->render(getMemDC());

		char str[256];
#ifdef _DEBUG
		{
			sprintf_s(str, "Current master volume: %d%% (Num+, Num-)", static_cast<int>(roundf(_currMasterVolume * 100.f)));
			TextOut(getMemDC(), 0, 80, str, static_cast<int>(strlen(str)));
			sprintf_s(str, "Current BGM volume: %d%%", static_cast<int>(roundf(_currBGMVolume * 100.f)));
			TextOut(getMemDC(), 0, 96, str, static_cast<int>(strlen(str)));
			sprintf_s(str, "Current SFX volume: %d%%", static_cast<int>(roundf(_currSFXVolume * 100.f)));
			TextOut(getMemDC(), 0, 112, str, static_cast<int>(strlen(str)));
		}
#endif
	}

	_backBuffer->render(getHDC(), 0, 0, 0, 0, WINW, WINH);
	_shouldRenderUsingWindowCoords = _prevRenderMode;

	if (KEY->down(VK_OEM_3))
	{
		if (KEY->press(VK_LSHIFT)) IMG->takeScreenshot(getHDC(), TRUE);
		else IMG->takeScreenshot(getHDC(), FALSE);
	}
}

