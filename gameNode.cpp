#include "stdafx.h"
#include "gameNode.h"
#include <windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM

gameNode::gameNode()
{
}


gameNode::~gameNode()
{
}

HRESULT gameNode::init()
{
	_hDC = GetDC(_hWnd);
	_shouldInitManagers = false;

	return S_OK;
}

HRESULT gameNode::init(bool shouldInitManagers)
{
	_hDC = GetDC(_hWnd);
	_shouldInitManagers = shouldInitManagers;

	if (_shouldInitManagers)
	{
		KEY->init();
		IMG->init();
		TIME->init();
		FX->init();
		SC->init();
		SND->init();
		KAN->init();
		DATA->init();
		TXT->init();
	}
	return S_OK;
}

void gameNode::release()
{
	if (_shouldInitManagers)
	{
		RNG->releaseSingleton();
		KEY->releaseSingleton();
		SC->release();
		SC->releaseSingleton();
		TIME->release();
		TIME->releaseSingleton();
		FX->release();
		FX->releaseSingleton();
		SND->release();
		SND->releaseSingleton();
		KAN->release();
		KAN->releaseSingleton();
		DATA->releaseSingleton();
		TXT->release();
		TXT->releaseSingleton();
		IMG->release();
		IMG->releaseSingleton();
	}
	ReleaseDC(_hWnd, _hDC);
}

void gameNode::update()
{
	SND->update();

}

void gameNode::render()
{

}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		
		case WM_MOUSEMOVE:
		_mouse.x = GET_X_LPARAM(lParam);
		_mouse.y = GET_Y_LPARAM(lParam);
		break;

		case WM_KEYDOWN:
			switch (wParam)
			{

				case VK_ESCAPE:
					PostQuitMessage(0);
				break;
			}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
