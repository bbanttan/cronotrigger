#include "stdafx.h"
#include "battle.h"
#include "character.h"
#include "enemy.h"
#include "bossGatoStage.h"

battle::battle()
{
}

battle::~battle()
{
}

HRESULT battle::init() { return S_OK; }

void battle::release()
{
}

void battle::update()
{
	switch (_battleState)
	{
		case BATTLE_STATE::NOT_START_YET:
		{
			if (_isInBattle)
			{
				_battleState = BATTLE_STATE::PLAYERS_POS;
			}
			break;
		}
		case BATTLE_STATE::PLAYERS_POS:
		{

			POINT& _cronoPos = _pl[0]->getPos();
			POINT& _luccaPos = _pl[1]->getPos();

			POINT _cronoStartPos = (*_PlBattleStartPos)[0];
			POINT _luccaStartPos = (*_PlBattleStartPos)[1];

			bool _ChkMove = true;

			if (Distance(_cronoPos.x, _cronoPos.y, _cronoStartPos.x, _cronoStartPos.y) > FLT_EPSILON)
			{
				
				_cronoPos.x += (_cronoPos.x > _cronoStartPos.x) ? -min(abs(_cronoPos.x - _cronoStartPos.x), 10) : min(abs(_cronoPos.x - _cronoStartPos.x), 10);
				_cronoPos.y += (_cronoPos.y > _cronoStartPos.y) ? -min(abs(_cronoPos.y - _cronoStartPos.y), 10) : min(abs(_cronoPos.y - _cronoStartPos.y), 10);
				_ChkMove = false;
			}

			else if (Distance(_cronoPos.x, _cronoPos.y, _cronoStartPos.x, _cronoStartPos.y) <= FLT_EPSILON)
			{
				_pl[0]->setT(1);
			}

			if (Distance(_luccaPos.x, _luccaPos.y, _luccaStartPos.x, _luccaStartPos.y) > FLT_EPSILON)
			{
				_luccaPos.x += (_luccaPos.x > _luccaStartPos.x) ? -min(abs(_luccaPos.x - _luccaStartPos.x), 10) : min(abs(_luccaPos.x - _luccaStartPos.x), 10);
				_luccaPos.y += (_luccaPos.y > _luccaStartPos.y) ? -min(abs(_luccaPos.y - _luccaStartPos.y), 10) : min(abs(_luccaPos.y - _luccaStartPos.y), 10);
				_ChkMove = false;
			}

			else if (Distance(_luccaPos.x, _luccaPos.y, _luccaStartPos.x, _luccaStartPos.y) <= FLT_EPSILON)
			{
				_pl[1]->setT(3);
			}
			
			if (_ChkMove)
			{
				_battleState = BATTLE_STATE::UI_SHOWUP;
			}
			break;
		}
		case BATTLE_STATE::UI_SHOWUP:
		{
			if (_textWindowState == TEXT_WINDOW_STATE::INVISIBLE)
			{
				_battleWindowOpeningCnt++;
				if (_battleWindowOpeningCnt >= 30)
				{
					for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
					{
						if (plIndex == 0)
						{
							_pl[plIndex]->setTriggerTime(1000);
						}
						else _pl[plIndex]->setTriggerTime(0);

						_plTurnBar[plIndex] = make_shared<progressBar>();
						_plTurnBar[plIndex]->init(817, 44 + 48 * plIndex, 160, 32);
						_plTurnBar[plIndex]->setGauge(_pl[plIndex]->getTriggerTime(), maxTriggerTime);
					}

					_textWindowState = TEXT_WINDOW_STATE::OPENING;
				}
			}

			else if (_textWindowState == TEXT_WINDOW_STATE::OPENING)
			{
				_battleWindowClipCount++;
				if (_battleWindowClipCount == 9)
				{
					_textWindowState = TEXT_WINDOW_STATE::VISIBLE;
					_battleState = BATTLE_STATE::ATK_RDY;
				}

				updateBattleWindow();
			}

			break;
		}
		case BATTLE_STATE::ATK_RDY:
		{
			bool _chkForState = true;
			for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
			{	
				if (_pl[plIndex]->getState() != BATTLE_READY)
				{
					_chkForState = false;
					if (_pl[plIndex]->getState() != GETTING_READY)
					{
						_pl[plIndex]->setState(GETTING_READY);
					}
				}
			}

			if (_chkForState) _battleState = BATTLE_STATE::RUNNING;

			updateBattleWindow();
			break;
		}
		case BATTLE_STATE::RUNNING:
		{
			if (!_actionQueue.empty())
			{
				if (_actionQueue.front()()) _actionQueue.pop();
			}
			else
			{
				for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
				{
					if (_pl[plIndex]->getHP() > 0)
					{
						_pl[plIndex]->setTriggerTime(min(_pl[plIndex]->getTriggerTime() + _pl[plIndex]->getSpeed(), maxTriggerTime));
						if (_pl[plIndex]->getTriggerTime() == maxTriggerTime) _shouldPopUpAction[plIndex] = true;
					}
					else _pl[plIndex]->setTriggerTime(0);
				}

				for (int emIndex = 0; emIndex < static_cast<int>(_enemyVector->size()); ++emIndex)
				{
					if (_em[emIndex]->getEnemyHP() > 0)
					{
						_em[emIndex]->setTriggerTime(min(_em[emIndex]->getTriggerTime() + _em[emIndex]->getSpeed(), maxTriggerTime));
						if (_em[emIndex]->getTriggerTime() == maxTriggerTime)
						{
							int targetPlIndex = RNG->getInt(static_cast<int>(_playerVector->size()));

							bool isTargetFound = false;

							if (_pl[targetPlIndex]->getHP() == 0)
							{
								for (int i = 0; i < static_cast<int>(_playerVector->size()); ++i)
								{
									targetPlIndex = i;
									if (_pl[targetPlIndex]->getHP() != 0)
									{
										isTargetFound = true;
										break;
									}
								}
							}
							else isTargetFound = true;

							if (isTargetFound)
							{
								_actionQueue.push([&em = this->_em[emIndex], &pl = this->_pl[targetPlIndex]]()->bool {return em->atkSingleTarget(pl, em); });
								_em[emIndex]->setTriggerTime(0);
							}
						}
					}
					else _em[emIndex]->setTriggerTime(0);
				}
			}

			_shouldShowArrowPointer = false;
			int popUpCount = 0, newArrowPointerX = -1;
			for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
			{
				_shouldShowArrowPointer = _shouldShowArrowPointer || _shouldPopUpAction[plIndex];
				if (_shouldPopUpAction[plIndex])
				{
					popUpCount++; newArrowPointerX = plIndex;
				}
			}

			if (popUpCount == 1) _arrowPointer.x = newArrowPointerX;
			
			// 게이지가 다 찼을 때
			if (_shouldShowArrowPointer)
			{
				if (KEY->down(VK_DOWN))
				{
					if (_arrowPointer.y < 2)
						_arrowPointer.y++;
				}
				else if (KEY->down(VK_UP))
				{
					if (_arrowPointer.y > 0)
						_arrowPointer.y--;
				}
				else if (KEY->down(VK_RIGHT))
				{
					if (_arrowPointer.x < _playerVector->size() - 1)
					{
						_arrowPointer.x++;
					}
					if (!_shouldPopUpAction[_arrowPointer.x])
					{
						if (_arrowPointer.x < _playerVector->size() - 1)
						{
							_arrowPointer.x++;
							if (_arrowPointer.x >= _playerVector->size() - 1 || !_shouldPopUpAction[_arrowPointer.x]) _arrowPointer.x -= 2;
						}
						else _arrowPointer.x--;
					}
				}
				else if (KEY->down(VK_LEFT))
				{
					if (_arrowPointer.x > 0)
						_arrowPointer.x--;
					if (!_shouldPopUpAction[_arrowPointer.x])
					{
						if (_arrowPointer.x > 0)
						{
							_arrowPointer.x--;
							if (_arrowPointer.x <= 0 || !_shouldPopUpAction[_arrowPointer.x]) _arrowPointer.x += 2;
						}
						else _arrowPointer.x++;
					}
				}
				else if (KEY->down('V'))
				{
					bool enemyDown = false;
					switch (_arrowPointer.y)
					{
						case 0: // 일반 공격
							{
								enemyDown = _em[0]->hitDamage(10); // 리마인더: 시험용
								_shouldPopUpAction[_arrowPointer.x] = false;
								_shouldShowArrowPointer = false;
								_pl[_arrowPointer.x]->setTriggerTime(0);

								for (int i = 0; i < static_cast<int>(_playerVector->size()); ++i)
								{
									if (i == _arrowPointer.x) continue;
									if (_shouldPopUpAction[i])
									{
										_arrowPointer.x = i;
										break;
									}
								}
								break;
							}

						case 1: // 스킬
							{

								break;
							}

						case 2: // 아이템
							{

								break;
							}
					}


					if (enemyDown) { _enemyRemainingCnt--; }
				}
			}

			int checkPlayersDown = 0;

			for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
			{
				int plHP = _pl[plIndex]->getHP();
				if (plHP <= 10)
				{

					if (plHP > 0 && _pl[plIndex]->getState() != BATTLE_HELP)
					{
						_pl[plIndex]->setState(BATTLE_HELP);
						_plTurnBar[plIndex]->setGauge(_pl[plIndex]->getTriggerTime(), maxTriggerTime); // 게이지 갱신
					}
					else if (plHP == 0)
					{
						if (_pl[plIndex]->getState() != BATTLE_LOSE)
						{
							_pl[plIndex]->setState(BATTLE_LOSE);
						}
						_shouldPopUpAction[plIndex] = false;
						_pl[plIndex]->setTriggerTime(0);
						_plTurnBar[plIndex]->setGauge(0, maxTriggerTime);
						checkPlayersDown++;
					}
				}
				else _plTurnBar[plIndex]->setGauge(_pl[plIndex]->getTriggerTime(), maxTriggerTime); // 게이지 갱신
			}

			for (int emIndex = 0; emIndex < static_cast<int>(_enemyVector->size()); ++emIndex)
			{
				if (_em[emIndex]->getEnemyHP() == 0) _em[emIndex]->setTriggerTime(0);
			}

			if (_enemyRemainingCnt == 0)
				_battleState = BATTLE_STATE::ALL_ENEMIES_DOWN;

			if (checkPlayersDown == _playerVector->size())
			{
				_shouldShowArrowPointer = false;
				_battleState = BATTLE_STATE::ALL_PLAYERS_DOWN;
			}

			updateBattleWindow();
			break;
		}
		case BATTLE_STATE::ALL_ENEMIES_DOWN:
		{
			for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
			{
				_pl[plIndex]->setState(BATTLE_WIN);
				// _pl[plIndex]->setT(0);
			}

			updateBattleWindow();

			_battleState = BATTLE_STATE::CHECK_REWARD_RDY;

			_isPlLost = false;
			break;
		}
		case BATTLE_STATE::ALL_PLAYERS_DOWN:
		{
			if (KEY->down('V')) _battleState = BATTLE_STATE::UI_CLOSE; // 리마인더: 시험용
			_isPlLost = true;
			break;
		}
		case BATTLE_STATE::CHECK_REWARD_RDY:
		{
			if (RNG->getInt(2) == 1)
			{
				_msgQueue.push("Got 1x Tonic!");
			}

			int expUp = 0;
			int tpUP = 0;

			for (int emIndex = 0; emIndex < static_cast<int>(_enemyVector->size()); ++emIndex)
			{
				expUp += _em[emIndex]->getEnemyExp();
				tpUP += _em[emIndex]->getEnemyTP();
			}

			_msgQueue.push("Got " + to_string(expUp) + " experience point(s)!");
			_msgQueue.push("Got " + to_string(tpUP) +  " tech point(s)!");

			// 골드, TP 처리 추가 필요

			for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
			{
				if (_pl[plIndex]->getState() != BATTLE_LOSE)
				{
					_pl[plIndex]->plusExp(expUp);



				}
			}
			updateBattleWindow();
			_battleState = BATTLE_STATE::CHECK_REWARD_START;
			break;
		}
		case BATTLE_STATE::CHECK_REWARD_START:
		{
			if (TXT->getTextWindowState2() == TEXT_WINDOW_STATE::INVISIBLE)
			{
				if (!_msgQueue.empty())
				{
					++_msgIntervalCount;
					if (_msgIntervalCount > 10)
					{
						_msgIntervalCount = 0;
						TXT->enqueueBM(_msgQueue.front());
						_msgQueue.pop();
					}
				}
				else
				{
					_battleState = BATTLE_STATE::UI_CLOSE;
					_msgIntervalCount = 0;
				}
			}

			updateBattleWindow();
			break;
		}
		case BATTLE_STATE::UI_CLOSE:
		{

				if (_textWindowState == TEXT_WINDOW_STATE::VISIBLE)
				{
					_textWindowState = TEXT_WINDOW_STATE::CLOSING;
				}
				else if (_textWindowState == TEXT_WINDOW_STATE::CLOSING)
				{
					_battleWindowClipCount--;
					if (_battleWindowClipCount == -1)
					{
						_textWindowState = TEXT_WINDOW_STATE::INVISIBLE;
						_battleWindowClipCount = 0;

						_battleState = BATTLE_STATE::RES_PLAYER;
					}
				}
			updateBattleWindow();
			break;
		}
		case BATTLE_STATE::RES_PLAYER:
		{
			for (int plIndex = 0; plIndex < static_cast<int>(_playerVector->size()); ++plIndex)
			{
				if (_pl[plIndex]->getHP() == 0)
				{
					_pl[plIndex]->hitDamage(-1);
				}
				_pl[plIndex]->setState(NORMAL_IDLE);
			}

			_battleState = BATTLE_STATE::END;
			break;
		}
		case BATTLE_STATE::END:
		{
			_isInBattle = false;
			break;	
		}
	}
}

