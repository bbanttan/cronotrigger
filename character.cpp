#include "stdafx.h"
#include "character.h"

HRESULT character::init()
{
	return S_OK;
}

void character::release()
{
}

void character::update()
{

}

void character::render()
{
}
/*
void character::follow(POINT crono, int Count)
{
	
	float tAngle = Angle(static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y), static_cast<float>(crono.x), static_cast<float>(crono.y));
	float tDis = Distance(static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y), static_cast<float>(crono.x), static_cast<float>(crono.y));
	float _pi = PI / 9;
	if (_pi * 3 < tAngle && tAngle <= _pi * 6) { _T = 1; }//위쪽보기
	else if (-_pi * 3 < tAngle && tAngle <= _pi * 3) { _T = 2; }//오른쪽보기
	else if (-_pi * 3 >= tAngle && tAngle > -_pi * 6) { _T = 0; }//아래쪽보기
	else { _T = 3; }//왼쪽 보기
	_moveC++;
	switch (_chaSet.state)
	{
	case NORMAL_IDLE:
		if (tDis >= 192 * Count && _moveC% 10 * Count == 0)
		{
			_chaSet.state = NORMAL_RUN;
			_moveC = 0;
		}
		else if (tDis >= 96 * Count && _moveC% 10 * Count == 0)
		{
			_chaSet.state = NORMAL_WALK;
			_moveC = 0;
		}

		break;

	case NORMAL_WALK:
		if (tDis >= 192 * Count && _moveC% 10 * Count == 0)
		{
			_chaSet.state = NORMAL_RUN;
			_moveC = 0;
		}
		else if (tDis < 96 * Count && _moveC% 10 * Count == 0)
		{
			_chaSet.state = NORMAL_IDLE;
			_moveC = 0;
		}
		_chaSet.pt.x += cosf(tAngle) * 5;
		_chaSet.pt.y += -sinf(tAngle) * 5;
		break;

	case NORMAL_RUN:
		if (tDis >= 96 * Count && tDis < 192 * Count && _moveC% 10 * Count == 0)
		{
			_chaSet.state = NORMAL_WALK;
			_moveC = 0;
		}
		else if (tDis < 96 * Count && _moveC% 10 * Count == 0)
		{
			_chaSet.state = NORMAL_IDLE;
			_moveC = 0;
		}
		_chaSet.pt.x += cosf(tAngle) * 8;
		_chaSet.pt.y += -sinf(tAngle) * 8;
		break;
	}
}
*/

void character::AstarCheck(int Key, POINT crono, int Count)
{
	if (KEY->down(Key) && !_isChrUnmovable)
	{
		float _checkDis = Distance(_chaSet.pt.x, _chaSet.pt.y, crono.x, crono.y);

		if (_checkDis <= TILESIZE * Count)
		{
			_chaSet.ani->stop();
			_chaSet.state = NORMAL_IDLE;
			AmoveC = 0;
		}
		else if (_checkDis > TILESIZE * Count && _checkDis <= TILESIZE * 2 * Count)
		{
			_chaSet.ani->stop();
			_chaSet.state = NORMAL_WALK;
			Acheck = true;
		}
		else if (_checkDis > TILESIZE * 2 * Count)
		{
			_chaSet.ani->stop();
			_chaSet.state = NORMAL_RUN;
			Acheck = true;
		}

		Astar* astar;
		astar = new Astar;

		Path = astar->pathFinder({ _chaSet.pt.x / TILESIZE, _chaSet.pt.y / TILESIZE }, { crono.x / TILESIZE, crono.y / TILESIZE });
		astar->release();
		delete astar;
		a = b = reach = 0;
	}
}

void character::AstarFollow(POINT crono, int Count)
{

	/*
	if (!_isChrUnmovable)
	{
		if (!Acheck)
		{
			AmoveC++;
			if (AmoveC % 30 * Count == 0)
			{
				_chaSet.ani->stop();
				_chaSet.state = NORMAL_IDLE;
				AmoveC = 0;
			}
			if (AmoveC % 30 * Count == 0 && _checkDis > TILESIZE * Count && _checkDis <= TILESIZE * 2 * Count)
			{
				_chaSet.ani->stop();
				_chaSet.state = NORMAL_WALK;
				Acheck = true;
			}
			if (AmoveC % 30 * Count == 0 && _checkDis > TILESIZE * 2 * Count)
			{
				_chaSet.ani->stop();
				_chaSet.state = NORMAL_RUN;
				Acheck = true;
			}
		}

		else if (Acheck)
		{
			Astar* astar;
			astar = new Astar;

			Path = astar->pathFinder({ _chaSet.pt.x / TILESIZE, _chaSet.pt.y / TILESIZE }, { crono.x / TILESIZE, crono.y / TILESIZE });
			astar->release();
			delete astar;
			a = b = reach = 0;

			Acheck = false;
			AmoveC = 0;

		}
	}
	*/
	if (Path.size() != 0)
	{

		if (reach <= 5.0f)
		{

			a = (float)Path[Path.size() - 1].x;
			b = (float)Path[Path.size() - 1].y;
			Path.pop_back();

			a = (a * TILESIZE) + TILESIZE / 2;
			b = (b * TILESIZE) + TILESIZE / 2;

		}

		if (_chaSet.state == NORMAL_RUN)
		{
			muonX = (int)(cosf(atan2f(b - (float)_chaSet.pt.y, a - (float)_chaSet.pt.x)) * 8);
			muonY = (int)(-sinf(-atan2f(b - (float)_chaSet.pt.y, a - (float)_chaSet.pt.x)) * 8);
		}
		else if (_chaSet.state == NORMAL_WALK)
		{
			muonX = (int)(cosf(atan2f(b - (float)_chaSet.pt.y, a - (float)_chaSet.pt.x)) * 5);
			muonY = (int)(-sinf(-atan2f(b - (float)_chaSet.pt.y, a - (float)_chaSet.pt.x)) * 5);
		}
		else if (_chaSet.state == NORMAL_IDLE)
		{
			muonX = 0;
			muonY = 0;
		}
		_chaSet.pt.x += muonX;
		_chaSet.pt.y += muonY;

		if (muonX < 0) _T = 3;
		else if (muonX > 0) _T = 2;
		else if (muonY < 0) _T = 1;
		else if (muonY > 0) _T = 0;

		if (Path.size() == 0)
		{
			_chaSet.state = NORMAL_IDLE;
			_chaSet.ani->stop();
		}

		reach = Distance((float)_chaSet.pt.x, (float)_chaSet.pt.y, a, b);

	}

}
