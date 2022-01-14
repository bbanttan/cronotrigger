#pragma once
#include "gameNode.h"

class progressBar : public gameNode
{
private:
	RECT _rctProgress;
	float _x, _y, _width;

	image* _progressBarFront;
	image* _progressBarBack;

public:
	progressBar() {};
	~progressBar() override {};

	HRESULT init(int x, int y, int width, int height);
	void release() override;
	void update() override;
	void render() override {}; // 사용하지 않는 함수
	void render(HDC hDC);

	// 현재 값을 즉각 반영하여 게이지 그림 너비를 갱신하는 함수
	void setGauge(float currVal, float maxVal);
	void setGauge(int currVal, int maxVal);

	//// 현재 값을 시간을 두고 반영하여 게이지 그림 너비를 갱신하는 함수
	//void updateGauge(float currVal, float maxVal);

	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	
};

