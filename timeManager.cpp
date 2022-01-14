#include "stdafx.h"
#include "timeManager.h"

timeManager::timeManager()
	: _timer(nullptr)
{
}

timeManager::~timeManager()
{
}

HRESULT timeManager::init()
{
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release()
{
	if (_timer != nullptr)
	{
		SAFE_DEL(_timer);
	}
}

void timeManager::update(float lock)
{
	if (_timer != nullptr)
	{
		_timer->tick(lock);
	}
}

void timeManager::render(HDC hDC)
{
	char str[256];
	string frameRate;
	SetBkMode(hDC, OPAQUE);

#ifdef _DEBUG
	{
		sprintf_s(str, "%d FPS", _timer->getFrameRate());
		TextOut(hDC, 0, 0, str, static_cast<int>(strlen(str)));

		sprintf_s(str, "World time: %.4f s", _timer->getWorldTime());
		TextOut(hDC, 0, 16, str, static_cast<int>(strlen(str)));

		sprintf_s(str, "Delta time: %.4f s", _timer->getElapsedTime());
		TextOut(hDC, 0, 32, str, static_cast<int>(strlen(str)));
	}
	#else
	{
		sprintf_s(str, "%d FPS", _timer->getFrameRate());
		TextOut(hDC, 0, 0, str, static_cast<int>(strlen(str)));
	}
#endif

}
