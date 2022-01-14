#pragma once
//노래 부르는 연습보스 

//#include "gameNode.h"
#include "gameScene.h"
#include "mapManager.h"
#include "gato.h"
#include <memory>


#define BOSSTILEX 16
#define BOSSTILEY 16


class battle;

class bossGatoStage : public mapManager
{
private:
	shared_ptr<battle> _battle;
	shared_ptr<gato> _gato;

	vector<character*> _playerVector;
	vector<enemy*>	_enemyVector;
	vector<POINT> _PlBattleStartPos;
	vector<POINT> _EmBattleStartPos;

	int _yOffset;
	int _dialogueStateNum;


protected:
	float			 _cameraX;
	float			 _cameraY;
public:
	bossGatoStage() {}
	~bossGatoStage() override {}
	
	virtual HRESULT init() override;
	virtual void update() override;
	virtual void release() override;
	virtual void render() override;

};

