#include "stdafx.h"
#include "Astar.h"

HRESULT Astar::init()
{
	return S_OK;
}

void Astar::release()
{
	vtCrtTile.clear();
	vtClose.clear();
	vtOpen.clear();
}

void Astar::update()
{


}

void Astar::render()
{

}

vector<POINT> Astar::pathFinder(POINT _end, POINT _start)
{
	//1.열린목록엔 아무것도 없고 첫 시작점을 하나 넣는다.
	//2.열린목록에서 비용이 가장 적게드는 노드를 검색(처음에는 스타트니깐 스타트 노드)
	//3.선택된 노드 닫힌 목록으로 넣고 열린목록에서 제외
	//4.선택된 노드를 기준으로 8방향노드를 찾음
	//5.이웃노드 검색시 벽 제외
	//6.검색된 이웃노드를 순회
	//7.이미 열린목록에 무언가있고 내가 그쪽으로 갈때와 아니면 그곳을 부모로둘때
	//글로가는것중에 어느것이 값이 가장싼지 확인하고 더 싸면 그곳을 부모로 재지정
	//8.

	endPos = _start;		//도착점
	crtPos = _end;		//현재 나의위치

	//1
	nodeInfo _node;

	vtOpen.push_back(_node);
	vtOpen[vtOpen.size() - 1].Xnode = crtPos.x;
	vtOpen[vtOpen.size() - 1].Ynode = crtPos.y;
	vtOpen[vtOpen.size() - 1].F = (abs(endPos.x - crtPos.x) + abs(endPos.y - crtPos.y)) * 10;

	while (1)
	{

		//주변이 막혀있을때
		if (!vtOpen.size() && vtClose.size() != 0)
		{
			vector<POINT> err;
			POINT p_err = MakePt(-100, -100);
			err.push_back(p_err);
			return err;
		}

		//############### 가장 F값이 작은노드찾는 구문 시작 ###############
		sort(vtOpen.begin(), vtOpen.end(), cmp());
		vtClose.push_back(vtOpen[0]);
		vtOpen.erase(vtOpen.begin());
		//############### 가장 F값이 작은노드찾는 구문 종료 ###############

		//############### 도착했을때 구문 시작 ###############
		if (vtClose[vtClose.size() - 1].Xnode == endPos.x &&
			vtClose[vtClose.size() - 1].Ynode == endPos.y)
		{
			POINT gg = vtClose[vtClose.size() - 1].parentNode;
			vector<POINT> Path;

			while (1)
			{
				for (int i = 0; i < vtClose.size(); i++)
				{
					if (gg.x == crtPos.x && gg.y == crtPos.y)
					{
						//vtOpen.clear();
						//vtClose.clear();
						return Path;
					}
					if (gg.x == vtClose[i].Xnode && gg.y == vtClose[i].Ynode)
					{
						Path.push_back(vtClose[i].parentNode);
						gg = vtClose[i].parentNode;
					}
				}
			}
		}
		//############### 도착했을때 구문 종료 ###############


		//############### 주변노드 탐색 및 생성 구문 시작 ###############

		POINT crtNode = { vtClose[vtClose.size() - 1].Xnode, vtClose[vtClose.size() - 1].Ynode };

		for (int i = -1; i < 2; i++) //4
		{
			for (int e = -1; e < 2; e++)
			{
				//############### 무시처리 구문 시작 ###############
				//벽밖으로 나갈때
				if (crtNode.x + e < 0 || crtNode.x + e >= (_crtXsize - 18)) continue;
				if (crtNode.y + i < 0 || crtNode.y + i >= _crtYsize) continue;

				bool forContinue = false;

				//string tileAttribute = vtCrtTile[(crtNode.x + e) + (crtNode.y + i) * 25].tile->getTileAttribute();
				int tileAttribute = _tile[crtNode.y + i][crtNode.x + e].geography;

				if (tileAttribute == 2) continue;

				for (int j = 0; j < vtClose.size(); j++) //현재노드는 스킵
				{
					if (vtClose[j].Xnode == (vtClose[vtClose.size() - 1].Xnode + e) &&
						vtClose[j].Ynode == (vtClose[vtClose.size() - 1].Ynode + i))
					{
						forContinue = true;
						break;
					}
				}
				if (forContinue) continue;

				//5
				//만약 현재 만들려고하는곳이 대각선인데, 원점에서 그 방향쪽으로 직선으로 벽이있다면 컨티뉴
				if (e == -1 && i == -1 && (_tile[crtNode.y][crtNode.x - 1].geography == 2 || _tile[crtNode.y - 1][crtNode.x].geography == 2)) continue;
				if (e == 1 && i == -1 && (_tile[crtNode.y][crtNode.x + 1].geography == 2 || _tile[crtNode.y - 1][crtNode.x].geography == 2)) continue;
				if (e == -1 && i == 1 && (_tile[crtNode.y][crtNode.x - 1].geography == 2 || _tile[crtNode.y + 1][crtNode.x].geography == 2)) continue;
				if (e == 1 && i == 1 && (_tile[crtNode.y][crtNode.x + 1].geography == 2 || _tile[crtNode.y + 1][crtNode.x].geography == 2)) continue;
				//############### 무시처리 구문 종료 ###############

				//############### 회유정책하는 구문 시작 ###############
				if (vtOpen.size() != 0)
				{
					for (int i = 0; i < vtOpen.size(); i++)
					{
						//원점과의 거리
						int orgToDistanceX = vtOpen[i].Xnode - crtNode.x;
						int orgToDistanceY = vtOpen[i].Ynode - crtNode.y;
						bool diaLine = false;
						int gLine;

						//인접사각형이 아닐때
						if (!((orgToDistanceX >= -1 && orgToDistanceX <= 1) &&
							(orgToDistanceY >= -1 && orgToDistanceY <= 1)))
						{
							continue;
						}

						//대각선일때
						if (
							(orgToDistanceX == -1 && orgToDistanceY == -1) ||
							(orgToDistanceX == 1 && orgToDistanceY == -1) ||
							(orgToDistanceX == -1 && orgToDistanceY == 1) ||
							(orgToDistanceX == 1 && orgToDistanceY == 1))
						{
							gLine = vtClose[vtClose.size() - 1].G + 14;
							diaLine = true;
						}
						else
						{
							gLine = vtClose[vtClose.size() - 1].G + 10;
						}

						if (gLine < vtOpen[i].G || (gLine <= vtOpen[i].G && diaLine))
						{
							vtOpen[i].parentNode.x = crtNode.x;
							vtOpen[i].parentNode.y = crtNode.y;
							vtOpen[i].G = gLine;
							vtOpen[i].H = (abs(vtOpen[i].Xnode - endPos.x) + abs(vtOpen[i].Ynode - (endPos.y))) * 10;
							vtOpen[i].F = vtOpen[i].G + vtOpen[i].H;
						}
					}
				}
				//############### 회유정책하는 구문 종료 ###############

				//############### F계산 및 주변노드 열린목록에 추가 구문 시작 ###############
				for (int n = 0; n < vtOpen.size(); n++)	//기존에있는 목록은 스킵
				{
					if (vtOpen[n].Xnode == crtNode.x + e &&
						vtOpen[n].Ynode == crtNode.y + i)
					{
						forContinue = true;
						break;
					}
				}
				if (forContinue) continue;

				nodeInfo node;
				vtOpen.push_back(node);

				vtOpen[vtOpen.size() - 1].Xnode = crtNode.x + e;
				vtOpen[vtOpen.size() - 1].Ynode = crtNode.y + i;

				//부모설정해주기
				vtOpen[vtOpen.size() - 1].parentNode.x = crtNode.x;
				vtOpen[vtOpen.size() - 1].parentNode.y = crtNode.y;

				//G비용 넣어주기
				vtOpen[vtOpen.size() - 1].G =
					(int)(vtClose[vtClose.size() - 1].G +
					(Distance(
					(_tile[vtOpen[vtOpen.size() - 1].Ynode][vtOpen[vtOpen.size() - 1].Xnode].tilePos.x * 64) + TILESIZE / 2,
						(_tile[vtOpen[vtOpen.size() - 1].Ynode][vtOpen[vtOpen.size() - 1].Xnode].tilePos.y * 64) + TILESIZE / 2,
						(vtClose[vtClose.size() - 1].Xnode * 64) + TILESIZE / 2,
						(vtClose[vtClose.size() - 1].Ynode * 64) + TILESIZE / 2 > 40 ? 14 : 10)));

				//H비용 넣어주기
				vtOpen[vtOpen.size() - 1].H =
					(abs((crtNode.x + e) - endPos.x) +
						abs((crtNode.y + i) - endPos.y)) * 10;

				//F비용 넣어주기
				vtOpen[vtOpen.size() - 1].F = vtOpen[vtOpen.size() - 1].G + vtOpen[vtOpen.size() - 1].H;


				//############### F계산 및 주변노드 열린목록에 추가 구문 종료 ###############
			}
		}
		//############### 주변노드 탐색 및 생성 구문 종료 ###############
	}
}
