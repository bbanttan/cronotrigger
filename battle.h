#pragma once
#include "gameNode.h"
#include "progressBar.h"
#include <memory>
#include <queue>
#include <functional>

constexpr int maxTriggerTime = 2000;

class enemy;
class character;

enum class BATTLE_STATE
{
	NOT_START_YET,		// 배틀 시작전의 상태
	PLAYERS_POS,		// 배틀 위치를 잡을 때의 상태
	UI_SHOWUP,			// 배틀 UI 시작
	ATK_RDY,			// 캐릭터 무기 준비
	RUNNING,			// 배틀 진행
	ALL_ENEMIES_DOWN,	// 모든 적 다운, 승리 모션 진행
	ALL_PLAYERS_DOWN,	// 모든 플레이어 다운
	CHECK_REWARD_RDY,	// 얻은 아이템, 골드, exp 표시 준비
	CHECK_REWARD_START,	// 얻은 아이템, 골드, exp 표시
	UI_CLOSE,			// 배틀 ui 종료
	RES_PLAYER,			// 죽은 플레이어들 부활(플레이어가 전멸 아닐 때만)
	END					// 배틀 종료
};

class battle : public gameNode
{
private:

	queue<function<bool()>> _actionQueue;
	queue<string> _msgQueue;
	vector<character*>* _playerVector;
	vector<enemy*>*	_enemyVector;
	vector<POINT>*	_PlBattleStartPos;
	vector<POINT>* _EmBattleStartPos;
	character* _pl[3];
	enemy* _em[3];

	BATTLE_STATE _battleState;

	string _plName[3], _emName[3];
	shared_ptr<progressBar> _plTurnBar[3], _emTurnBar[3];
	int _blinkCount[3]; // 플레이어 캐릭터 HP 10 이하일 때 사용하는 변수
	bool _shouldPopUpAction[3], _shouldShowArrowPointer;

	POINT _arrowPointer; // x는 캐릭터 인덱스, y는 액션 인덱스

	int _msgIntervalCount;


	int _yOffset;

	TEXT_WINDOW_STATE _textWindowState;

	HDC _hTempDC;

	int _battleWindowOpeningCnt;
	int _battleWindowClipCount;
	int _enemyRemainingCnt;

	bool _isPlLost; // 플레이어가 패배했는가
public:
	battle();
	~battle() override;


	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	int& getBlinkCount(int i) { return _blinkCount[i]; }

	void updateBattleWindow();
	void renderBattleWindow();

	void init(vector<enemy*>* _enemyVector, vector<character*>* _playerVector,
			  vector<POINT>* _PlBattleStartPos, vector<POINT>* _EmBattleStartPos);

	BATTLE_STATE getBattleState() { return _battleState; }
	void resetBattleState() { _battleState = BATTLE_STATE::NOT_START_YET; }
	string findEnemyName(int id);

	// 플레이어 승리 시 false, 패배 시 true
	bool getResult() { return _isPlLost; }
};

