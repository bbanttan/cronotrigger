#pragma once
#include "gameNode.h"
#include<algorithm>

struct nodeInfo
{
	int F, G, H, Xnode, Ynode;
	POINT parentNode;
};

class Astar : public gameNode
{
private:

	POINT endPos;		//도착점
	POINT crtPos;		//현재 나의위치

	vector<nodeInfo>	vtCrtTile;	//현재맵상태
	vector<nodeInfo>	vtOpen;		//열린목록
	vector<nodeInfo>	vtClose;	//닫힌목록

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	vector<POINT> pathFinder(POINT _start, POINT _end);

	struct cmp
	{
		bool operator()(const nodeInfo &_f1, const nodeInfo &_f2) const
		{
			return _f1.F < _f2.F;
		}
	};
};

