#include "stdafx.h"
#include "playground.h"
#include <queue>
#include <list>

// 메모리 누수 감지용
#ifdef _DEBUG 
#include <crtdbg.h>
#endif

HINSTANCE	_hInst;
HWND		_hWnd;
POINT		_mouse, _currOrg, _newOrg, _menuMsgPos;
RECT		_camMovLim, _totRegion;
BOOL		_isChrUnmovable, _isGamePaused{ FALSE }, _isScrBlackingOut{ FALSE }, _isMenuDisplayed{ FALSE },
			_isInBattle{ FALSE }, _isActiveBattle{ TRUE },
			_shouldRenderUsingWindowCoords{ FALSE }, _prevRenderMode{ FALSE }; // 주의: _shouldRenderUsingWindowCoords는 Z가 붙어 있는 그림 출력 함수에는 적용되지 않는다.
float		_currMasterVolume, _currBGMVolume, _currSFXVolume;
string		_playChrNames[7]{ "Crono", "Lucca", "Marle", "Frog", "Robo", "Ayla", "Magus" };
int			_battleGaugeSpeed{ 4 }, _battleMsgSpeed{ 4 }, _textWindowSkin{ 0 }, _battleGaugeType{ 1 };
vector<int>	_events;
BYTE		_textWindowAlpha{ 0xFF }, _blackScreenAlpha{ 0x00 };

int _crtXsize, _crtYsize;

vector<vector<tileInfo>>	_tile;

playground _pg;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 메인 함수
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
#ifdef _DEBUG 
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // 메모리 누수를 감지한다.
	//_CrtSetBreakAlloc(517); // 메모리 누수 시작 위치에서 실행을 정지한다.
#endif

	MSG			message;
	WNDCLASS	wndClass;

	_hInst = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	RECT winRect = {0, 0, WINW, WINH};

	AdjustWindowRect(&winRect, WS_CAPTION | WS_SYSMENU, FALSE);

	_hWnd = CreateWindow(
		WINNAME, WINNAME,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		WINX + winRect.left, WINY + winRect.top, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(_hWnd, nCmdShow);

	if (FAILED(_pg.init()))	return NULL;

	while (TRUE)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIME->update(60.0f);
			_pg.update();
			_pg.render();
		}
	}
	
	_pg.release();

	UnregisterClass(WINNAME, _hInst);

	return (int)message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _pg.MainProc(hWnd, iMessage, wParam, lParam);
}