void battle::render()
{
	renderBattleWindow();
}

void battle::updateBattleWindow()
{
	if (_isInBattle)
	{
		if (TXT->getTextWindowPos())
		{
			_yOffset = -606;
		}
		else
		{
			_yOffset = 0;
		}

		_shouldRenderUsingWindowCoords = TRUE;

		IMG->render("전투 적 목록 창 스킨", _hTempDC, 0, 0);
		IMG->render("전투 스탯 창 스킨", _hTempDC, 387, 0);

		for (int emIndex = 0; emIndex < static_cast<int>(_enemyVector->size()); ++emIndex)
		{
			TXT->render(_hTempDC, _emName[emIndex], 64, 44 + emIndex * 48);	// 적 캐릭터 이름
		}

		// HP 글자
		IMG->frameRender("흰색 타일셋0", _hTempDC, 612 - 4, 11, 8, 3);
		IMG->frameRender("흰색 타일셋0", _hTempDC, 640, 11, 10, 3);

		// MP 글자
		IMG->frameRender("흰색 타일셋0", _hTempDC, 738, 11, 9, 3);
		IMG->frameRender("흰색 타일셋0", _hTempDC, 770, 11, 10, 3);

		for (int plIndex = static_cast<int>(_playerVector->size()) - 1; plIndex >= 0; --plIndex)
		{
			TXT->render(_hTempDC, _plName[plIndex], 416, 44 + plIndex * 48); // 플레이어 캐릭터 이름

			// 플레이어 캐릭터의 전투 액션 창, HP, MP, 턴 게이지
			int currentHP = _pl[plIndex]->getHP();
			string str1 = to_string(currentHP);
			string tileset;

			if (currentHP == 0)
			{
				_pl[plIndex]->setState(BATTLE_HELP);
				tileset = "비활성 타일셋0";
				_blinkCount[plIndex] = 24;
			}
			else if (currentHP <= 10)
			{
				// HP 10 이하일 때 12 프레임 단위로 타일셋 교체
				if (_blinkCount[plIndex] < 12)
				{
					tileset = "비활성 타일셋0";
				}
				else tileset = "흰색 타일셋0";
				_blinkCount[plIndex]--;
				if (_blinkCount[plIndex] == 0)
					_blinkCount[plIndex] = 24;
			}
			else tileset = "흰색 타일셋0";

			for (int i = 0; i < str1.size(); i++)
			{
				if (currentHP < 10)
				{
					int frameX = str1[i] - '0';
					IMG->frameRender(tileset, _hTempDC, 676 + i * 32, 44 + plIndex * 48, frameX, 4);
				}
				else if (currentHP < 100)
				{
					int frameX = str1[i] - '0';
					IMG->frameRender(tileset, _hTempDC, 644 + i * 32, 44 + plIndex * 48, frameX, 4);
				}

				else if (currentHP > 100)
				{
					int frameX = str1[i] - '0';
					IMG->frameRender(tileset, _hTempDC, 612 + i * 32, 44 + plIndex * 48, frameX, 4);
				}
			}

			IMG->frameRender(tileset, _hTempDC, 710, 44 + plIndex * 48, 11, 3); // :

			int currentMP = _pl[plIndex]->getMP();
			string str2 = to_string(currentMP);

			for (int i = 0; i < str2.size(); i++)
			{
				if (currentMP < 10)
				{
					int frameX = str2[i] - '0';
					IMG->frameRender(tileset, _hTempDC, 772 + i * 32, 44 + plIndex * 48, frameX, 4);
				}

				else if (currentMP < 100)
				{
					int frameX = str2[i] - '0';
					IMG->frameRender(tileset, _hTempDC, 740 + i * 32, 44 + plIndex * 48, frameX, 4);
				}
			}

			if (_shouldPopUpAction[plIndex])
			{
				IMG->render("전투 창 스킨", _hTempDC, plIndex * 192, 0);
				TXT->render(_hTempDC, "Att.", 64 + plIndex * 192, 44);
				TXT->render(_hTempDC, "Tech", 64 + plIndex * 192, 92);
				TXT->render(_hTempDC, "Item", 64 + plIndex * 192, 140);
			}

			_plTurnBar[plIndex]->render(_hTempDC);
		}

		if (_shouldShowArrowPointer)
			IMG->frameRender("위치 표시 타일셋", _hTempDC, _arrowPointer.x * 192, 19 + _arrowPointer.y * 48, 0, 1);

		_shouldRenderUsingWindowCoords = FALSE;

	}
}

