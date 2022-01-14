#include "stdafx.h"
#include "progressBar.h"

HRESULT progressBar::init(int x, int y, int width, int height)
{
	_x = static_cast<float>(x);
	_y = static_cast<float>(y);

	_rctProgress = MakeRct(x, y, width, height);

	_progressBarFront = IMG->add("frontBar", "res/images/progressBars/fullBarInActive.bmp", width, height, true, RGB(255, 0, 255));
	_progressBarBack = IMG->add("backBar", "res/images/progressBars/emptyBar.bmp", width, height, true, RGB(255, 0, 255));
	IMG->add("fullBar", "res/images/progressBars/fullBar.bmp", width, height, true, RGB(255, 0, 255));
	_width = static_cast<float>(_progressBarFront->getWidth());

	return S_OK;
}

void progressBar::release()
{

}

void progressBar::update()
{
	_rctProgress = MakeRctC(static_cast<int>(_x), static_cast<int>(_y), _progressBarFront->getWidth(), _progressBarFront->getHeight());
}

void progressBar::render(HDC hDC)
{
	IMG->render("backBar", hDC,
		_rctProgress.left,
		_rctProgress.top, 0, 0,
		_progressBarBack->getWidth(), _progressBarBack->getHeight());

	_progressBarFront->render(hDC,
		_rctProgress.left,
		_rctProgress.top, 0, 0,
		static_cast<int>(_width), _progressBarFront->getHeight());
}

void progressBar::setGauge(float currVal, float maxVal)
{
	if (abs(currVal - maxVal) < FLT_EPSILON)
	{
		_width = static_cast<float>(_progressBarBack->getWidth());
		_progressBarFront = IMG->find("fullBar");
	}
	else
	{
		_width = (currVal / maxVal) * (_progressBarBack->getWidth() - 32) + 16; // 보정치 적용
		_progressBarFront = IMG->find("frontBar");
	}
}

void progressBar::setGauge(int currVal, int maxVal)
{
	if (currVal == maxVal)
	{
		_width = static_cast<float>(_progressBarBack->getWidth());
		_progressBarFront = IMG->find("fullBar");
	}
	else
	{
		_width = (static_cast<float>(currVal) / static_cast<float>(maxVal)) * (_progressBarBack->getWidth() - 32) + 16; // 보정치 적용
		_progressBarFront = IMG->find("frontBar");
	}
}

//void progressBar::updateGauge(float currVal, float maxVal)
//{
//	if (currVal < FLT_EPSILON)
//	{
//		_width = 0; return;
//	}
//
//	float targetWidth = (currVal / maxVal) * _progressBarBack->getWidth();
//
//	if (abs(currVal - maxVal) < FLT_EPSILON)
//	{
//		_width = targetWidth;
//	}
//	else if (_width < ((currVal - 1.f) / maxVal) * _progressBarBack->getWidth())
//	{
//		_width = ((currVal - 1.f) / maxVal) * _progressBarBack->getWidth();
//	}
//	else
//	{
//		if (abs(targetWidth - _width) > 4.f) _width += 4.f * (targetWidth - _width) / abs(targetWidth - _width);
//		else _width = targetWidth;
//	}
//}
