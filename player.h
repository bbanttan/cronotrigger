#pragma once
#include "gameNode.h"
#include "crono.h"
#include "lucca.h"
#include "frog.h"
#include "marle.h"
class player : public gameNode
{
private:
	crono* _crono;
	lucca* _lucca;
	frog* _frog;
	marle* _marle;
	int _cha;//체인지
public:
	player() {};
	~player() {};

	HRESULT init();
	void release();
	void update();
	void render();

	//크로노 getter
	crono* getCrono() { return _crono; }
	//루카 getter
	lucca* getLucca() { return _lucca; }
	//개구리 getter
	frog* getFrog() { return _frog; }
	//마루 getter
	marle* getMarle() { return _marle; }
};