#pragma once
#include "gameNode.h"
#include "Astar.h"
struct tagStatus	//능력치
{
	int power;			//힘(근접무기공격력 + )	(최대 100)
	int magic;			//마력(마법공격력)(최대 100)
	int speed;			//배틀시 게이지 차징속도 증가(최대16)(헤이스트 등 특정 스킬로 인한 max 24)
	int stamina;		//체력(방어력 + / 최대 hp +)(최대 100)
	int hit;			//명중(원거리 공격력)
	int evasion;		//회피

	int atk;			//공격력(힘 + 무기공격력)
	int def, Mdef;		//방어력, 마법방어력

	int hp, maxHp;		//체력, 최대체력
	int mp, maxMp;		//mp, 최대mp
	int exp, maxExp;	//경험치
	int Lv;				//레벨
	int money;			//돈

	int ability;		//특수능력(아이템)

	int element;		//속성

	int condition;		//상태이상

	int techPoint;		//기술 숙련도 점수
};
enum STATE			//상태이넘
{
	NORMAL_IDLE,	//평상시
	NORMAL_WALK,	//걸을 때
	NORMAL_RUN,		//달릴 때
	NORMAL_TALK,	//대화할 때
	NORMAL_EVENT,	//이벤트
	NORMAL_READY,	//배틀 승리 후 주인공에게 모일 때
	GETTING_READY,	//준비(배틀 시작시 각자리로 날아갈 때)
	BATTLE_READY,	//대기
	BATTLE_MOVE,	//배틀시 일반움직임
	BATTLE_RUSH,	//배틀시 달려들기
	BATTLE_ATK,		//근거리와 원거리 구분짓기
	BATTLE_SKILL,	//스킬
	BATTLE_RETURN,	//배틀시 공격 후 돌아오기
	BATTLE_HIT,		//피격
	BATTLE_HELP,	//배틀 중 hp가 0이 되었을 때 그로기상태
	BATTLE_WIN,	//승리시(경험치, 골드 등 얻을 때)
	BATTLE_LOSE	//패배시(전부가 hp0이 될 때)
};
struct tagChaSet	//캐릭터 셋팅
{
	int triggerTime;	//전투 타이머 변수

	POINT pt;			//캐릭터 포인트
	POINT atk;			//캐릭터 공격시 공격지점 포인트

	POINT atkS;			//공격시 출발포인트
	POINT shadow;		//위치 지정용 그림자
	float Dis;			//자신과 상대 사이의 거리
	float angle;		//자신과 상대 사이의 각도
	float Sangle;		//출발지점과 자신 사이 거리

	RECT rc;			//캐릭터 렉트
	RECT Arc;			//공격 렉트

	image* img;			//이미지
	animation* ani;		//애니
	image* Sha;			//그림자 이미지

	image* Bimg;		//슬로우 등에 사용할 뒷배경 이미지
	animation* Bani;	//슬로우 등에 사용할 뒷배경 애니

	image* Oimg;		//(Other)혼란 등의 상태이상에 사용될 이미지
	animation* Oani;	//(Other)혼란 등의 상태이상에 사용될 애니

	string weapon;		//캐릭터 현재 무기
	string head;		//캐릭터 현재 머리방어구
	string armor;		//캐릭터 현재 갑옷
	string acc; 		//캐릭터 현재 액세서리
	string name;		//캐릭터 이름

	STATE state;		//스테이트 이넘
};

struct tagImage
{
	image* idle;		//보통
	image* walk;		//걷기
	image* run;			//달리기
	image* Bready;		//배틀 준비
	image* rush;		//달려들기
	image* atk;			//공격/크리
	image* atk2;		//공격2(원거리)
	image* spin;		//스핀(크로노 스킬)
	image* hit;			//피격
	image* hit2;		//피격2		
	image* win;			//승리포즈
	image* Bwin;		//배틀 승리
	image* downs;		//쓰러지기 직전
	image* down;		//쓰러짐
	image* cantRun;		//달리다 벽에 부딪힐 때	
	image* drink;		//마시기		
	image* handsUp;		//손 올리기	
	image* ladder;		//사다리		
	image* tied;		//묶임		
	image* what;		//궁금		
	image* skill;		//스킬
	image* skill2;		//스킬2
	image* skill3;		//스킬3
	image* smile;		//웃음
	image* glass;		//안경(루카)
};

