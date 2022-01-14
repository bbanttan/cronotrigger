#pragma once
#include "character.h"
#include "gameNode.h"

class crono;
class lucca;
//class player;

enum class ATTACK_STATE
{
	INIT,
	RUNNING,
	END
};

struct tagEnemyImg
{

	image* idle;		//보통
	image* talk;		//대화
	image* move;		//이동
	image* rush;		//달려들기
	image* atk;			//공격/크리
	image* hit;			//피격
	image* down;		//쓰러짐	
	image* other1;		//부가연출1
	image* other2;		//부가연출2

};

struct tagEnemyAni

{
	animation* wait;	//전투전 대기중일 때
	animation* talk;	//대화할 때
	animation* idle_front, *idle_back, *idle_right, *idle_left;		//서있을 때의 애니		
	animation* move_front, *move_back, *move_right, *move_left;			//달릴 때의 애니			
	animation* SB_front, *SB_back, *SB_right, *SB_left;				//스탠바이 중의 애니		
	animation* rush_front, *rush_back, *rush_right, *rush_left;		//달려들 때의 애니		
	animation* atk_front, *atk_back, *atk_right, *atk_left;			//공격할 때의 애니	
	animation* hit_front, *hit_back, *hit_right, *hit_left;			//피격 때의 애니		
	animation* hit;	//피격 1장일 때
	animation* down;												//쓰러졌을 때의 애니		
	animation* Oth_F, *Oth_B, *Oth_L, *Oth_R;//임시로 만든 애니
};
	

class enemy : public gameNode
{
protected:
	STATE _State;
	tagStatus _Status;
	tagChaSet _ChaSet;
	tagEnemyImg _Img;
	tagEnemyAni _Ani;
	ATTACK_STATE _AtkState = ATTACK_STATE::INIT;

	int _id; // 캐릭터 ID
private:
	lucca* _lucca;
	//player* pl;

	int _atkAniStateCnt;
public:
	enemy();
	~enemy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	template <class T>
	bool atkSingleTarget(T* atkTarget,  enemy* attacker);

	//getter
	int getID() { return _id; }
	int getEnemyExp() { return _Status.exp; }
	int getEnemyHP() { return _Status.hp; }
	int getEnemyTP() { return _Status.techPoint; }
	int getTriggerTime() { return _ChaSet.triggerTime; }
	int getSpeed() { return _Status.speed; }
	POINT& getPt() { return _ChaSet.pt; }

	//setter
	void setTriggerTime(int triggerTime) { _ChaSet.triggerTime = triggerTime; }
	bool hitDamage(int damage)
	{
		if (_Status.hp == 0) return false;
	
		_Status.hp -= min(damage, _Status.hp);
		if (_Status.hp == 0)
		{
			_State = NORMAL_IDLE;
			return true;
		}
		return false;
	}
	void setPt(POINT pt) { _ChaSet.pt = pt; }
	void setState(STATE state) { _State = state; }
};

template<class T>
inline bool enemy::atkSingleTarget(T * atkTarget, enemy * attacker)
{
	//처음,중간에계속,끝 상태
	if (attacker->getEnemyHP() <= 0) return true;
	switch (_AtkState)
	{
		case  ATTACK_STATE::INIT:
		{
			POINT& _atkTargetPos = atkTarget->getPos();
			POINT& _attackerPos = attacker->getPt();
			if (Distance(_attackerPos.x, _attackerPos.y, _atkTargetPos.x, _atkTargetPos.y) > 30)
			{
				float tempTargetAngle;
				tempTargetAngle = Angle(_attackerPos.x, _attackerPos.y, _atkTargetPos.x, _atkTargetPos.y);
				//도착하는위치 이번프레임은 도착하는위치여야한다
				attacker->setPt({ (int)(_attackerPos.x + 10 * cosf(tempTargetAngle)), (int)(_attackerPos.y - 10 * sinf(tempTargetAngle)) });
			}
			else
			{
				_AtkState = ATTACK_STATE::RUNNING;
				_atkAniStateCnt = 0;
			}
			return false;
		}
		case  ATTACK_STATE::RUNNING:
		{
			if (_atkAniStateCnt == 0)
			{
				attacker->setState(BATTLE_ATK);
				attacker->_Ani.atk_front->start();
				_atkAniStateCnt++;
			}
			else if (_atkAniStateCnt > 0)
			{
				_atkAniStateCnt++;
				if (_atkAniStateCnt == 20)
				{
					atkTarget->hitDamage(RNG->getIntFromTo(9, 11));
					if (atkTarget->getHP() < 0)
					{
						atkTarget->hitDamage(atkTarget->getHP());
					}
				}
				
			}

			if (attacker->_Ani.atk_front->getCurrPlaylistIdx() == 0 && _atkAniStateCnt >= 30)
			{
				attacker->_Ani.atk_front->stop();
				attacker->setState(NORMAL_IDLE);
				attacker->_Ani.idle_front->start();
				_AtkState = ATTACK_STATE::END;
			}
			return false;
		}

		case  ATTACK_STATE::END:
		{
			_atkAniStateCnt = 0;
			_AtkState = ATTACK_STATE::INIT;
			return true;
		}
	}
	return true;
}
