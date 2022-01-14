#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	_crono = new crono;
	_crono->init();
	_lucca = new lucca;
	_lucca->init();
	_lucca->getPos() = _crono->getPos();
	_frog = new frog;
	_frog->init();
	_marle = new marle;
	_marle->init();
	return S_OK;
}

void player::release()
{
	_crono->release();
	SAFE_DEL(_crono);
	_lucca->release();
	SAFE_DEL(_lucca);
	_frog->release();
	SAFE_DEL(_frog);
	_marle->release();
	SAFE_DEL(_marle);
}

void player::update()
{
	_crono->update();
	_lucca->update();
	_frog->update();
	_marle->update();
	if (!_isInBattle)
	{
		if (KEY->down('0'))
		{
			//if (_cha == 0)_cha = 1;
			//else if (_cha == 1)_cha = 2;
			//else if (_cha == 2)_cha = 3;
			//else if (_cha == 3)_cha = 0;
			_cha++;
			if (_cha == 3)_cha = 0;
		}
		if (_cha == 0)_crono->keySetting();
		else if (_cha == 1)_lucca->keySetting();
		else if (_cha == 2)_frog->keySetting();
		else if (_cha == 3)_marle->keySetting();

		if (_crono->atkCheck()) { _frog->setState(BATTLE_HIT); }
		else if (_frog->getState() == BATTLE_HIT && !_crono->atkCheck()) { _frog->setState(NORMAL_IDLE); }

		_lucca->AstarCheck('1', _crono->getPos(), 1);
		_frog->AstarCheck('2', _crono->getPos(), 2);
		_marle->AstarCheck('3', _crono->getPos(), 3);

		_lucca->AstarFollow(_crono->getPos(), 1);
		_frog->AstarFollow(_crono->getPos(), 2);
		_marle->AstarFollow(_crono->getPos(), 3);
	}
}

void player::render()
{
	
	_crono->render();
	_lucca->render();
	_frog->render();
	_marle->render();
}