struct tagAni
{
	//0 : front / 1 : back / 2 : right / 3 : left
	//보통은 위와 같이 적용되지만, 일부 이미지는 2장 / 1장만 있을 수도 있다

	animation* idle_0, *idle_1, *idle_2;					//서있을 때의 애니			0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* walk_0, *walk_1, *walk_2;					//걸을 때의 애니			0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* run_0, *run_1, *run_2;						//달릴 때의 애니			0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* Bready_0, *Bready_1, *Bready_3, *Bready_2;	//배틀 준비중의 애니		0 : 앞 / 1 : 뒤 / 3 : 왼쪽
	animation* SB_0, *SB_1, *SB_3, *SB_2;					//스탠바이 중의 애니		0 : 앞 / 1 : 뒤 / 3 : 왼쪽
	animation* rush_0, *rush_1, *rush_2;					//달려들 때의 애니			0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* atk_0, *atk_1, *atk_2;						//공격할 때의 애니			0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* cri_0, *cri_1, *cri_2;						//공격-크리티컬 때의 애니	0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* atk2_0, *atk2_1, *atk2_2;					//원거리 공격할 때의 애니	0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* hit_0, *hit_1, *hit_2;						//피격 때의 애니			0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* hit2_0, *hit2_2, *hit2_1;					//피격 때의 애니2			0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* spin;										//스핀 때의 애니
	animation* downs_0, *downs_1, *downs_2;					//쓰러지기 직전의 애니		0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* down;										//쓰러졌을 때의 애니		
	animation* down_0, *down_1, *down_2;					//쓰러졌을 때의 애니(방향이 나뉘어있을때 씀)
	animation* Bwin;										//이겼을 때의 애니(방향 상관없을 때 씀)
	animation* Bwin_0, *Bwin_1, *Bwin_3, *Bwin_2;			//이겼을 때의 애니			0 : 앞 / 1 : 뒤 / 3 : 왼쪽
	animation* win;											//승리포즈만 취할 때

	animation* cantRun_0, *cantRun_1, *cantRun_2;			//못달린다				0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* drink;										//마시기
	animation* handsUp_0, *handsUp_1;						//손들기					0 : 앞 / 1 : 뒤
	animation* ladder;										//사다리					
	animation* tied_0, *tied_1, *tied_2;					//묶임					0 : 앞 / 1 : 뒤 / 2 : 오른쪽
	animation* what_0, *what_1, *what_2;					//?						0 : 앞 / 1 : 뒤 / 2 : 오른쪽

	animation* skill_0, *skill_1, *skill_2, *skill_3;		//스킬
	animation* skill2_0, *skill2_1, *skill2_2, *skill2_3;	//스킬2
	animation* skill3_0, *skill3_1, *skill3_2, *skill3_3;	//스킬3
	animation* glass_0, *glass_2;							//안경 애니

	animation*  oneArr_0, *oneArr_1, *oneArr_2;	//한장의 애니일 때 / 각 숫자는 세로로 몇째줄인지 알려줌
	animation* twoArrT_0, *twoArrT_1, *twoArrT_2;	//두장의 애니일 때 / 반복할 때
	animation* twoArrF_0, *twoArrF_1, *twoArrF_2;	//두장의 애니일 때 / 반복 안할 때
};
enum class STATE_BATTLE
{
	INIT,	//배틀 준비
	READY,	//게이지 다찼을 때
	MOVE,	//근거리 공격시 이동
	ATK,	//근거리/원거리 이동
	HIT,	//피격
	RETURN,	//돌아가기(후 READY)
	END		//종료(승리)
};
class character : public gameNode
{
protected:
	tagStatus _status;		//능력치
	tagStatus _itemSt;	//아이템 능력치
	tagChaSet _chaSet;		//셋팅