void battle::renderBattleWindow()
{
	if(_textWindowState == TEXT_WINDOW_STATE::OPENING || _textWindowState == TEXT_WINDOW_STATE::CLOSING)
		IMG->renderZ(5000, IMG->find("전투 윈도우 용 빈 비트맵"), getMemDC(), 0 + _currOrg.x , 641 + _yOffset + _currOrg.y + 12 * (8 - _battleWindowClipCount), 0, 12 * (8 - _battleWindowClipCount), WINW, 24 * _battleWindowClipCount);
	else if (_textWindowState == TEXT_WINDOW_STATE::VISIBLE)
		IMG->renderZ(5000, IMG->find("전투 윈도우 용 빈 비트맵"), getMemDC(), 0 + _currOrg.x, 641 + _yOffset + _currOrg.y, 0, 0, WINW, 192);
}

void battle::init(vector<enemy*>* _enemyVector, vector<character*>* _playerVector,
			  vector<POINT>* _PlBattleStartPos, vector<POINT>* _EmBattleStartPos)
{
	_arrowPointer.x = -1;
	_arrowPointer.y = 0;

	_yOffset = 0;
	_isPlLost = true;

	_hTempDC = IMG->find("전투 윈도우 용 빈 비트맵")->getMemDC();

	this->_playerVector = _playerVector;
	this->_enemyVector = _enemyVector;
	this->_PlBattleStartPos = _PlBattleStartPos;
	this->_EmBattleStartPos = _EmBattleStartPos;

	_battleState = BATTLE_STATE::NOT_START_YET;

	_textWindowState = TEXT_WINDOW_STATE::INVISIBLE;

	_msgIntervalCount = 0;
	_battleWindowOpeningCnt = _battleWindowClipCount = 0;
	_shouldShowArrowPointer = false;

	for (int i = 0; i < _playerVector->size(); ++i)
	{
		_pl[i] = (*_playerVector)[i];
		_shouldPopUpAction[i] = false;
		_blinkCount[i] = 24;
		_plName[i] = _playChrNames[_pl[i]->getID()];
	}

	_enemyRemainingCnt = 0;
	for (int i = 0; i < _enemyVector->size(); ++i)
	{
		_enemyRemainingCnt++;
		_em[i] = (*_enemyVector)[i];
		_emName[i] = findEnemyName(_em[i]->getID());
	}
}

string battle::findEnemyName(int id)
{
	switch (id)
	{
		case 0:
			return "Gato";
		default:
			return " ";
	}
}
