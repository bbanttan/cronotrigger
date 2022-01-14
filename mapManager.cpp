#include "stdafx.h"
#include "mapManager.h"
#include "player.h"

HRESULT mapManager::init()
{
	return S_OK;
}

void mapManager::release()
{
	cO.clear();
	eR.clear();
}

void mapManager::update()
{
	
}

void mapManager::render()
{
	
	//_tile.clear();
}

void mapManager::zorderUpdate()
{
	int yGrid = (int)(currPlPos->y / TILESIZE);
	int xGrid = (int)(currPlPos->x / TILESIZE);

	if (yGrid >= _totRegion.bottom / TILESIZE) return;

	if (_tile[yGrid][xGrid].geography == 4)
		zGrid = currPlPos->y + 20;
	else
		zGrid = currPlPos->y - 2;
}

void mapManager::mapCollision()
{
	for (int y = 0; y < _crtYsize; ++y)
	{
		for (int x = 0; x < _crtXsize-18; ++x)
		{
			if (!_tile[y][x].rectYes) continue;

			if (currPlPos->x < _tile[y][x].rect.right && currPlPos->x > _tile[y][x].rect.left
				&& currPlPos->y < _tile[y][x].rect.bottom && currPlPos->y > _tile[y][x].rect.top)
			{
				int Xdiff = prevPlPos.x - currPlPos->x; //전 x 위치와 현 x 위치의 차이
				int Ydiff = prevPlPos.y - currPlPos->y; //전 y 위치와 현 y 위치의 차이

				if (Xdiff > 0) currPlPos->x = _tile[y][x].rect.right;
				else if (Xdiff < 0) currPlPos->x = _tile[y][x].rect.left;
				if (Ydiff > 0)currPlPos->y = _tile[y][x].rect.bottom;
				else if (Ydiff < 0)currPlPos->y = _tile[y][x].rect.top;
			}
		}
	}
	prevPlPos = *currPlPos;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mapManager::addXtile()
{
	for (int y = 0; y < _crtYsize; y++)
	{
		tileInfo muonTile;

		muonTile.tilePos = { _crtXsize - 19, y };
		muonTile.layerPos1 = { 0,0 };
		muonTile.layerPos2 = { 0,0 };
		muonTile.geography = 0;
		muonTile.rectYes = false;

		_tile[y].push_back(muonTile);
	}
}

void mapManager::removeXtile()
{
	for (int y = 0; y < _crtYsize; y++)
	{
		_tile[y].pop_back();
	}
}

void mapManager::addYtile()
{
	vector<tileInfo> muonVtTile;

	for (int x = 0; x < _crtXsize - 18; x++)
	{
		tileInfo muonTile;

		muonTile.tilePos = { x, _crtYsize - 1 };
		muonTile.layerPos1 = { 0,0 };
		muonTile.layerPos2 = { 0,0 };
		muonTile.geography = 0;
		muonTile.rectYes = false;

		muonVtTile.push_back(muonTile);

		if (x == (_crtXsize - 18) - 1) _tile.push_back(muonVtTile);
	}
}

void mapManager::removeYtile()
{
	_tile.pop_back();
}

void mapManager::load()
{
	HANDLE file;
	DWORD read;

	int
		idxX,
		idxY,
		tempX = -1000,
		tempY = -1000;

	tileInfo _tiles[999];

	file = CreateFile("saveMap1.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tileInfo) * 999, &read, NULL);

	for (int i = 0; i < 999; i++)
	{
		if (tempX < _tiles[i].tilePos.x)
		{
			tempX = _tiles[i].tilePos.x;
			idxX = i;
		}
		if (tempY < _tiles[i].tilePos.y)
		{
			tempY = _tiles[i].tilePos.y;
			idxY = i;
		}
	}

	int tempXsize = _crtXsize;
	int tempYsize = _crtYsize;

	//setMap((tempX + 1) - (crtXsize - 18), (tempY + 1) - (crtYsize));

	_crtXsize += (tempX + 1) - (_crtXsize - 18);
	_crtYsize += (tempY + 1) - (_crtYsize);

	if ((tempY + 1) - (tempYsize) > 0)
	{
		for (int i = 0; i < (tempY + 1) - (tempYsize); i++)
		{
			addYtile();
		}
	}
	if ((tempY + 1) - (tempYsize) < 0)
	{
		for (int i = 0; i < (tempYsize)-(tempYsize); i++)
		{
			removeYtile();
		}
	}
	if ((tempX + 1) - (tempXsize - 18) > 0)
	{
		for (int i = 0; i < ((tempX + 1) - (tempXsize - 18)); i++)
		{
			addXtile();
		}
	}
	if ((tempX + 1) - (tempXsize - 18) < 0)
	{
		for (int i = 0; i < ((tempXsize - 18) - (tempX + 1)); i++)
		{
			removeXtile();
		}
	}

	//crtXsize = (idxX + 19);
	//crtYsize = (idxY / idxX);

	for (int y = 0; y < _crtYsize; y++)
	{
		for (int x = 0; x < _crtXsize - 18; x++)
		{
			_tile[y][x].tilePos = { _tiles[x + (y * (_crtXsize - 18))].tilePos.x   , _tiles[x + (y * (_crtXsize - 18))].tilePos.y };
			_tile[y][x].layerPos1 = { _tiles[x + (y * (_crtXsize - 18))].layerPos1.x , _tiles[x + (y * (_crtXsize - 18))].layerPos1.y };
			_tile[y][x].layerPos2 = { _tiles[x + (y * (_crtXsize - 18))].layerPos2.x , _tiles[x + (y * (_crtXsize - 18))].layerPos2.y };
			_tile[y][x].geography = _tiles[x + (y * (_crtXsize - 18))].geography;
			_tile[y][x].rectYes = _tiles[x + (y * (_crtXsize - 18))].rectYes;
		}
	}
	CloseHandle(file);
}

void mapManager::load2(const char* _name)
{
	HANDLE file;
	DWORD read;

	int
		idxX,
		idxY,
		tempX = -1000,
		tempY = -1000;

	tileInfo _tiles[2000];

	file = CreateFile(_name, GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tileInfo) * 2000, &read, NULL);

	for (int i = 0; i < 2000; i++)
	{
		if (tempX < _tiles[i].tilePos.x)
		{
			tempX = _tiles[i].tilePos.x;
			idxX = i;
		}
		if (tempY < _tiles[i].tilePos.y)
		{
			tempY = _tiles[i].tilePos.y;
			idxY = i;
		}
	}

	int tempXsize = _crtXsize;
	int tempYsize = _crtYsize;

	//setMap((tempX + 1) - (crtXsize - 18), (tempY + 1) - (crtYsize));

	_crtXsize += (tempX + 1) - (_crtXsize - 18);
	_crtYsize += (tempY + 1) - (_crtYsize);

	if ((tempY + 1) - (tempYsize) > 0)
	{
		for (int i = 0; i < (tempY + 1) - (tempYsize); i++)
		{
			addYtile();
		}
	}
	if ((tempY + 1) - (tempYsize) < 0)
	{
		for (int i = 0; i < (tempYsize)-(tempYsize); i++)
		{
			removeYtile();
		}
	}
	if ((tempX + 1) - (tempXsize - 18) > 0)
	{
		for (int i = 0; i < ((tempX + 1) - (tempXsize - 18)); i++)
		{
			addXtile();
		}
	}
	if ((tempX + 1) - (tempXsize - 18) < 0)
	{
		for (int i = 0; i < ((tempXsize - 18) - (tempX + 1)); i++)
		{
			removeXtile();
		}
	}

	//crtXsize = (idxX + 19);
	//crtYsize = (idxY / idxX);

	for (int y = 0; y < _crtYsize; y++)
	{
		for (int x = 0; x < _crtXsize - 18; x++)
		{
			_tile[y][x].tilePos = { _tiles[x + (y * (_crtXsize - 18))].tilePos.x   , _tiles[x + (y * (_crtXsize - 18))].tilePos.y };
			_tile[y][x].layerPos1 = { _tiles[x + (y * (_crtXsize - 18))].layerPos1.x , _tiles[x + (y * (_crtXsize - 18))].layerPos1.y };
			_tile[y][x].layerPos2 = { _tiles[x + (y * (_crtXsize - 18))].layerPos2.x , _tiles[x + (y * (_crtXsize - 18))].layerPos2.y };
			_tile[y][x].geography = _tiles[x + (y * (_crtXsize - 18))].geography;
			_tile[y][x].rectYes = _tiles[x + (y * (_crtXsize - 18))].rectYes;
			_tile[y][x].rect = MakeRct(_tile[y][x].tilePos.x * TILESIZE, _tile[y][x].tilePos.y * TILESIZE, TILESIZE, TILESIZE);
		}
	}
	CloseHandle(file);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void mapManager::tileMapLoad(string strKey, int width, int height)
{
	load();
	image* img = IMG->find(strKey);
	if (!img) return;
	HDC hTempDC = CreateCompatibleDC(img->getMemDC());
	BITMAPINFO bI = { 0 };
	DWORD* bitmapData = nullptr;
	bI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bI.bmiHeader.biBitCount = 32;
	bI.bmiHeader.biWidth = width;
	bI.bmiHeader.biHeight = height;
	bI.bmiHeader.biCompression = BI_RGB;
	bI.bmiHeader.biPlanes = 1;
	HBITMAP hTempBitmap = CreateDIBSection(hTempDC, &bI, DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapData), 0, 0);
	if (hTempBitmap == nullptr)
	{
		DeleteDC(hTempDC);
		return;
	}
	HBITMAP hTempOBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);
	_shouldRenderUsingWindowCoords = TRUE;
	for (int y = 0; y < _crtYsize; y++)
	{
		for (int x = 0; x < _crtXsize - 18; x++)
		{
			IMG->frameRender("m", hTempDC, TILESIZE * x, TILESIZE * y, _tile[y][x].layerPos1.x, _tile[y][x].layerPos1.y);
			IMG->frameRender("m", hTempDC, TILESIZE * x, TILESIZE * y, _tile[y][x].layerPos2.x, _tile[y][x].layerPos2.y);
			//img->frameRender(hTempDC, TILESIZE * x, TILESIZE * y, tile[y][x].layerPos1.x, tile[y][x].layerPos1.y);
			//img->frameRender(hTempDC, TILESIZE * x, TILESIZE * y, tile[y][x].layerPos2.x, tile[y][x].layerPos2.y);
			//img->render(hTempDC, TILESIZE * x, TILESIZE * y, tile[y][x].layerPos1.x * 64, tile[y][x].layerPos1.y * 64, 64, 64);
		}
	}
	_shouldRenderUsingWindowCoords = FALSE;
	BitBlt(img->getMemDC(), 0, 0, width, height, hTempDC, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hTempDC, hTempOBitmap));
	DeleteDC(hTempDC);

}