	tagImage im;
	tagAni an;

	bool Cleft, _run;	//좌우반전, 달리기
	int _T;		//방향전환용 int(turn) / 0:front , 1:back , 2:right , 3:left
	int Cc;		//idle 상태의 눈깜빡임 카운트
	int _rcW, _rcH;	//캐릭터 렉트용 x,y축 갈이
	int hitCol;		//피격시 하얗게 될 때를 보이는 int
	int Dcheck;		//다운 체크용 int
	bool aniReset;	//애니메이션 스탑용 bool
	bool Latk;		//원거리 공격 체크용 bool
	int dia;		//(임시)포인트 확인용 지름
	int _moveC;
	int _id; // 캐릭터 ID

	STATE_BATTLE state_b = STATE_BATTLE::INIT;
	/////////// Astar용 //////////////
	float a, b, reach;
	vector<POINT> Path;
	bool Acheck;//a스타 시작 체크용
	int AmoveC;//a스타 시작 제한용
	int muonX, muonY;
	////////// Astar용 ///////////////
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//setter

//공격력 setter
	void plusAtk(int atk) { _status.atk += atk; }
	void setAtk(int atk) { _status.atk = atk; }
	//마력 +
	void plusMagic(int magic) { _status.magic += magic; }
	void setMagic(int magic) { _status.magic = magic; }//마력 setter
	//힘 +
	void plusPower(int power) { _status.power += power; }
	void setPower(int power) { _status.power = power; }//힘 setter
	//스피드 +
	void plusSpeed(int speed) { _status.speed += speed; }
	void setSpeed(int speed) { _status.speed = speed; }//스피드 setter
	//체력 +
	void plusStamina(int stamina) { _status.stamina += stamina; }
	void setStamina(int stamina) { _status.stamina = stamina; }//체력 setter
	//민첩 +
	void plusHit(int hit) { _status.hit += hit; }
	void setHit(int hit) { _status.hit = hit; }//민첩 setter
	//회피 +
	void plusEvasion(int evasion) { _status.evasion += evasion; }
	void setEvasion(int evasion) { _status.evasion = evasion; }//회피 setter

	//컨디션(느려짐 등 상태이상) setter
	void setCondition(int condition) { _status.condition = condition; }

	//데미지(hp) -
	void hitDamage(int damage) { _status.hp -= damage; }
	//최대hp +
	void plusMaxHP(int maxHP) { _status.maxHp += maxHP; }
	void setMaxHP(int maxHP) { _status.maxHp = maxHP; }//최대 hp setter

	//mp -
	void minusMP(int mp) { _status.mp -= mp; }
	//최대mp +
	void plusMaxMP(int maxMP) { _status.maxMp += maxMP; }
	void setMaxMP(int maxMP) { _status.maxMp = maxMP; }//최대 mp setter

	//소지금 +
	void plusMoney(int money) { _status.money += money; }
	void setMoney(int money) { _status.money = money; }//소지금 setter

	//경험치 +
	void plusExp(int exp) { _status.exp += exp; }
	//최대경험치 setter
	void setMaxExp(int maxExp) { _status.maxExp = maxExp; }


	//피격 등에 설정시킬 STATE setter
	void setState(STATE state) { _chaSet.state = state; }

	//방향전환 T setter
	void setT(int T) { _T = T; }

	//트리거타임 setter
	void setTriggerTime(int triggerTime) { _chaSet.triggerTime = triggerTime; }

