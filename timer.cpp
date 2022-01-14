#include "stdafx.h"
#include "timer.h"

timer::timer()
{
}

timer::~timer()
{
}

HRESULT timer::init()
{
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency))
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
		_timeScale = 1.0f / _frequency;
	}
	
	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

void timer::tick(float FPS)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_currTime);

	_timeElapsed = (_currTime - _lastTime) * _timeScale;

	if (FPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / FPS))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&_currTime);
			_timeElapsed = (_currTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _currTime;
	_FPSFrameCount++;
	_FPSTimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}

}

unsigned long timer::getFrameRate(char * str) const
{
	return _frameRate;
}
