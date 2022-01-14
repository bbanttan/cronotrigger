#pragma once
#include "image.h"

static image* _backBuffer = IMG->add("backBuffer", WINW, WINH);

typedef struct tagPOINT3D { LONG x, y, z; } POINT3D, *LPPOINT3D;

class gameNode
{
private:
	HDC _hDC;
	bool _shouldInitManagers;

public:
	gameNode();
	virtual ~gameNode();

	virtual HRESULT init();
	virtual HRESULT init(bool shouldInitManagers);
	virtual void release();
	virtual void update();
	virtual void render();

	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hDC; }

	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);
};