	//아이템 공격력 setter
	void setItemAtk(int atk) { _itemSt.atk = atk; }
	//아이템 마력 setter
	void setItemMagic(int magic) { _itemSt.magic = magic; }
	//아이템 힘 setter
	void setItemPower(int power) { _itemSt.power = power; }
	//아이템 스피드 setter
	void setItemSpeed(int speed) { _itemSt.speed = speed; }
	//아이템 체력 setter
	void setItemStamina(int stamina) { _itemSt.stamina = stamina; }
	//아이템 민첩 setter
	void setItemHit(int hit) { _itemSt.hit = hit; }
	//아이템 회피 setter
	void setItemEvasion(int evasion) { _itemSt.evasion = evasion; }
	//아이템 최대 hp setter
	void setItemMaxHP(int maxHP) { _itemSt.maxHp = maxHP; }
	//아이템 최대 mp setter
	void setItemMaxMP(int maxMP) { _itemSt.maxMp = maxMP; }
	//아이템 특수능력 setter
	void setItemAbility(int ability) { _itemSt.ability = ability; }

	void setAniIndex(int aniIndex) { _chaSet.ani->setCurrPlaylistIdx(aniIndex); }
	//getter
	// 캐릭터 ID getter
	int getID() { return _id; }

	//트리거타임 getter
	int getTriggerTime() { return _chaSet.triggerTime; }

	//현재 공격력
	int getAtk() { return _status.atk; }
	//현재 마력
	int getMagic() { return _status.magic; }
	//현재 힘
	int getPower() { return _status.power; }
	//현재 스피드
	int getSpeed() { return _status.speed; }
	//현재 체력
	int getStamina() { return _status.stamina; }
	//현재 명중
	int getHit() { return _status.hit; }
	//현재 회피
	int getEvasion() { return _status.evasion; }

	//현재 컨디션
	int getCondition() { return _status.condition; }

	//현재 hp
	int getHP() { return _status.hp; }
	//현재 maxHP
	int getMaxHP() { return _status.maxHp; }

	//현재 mp
	int getMP() { return _status.mp; }
	//현재 maxMP
	int getMaxMP() { return _status.maxMp; }

	//현재 소지금
	int getMoney() { return _status.money; }

	//현재 경험치
	int getExp() { return _status.exp; }
	//현재 최대경험치
	int getMaxExp() { return _status.maxExp; }
	//현재 레벨
	int getLV() { return _status.Lv; }

	//피격 등을 확인할 STATE getter
	STATE getState() { return _chaSet.state; }

	//방향확인용 T getter
	int getT() { return _T; }

	//아이템 공격력 setter
	int getItemAtk() { return _itemSt.atk; }
	//아이템 마력 setter
	int getItemMagic() { return _itemSt.magic; }
	//아이템 힘 setter
	int getItemPower() { return _itemSt.power; }
	//아이템 스피드 setter
	int getItemSpeed() { return  _itemSt.speed; }
	//아이템 체력 setter
	int getItemStamina() { return _itemSt.stamina; }
	//아이템 민첩 setter
	int getItemHit() { return  _itemSt.hit; }
	//아이템 회피 setter
	int getItemEvasion() { return  _itemSt.evasion; }
	//아이템 최대 hp setter
	int getItemMaxHP() { return  _itemSt.maxHp; }
	//아이템 최대 mp setter
	int getItemMaxMP() { return  _itemSt.maxMp; }
	//아이템 특수능력 setter
	int getItemAbility() { return _itemSt.ability; }

	int getAniIndex() { return _chaSet.ani->getCurrPlaylistIdx(); }

	POINT& getPos() { return _chaSet.pt; }
	POINT& getAtkPos() { return _chaSet.atk; }
	POINT& getAtkSPos() { return _chaSet.atkS; }


	void stopAni() { _chaSet.ani->stop(); }
	//단순하게 크로노 따라가기 crono -> 따라갈 대상(보통 첫번째 캐릭터는 크로노) / Count -> 따라가기 시작하는 거리 및 시작하는 시간에 곱하는 값
	//void follow(POINT crono, int Count);
	//A스타 설정하기
	void AstarCheck(int Key, POINT crono, int Count);
	//A스타로 따라가기
	void AstarFollow(POINT crono, int Count);
	bool atkCheck()
	{
		if (_chaSet.state == BATTLE_ATK)
		{
			if (_chaSet.ani->getCurrPlaylistIdx() == 1 || _chaSet.ani->getCurrPlaylistIdx() == 2)return true;
			else return false;
		}
		else return false;
	}
	bool white()
	{
		if (_chaSet.state == BATTLE_HIT && hitCol % 10 == 0)return true;
		else return false;
	}
	//0번 아니면서 false면 애니 스탑 상태
	bool aniPlayCheck()
	{
		if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)return false;//애니메이션 번호가 0번이 아니고 멈춰있다면 false를 리턴해라
		else return true;//그 외는 true를 리턴해라
	}
	//0번이면서 false면 애니 스탑 상태
	bool aniZeroCheck()
	{
		if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE)return false;//애니메이션 번호가 0번이고 멈춰있다면 false를 리턴해라
		else return true;//그 외는 true를 리턴해라
	}
	//atkTarget : 상대방 / attacker : 플레이어 / 행동 int등
	template <class T>
	bool atkSingleTarget(T* atkTarget, character* attacker);
};
template<class T>
inline bool character::atkSingleTarget(T * atkTarget, character * attacker)
{
	//처음,중간에계속,끝 상태
	if (attacker->getHP() <= 0) return true;
	switch (state_b)
	{
	case STATE_BATTLE::INIT:

		if (attacker->getState() == BATTLE_READY)state_b = STATE_BATTLE::READY;
		return false;
		break;
	case STATE_BATTLE::READY:
		*attacker->getAtkPos() = atkTarget->getPt();	//공격 지점 = 상대 포인트
		*attacker->getAtkSPos() = attacker->getPos();	//공격 시작 지점 = 공격자 포인트

		float tAngle = Angle(static_cast<float>(_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y), static_cast<float>(_chaSet.atk.x), static_cast<float>(_chaSet.atk.y));
		float tDis = Distance(static_cast<float>(_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y), static_cast<float>(_chaSet.atk.x), static_cast<float>(_chaSet.atk.y));
		float _pi = PI / 9;
		if (_pi * 3 < tAngle && tAngle <= _pi * 6) { _T = 1; }//위쪽보기
		else if (-_pi * 3 < tAngle && tAngle <= _pi * 3) { _T = 2; }//오른쪽보기
		else if (-_pi * 3 >= tAngle && tAngle > -_pi * 6) { _T = 0; }//아래쪽보기
		else { _T = 3; }//왼쪽 보기

		if (attacker->getState() == BATTLE_MOVE || attacker->getState()==BATTLE_RUSH)state_b = STATE_BATTLE::MOVE;
		else if (attacker->getState() == BATTLE_ATK)STATE_BATTLE::ATK;
		else if (attacker->getState() == BATTLE_HIT)STATE_BATTLE::HIT;
		else if (attacker->getState() == BATTLE_WIN)STATE_BATTLE::END;
		return false;
		break;
	case STATE_BATTLE::MOVE:
		if (attacker->getState() == BATTLE_ATK)state_b = STATE_BATTLE::ATK;
		return false;
		break;
	case STATE_BATTLE::ATK:
		if (attacker->getState() == BATTLE_READY)state_b = STATE_BATTLE::READY;
		else if (attacker->getState() == BATTLE_RETURN)state_b = STATE_BATTLE::RETURN;

		return false;
		break;
	case STATE_BATTLE::HIT:
		if (attacker->getState() == BATTLE_READY)state_b = STATE_BATTLE::READY;

		return false;
		break;
	case STATE_BATTLE::RETURN:
		if (attacker->getState() == BATTLE_READY)state_b = STATE_BATTLE::READY;

		return false;
		break;
	case STATE_BATTLE::END:
		if (attacker->getState() != BATTLE_WIN)STATE_BATTLE::INIT;
		return false;
		break;
	}
	return true;
}
