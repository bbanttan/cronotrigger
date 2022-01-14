#include "stdafx.h"
#include "gameScene.h"
#include "player.h"
#include "item.h"

void gameScene::drawTopSecondBox()
{
	BitBlt(getMemDC(), 0, 0, WINW - _topBoxAdjX, 96, getMemDC(), _topBoxAdjX, 0, SRCCOPY);
	PatBlt(_hBoxDC, 0, 96 + 640, WINW, WINH - 640 - 96, BLACKNESS);

	string tempStr = _p->getPlayerChName() + "  $ " + to_string(_p->currentMoney());
	tempStr.insert(tempStr.end() - 2, 1, '.');

	DrawText(_hBoxDC, tempStr.c_str(), -1, &_topBoxTextArea, DT_LEFT | DT_TOP | DT_SINGLELINE);

	BitBlt(getMemDC(), WINW - _topBoxAdjX, 0, _topBoxAdjX, 96, _hBoxDC, 0, 0, SRCCOPY);
}

void gameScene::drawBottomSecondBox()
{
	BitBlt(getMemDC(), 0, 96 + 640, WINW - _bottomBoxAdjX, WINH - 640 - 96, getMemDC(), _bottomBoxAdjX, 96 + 640, SRCCOPY);
	PatBlt(_hBoxDC, 0, 96 + 640, WINW, WINH - 640 - 96, BLACKNESS);

	DrawText(_hBoxDC, "BELONGINGS", -1, &_bottomBoxTextArea[0], DT_LEFT | DT_TOP | DT_SINGLELINE);
	DrawText(_hBoxDC, "VOLUME", -1, &_bottomBoxTextArea[1], DT_LEFT | DT_TOP | DT_SINGLELINE);
	DrawText(_hBoxDC, "LEVELS", -1, &_bottomBoxTextArea[2], DT_LEFT | DT_TOP | DT_SINGLELINE);
	DrawText(_hBoxDC, "STATUS", -1, &_bottomBoxTextArea[3], DT_LEFT | DT_TOP | DT_SINGLELINE);
	DrawText(_hBoxDC, "HELP", -1, &_bottomBoxTextArea[4], DT_LEFT | DT_TOP | DT_SINGLELINE);

	if (_shouldShowBottomBoxArrow)
	{
		IMG->frameRender("타일셋", _hBoxDC, _bottomBoxArrowPos[_bottomBoxArrowPosIdx].x + _currOrg.x, _bottomBoxArrowPos[_bottomBoxArrowPosIdx].y + _currOrg.y, 11, 3);
	}

	BitBlt(getMemDC(), WINW - _bottomBoxAdjX, 96 + 640, _bottomBoxAdjX, WINH - 640 - 96, _hBoxDC, 0, 96 + 640, SRCCOPY);
}

void gameScene::showBelongingsScr()
{
	// 하단 영역 화살표 표시
	if (_shouldShowBottomBoxArrow && !_bottomBoxArrowPos.empty())
	{
		PatBlt(_hBoxDC, 0, 96 + 640, WINW, WINH - 640 - 96, BLACKNESS);
		IMG->frameRender("타일셋", _hBoxDC, _bottomBoxArrowPos[_bottomBoxArrowPosIdx].x + _currOrg.x, _bottomBoxArrowPos[_bottomBoxArrowPosIdx].y + _currOrg.y, 11, 3);
		
		BitBlt(getMemDC(), 0, 96 + 640, WINW, WINH - 640 - 96, _hBoxDC, 0, 96 + 640, SRCCOPY);
	}
	if (_bottomBoxCountForBelongings > 1020 && _bottomBoxCountForBelongings < 1041)
	{
		PatBlt(_hBoxDC, 0, 96 + 640, WINW, WINH - 640 - 96, BLACKNESS);
		IMG->frameRender("타일셋", _hBoxDC, 896 + 32 + _currOrg.x, 844 - 14 + _currOrg.y, 15, 7);
		BitBlt(getMemDC(), 0, 96 + 640, WINW, WINH - 640 - 96, _hBoxDC, 0, 96 + 640, SRCCOPY);
	}

	// 주 화면 영역 화살표 표시
	if (_shouldShowMainBoxArrow && !_mainBoxArrowPos.empty())
	{
		PatBlt(_hBoxDC, 0, 96, WINW, 640, BLACKNESS);
		IMG->frameRender("타일셋", _hBoxDC, _mainBoxArrowPos[_mainBoxArrowPosIdx].x + _currOrg.x, _mainBoxArrowPos[_mainBoxArrowPosIdx].y + _currOrg.y, 10, 2);
		BitBlt(getMemDC(), 0, 96, WINW, 640, _hBoxDC, 0, 96, SRCPAINT);
	}

	//if (!_itemNameTextArea.empty())
	//{
	//	for (int i = 0; i < _itemList.size(); ++i)
	//	{
	//		Rectangle(getMemDC(), _itemNameTextArea[i].left, _itemNameTextArea[i].top, _itemNameTextArea[i].right, _itemNameTextArea[i].bottom);
	//	}
	//}
	L->render();
}

void gameScene::showVolumeScr()
{
	if (_bottomBoxCountForVolume > 0)
	{
		L->selectRender("Volume");
		PatBlt(_hBoxDC, 0, 0, WINW, WINH, BLACKNESS);
		DrawText(_hBoxDC, "Master", -1, &_itemNameTextArea[0], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "Background", -1, &_itemNameTextArea[1], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "Effects", -1, &_itemNameTextArea[2], DT_LEFT | DT_TOP);
		for (int i = 0; i < 3; ++i)
			FillRect(_hBoxDC, &_volumeRct[i], _hBrush1);
		for (int i = 3; i < 6; ++i)
			FillRect(_hBoxDC, &_volumeRct[i], _hBrush2);
		BitBlt(getMemDC(), 0, 0, WINW, WINH, _hBoxDC, 0, 0, SRCPAINT);
	}

	if (_shouldShowMainBoxArrow)
	{
		PatBlt(_hBoxDC, 0, 0, WINW, WINH, BLACKNESS);
		IMG->frameRender("타일셋", _hBoxDC, _mainBoxArrowPos[_mainBoxArrowPosIdx].x + _currOrg.x, _mainBoxArrowPos[_mainBoxArrowPosIdx].y + _currOrg.y, 11, 3);
		BitBlt(getMemDC(), 0, 0, WINW, WINH, _hBoxDC, 0, 0, SRCPAINT);
	}
}

void gameScene::showLevelsScr()
{
	if (_bottomBoxCountForLevels > 0)
	{
		L->render();
	};
}

void gameScene::showStatusScr()
{
	if (_bottomBoxCountForStatus > 0)
	{
		L->selectRender("Status");
		L->selectRender("Status2");
		PatBlt(_hBoxDC, 0, 0, WINW, WINH, BLACKNESS);
		DrawText(_hBoxDC, "PUNCH", -1, &_itemNameTextArea[0], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "KICK", -1, &_itemNameTextArea[1], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "WEAPON", -1, &_itemNameTextArea[2], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "THROWING", -1, &_itemNameTextArea[3], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "AGILITY", -1, &_itemNameTextArea[4], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "DEFENSE", -1, &_itemNameTextArea[5], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "STRENGTH", -1, &_itemNameTextArea[6], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "WILL POWER", -1, &_itemNameTextArea[7], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "STAMINA", -1, &_itemNameTextArea[8], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "MAX POWER", -1, &_itemNameTextArea[9], DT_LEFT | DT_TOP);

		DrawText(_hBoxDC, to_string(_p->currentPunch()).c_str(), -1, &_itemNameTextArea[0], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentKick()).c_str(), -1, &_itemNameTextArea[1], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentWeapon()).c_str(), -1, &_itemNameTextArea[2], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentPower()).c_str(), -1, &_itemNameTextArea[3], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentAgility()).c_str(), -1, &_itemNameTextArea[4], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentGuard()).c_str(), -1, &_itemNameTextArea[5], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentEndure()).c_str(), -1, &_itemNameTextArea[6], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentEnergy()).c_str(), -1, &_itemNameTextArea[7], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentHP()).c_str(), -1, &_itemNameTextArea[8], DT_RIGHT | DT_TOP);
		DrawText(_hBoxDC, to_string(_p->currentMaxHP()).c_str(), -1, &_itemNameTextArea[9], DT_RIGHT | DT_TOP);
		BitBlt(getMemDC(), 0, 0, WINW, WINH, _hBoxDC, 0, 0, SRCPAINT);

	}
}

void gameScene::showHelpScr()
{
	if (_bottomBoxCountForHelp > 0 && _bottomBoxCountForHelp < 61)
	{
		PatBlt(_hBoxDC, 0, 0, WINW, WINH, BLACKNESS);
		SetTextColor(_hBoxDC, RGB(112, 240, 64));
		DrawText(_hBoxDC, "BELONGINGS", -1, &_itemNameTextArea[0], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "LEVELS", -1, &_itemNameTextArea[3], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "STATUS", -1, &_itemNameTextArea[6], DT_LEFT | DT_TOP);
		SetTextColor(_hBoxDC, RGB(255, 255, 255));
		DrawText(_hBoxDC, "          : This screen lets", -1, &_itemNameTextArea[0], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "you know what items you have", -1, &_itemNameTextArea[1], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "left and how to use them.", -1, &_itemNameTextArea[2], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "      : In this screen you", -1, &_itemNameTextArea[3], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "can change the skill level &", -1, &_itemNameTextArea[4], DT_LEFT | DT_TOP | DT_NOPREFIX);
		DrawText(_hBoxDC, "message speed of the game.", -1, &_itemNameTextArea[5], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "      : The status screen", -1, &_itemNameTextArea[6], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "displays all of your current", -1, &_itemNameTextArea[7], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "strength levels.", -1, &_itemNameTextArea[8], DT_LEFT | DT_TOP);
		
		if (_bottomBoxCountForHelp > 20 && _bottomBoxCountForHelp < 40) IMG->frameRender("타일셋", _hBoxDC, _itemNameTextArea[9].left + _currOrg.x, _itemNameTextArea[9].top + _currOrg.y, 15, 7);
		BitBlt(getMemDC(), 0, 0, WINW, WINH, _hBoxDC, 0, 0, SRCPAINT);
	}
	else if (_bottomBoxCountForHelp > 120)
	{
		PatBlt(_hBoxDC, 0, 0, WINW, WINH, BLACKNESS);
		SetTextColor(_hBoxDC, RGB(112, 240, 64));
		DrawText(_hBoxDC, "VOLUME", -1, &_itemNameTextArea[0], DT_LEFT | DT_TOP);
		SetTextColor(_hBoxDC, RGB(255, 255, 255));
		DrawText(_hBoxDC, "      : In this screen you", -1, &_itemNameTextArea[0], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "can adjust the volume for", -1, &_itemNameTextArea[1], DT_LEFT | DT_TOP);
		DrawText(_hBoxDC, "background music & sound", -1, &_itemNameTextArea[2], DT_LEFT | DT_TOP | DT_NOPREFIX);
		DrawText(_hBoxDC, "effects.", -1, &_itemNameTextArea[3], DT_LEFT | DT_TOP);
		BitBlt(getMemDC(), 0, 0, WINW, WINH, _hBoxDC, 0, 0, SRCPAINT);
	}
	
}

void gameScene::updateMenuScr(BYTE idx)
{
	switch (idx)
	{
		case 0: // Belongings
			{
				// 창 끄기
				if (KEY->down('J') && _bottomBoxCountForBelongings < 1000 || (KEY->down('K') && _bottomBoxArrowPosIdx == 1))
				{
					_itemNameTextArea.clear();
					_mainBoxArrowPos.clear();
					removeMenuScr(0);
					return;
				}

				// 아이템 사용하기
				if (KEY->down('K') && _shouldShowMainBoxArrow && _bottomBoxArrowPosIdx == 0 && !_itemPtrList.empty())
				{
					if (_bottomBoxCountForBelongings < 1000) _bottomBoxCountForBelongings = 1000;
					else if (_bottomBoxCountForBelongings > 1000 && _bottomBoxCountForBelongings < 1041)
					{
						_bottomBoxCountForBelongings = 1100;
					}

					if (_bottomBoxCountForBelongings == 1100)
					{
						item* tempItem = _p->getVItem()[_mainBoxArrowPosIdx];
						_p->setAllStatusValuesUsingShopItem(
							tempItem->getPlusPunch(),
							tempItem->getPlusKick(),
							tempItem->getPlusWeapon(),
							tempItem->getPlusPower(),
							tempItem->getPlusAgility(),
							tempItem->getPlusGuard(),
							tempItem->getPlusEndure(),
							tempItem->getPlusEnergy(),
							tempItem->getRecoveryHp(),
							tempItem->getPlusMaxHp()
						);
						SAFE_DEL(tempItem);

						_p->getVItem().erase(_p->getVItem().begin() + _mainBoxArrowPosIdx);
						if (_p->getVItem().empty()) _itemNameTextArea.clear();
					}
				}
				else if (KEY->down('J') && _bottomBoxCountForBelongings > 999)
				{
					_bottomBoxCountForBelongings = 1100;
				}

				// 아이템 버리기
				if (KEY->down('K') && _shouldShowMainBoxArrow && _bottomBoxArrowPosIdx == 2 && !_itemPtrList.empty())
				{
					item* tempItem = _p->getVItem()[_mainBoxArrowPosIdx];
					SAFE_DEL(tempItem);
					_p->getVItem().erase(_p->getVItem().begin() + _mainBoxArrowPosIdx);
					if (_p->getVItem().empty()) _itemNameTextArea.clear();
					_bottomBoxCountForBelongings = 1100;
				}

				// 창 내용 표시하기 준비
				_itemPtrList = _p->getVItem();
				_itemList.clear();
				if (_itemPtrList.empty())
				{
					_itemList.push_back("Nothing");
				}
				else
				{
					for (int i = 0; i < _itemPtrList.size(); ++i)
					{
						_itemList.push_back(_itemPtrList[i]->getName());
					}
				}

				if (_itemNameTextArea.size() != _itemList.size())
				{
					_shouldShowBottomBoxArrow =	_shouldShowMainBoxArrow = false;
					_itemNameTextArea.clear(); _itemNameTextArea.reserve(_itemList.size());
					_mainBoxArrowPos.clear(); _mainBoxArrowPos.reserve(_itemList.size());
					L->AllDeleteLine();

					string tempStr = "";
					for (int i = 0; i < _itemList.size(); ++i)
					{
						_itemNameTextArea.push_back({ 128, 200 + 60 * i, 462, 200 + 60 * i + 36 });
						tempStr += _itemList[i] + "%";

						_mainBoxArrowPos.push_back({66, 200 + 60 * i});
					}
					L->CreateLine({ _itemNameTextArea[0].left, _itemNameTextArea[0].top }, tempStr, "itemList", false);
					
					// 플레이어 캐릭터 이름 및 체력 표시 준비
					_hPString = "";
					size_t hPBarsToDraw = static_cast<size_t>(_p->currentHP() + 7) / 8llu;
					for (size_t i = 0; i < (hPBarsToDraw >> 1); ++i)
					{
						_hPString += '{';
					}
					if (hPBarsToDraw % 2 == 1) _hPString += '}';
					L->calmLine({ 64, 36 }, _p->getPlayerChName() + " " + _hPString);
				}
				else
				{
					L->update();
					int itemListSizeTimesTen = static_cast<int>(_itemList.size() * 8);
					if (_bottomBoxCountForBelongings < itemListSizeTimesTen) ++_bottomBoxCountForBelongings;
					else if (_bottomBoxCountForBelongings == itemListSizeTimesTen)
					{
						L->CreateLine({ 64, 768 }, "Select belonging & its use.%   USE    EXIT    THROW AWAY", "bottomBoxText", false);
						++_bottomBoxCountForBelongings;
					}
					else if (_bottomBoxCountForBelongings < itemListSizeTimesTen + 40)
						++_bottomBoxCountForBelongings;
					else if (_bottomBoxCountForBelongings == itemListSizeTimesTen + 40)
					{
						_bottomBoxArrowPos.clear();
						_bottomBoxArrowPos.reserve(3);
						_bottomBoxArrowPos = { { 128, 828 }, { 352, 828 }, { 608, 828 } };
						SND->play("36.wav", _currMasterVolume * _currSFXVolume);

						_bottomBoxArrowPosIdx = 0;
						_shouldShowBottomBoxArrow = true;
						++_bottomBoxCountForBelongings;

						_mainBoxArrowPosIdx = 0;
						_shouldShowMainBoxArrow = true;
					}
					else if (_bottomBoxCountForBelongings == 1000)
					{
						_shouldShowBottomBoxArrow = false;
						L->selectDeleteLine("bottomBoxText");
						L->CreateLine({ 64, 768 }, _p->getPlayerChName() + " ate the " + _p->getVItem()[_mainBoxArrowPosIdx]->getName() + ".", "bottomBoxText", false);
						++_bottomBoxCountForBelongings;
					}
					else if (_bottomBoxCountForBelongings > 1000 && _bottomBoxCountForBelongings < 1021)
					{
						++_bottomBoxCountForBelongings;
					}
					else if (_bottomBoxCountForBelongings > 1020 && _bottomBoxCountForBelongings < 1041)
					{
						++_bottomBoxCountForBelongings;
						if (_bottomBoxCountForBelongings == 1041) _bottomBoxCountForBelongings = 1001;
					}
					else if (_bottomBoxCountForBelongings == 1100)
					{
						L->selectDeleteLine("bottomBoxText");
						_bottomBoxCountForBelongings = itemListSizeTimesTen;
					}
				}
			}
			break;
		case 1: // Volume
			{
				// 창 끄기
				if (KEY->down('J'))
				{
					removeMenuScr(1);
					_itemNameTextArea.clear();
					_mainBoxArrowPos.clear();
					_bottomBoxCountForVolume = 0;
					return;
				}

				// 창 내용 표시하기 준비
				if (_bottomBoxCountForVolume == 0)
				{
					_mainBoxArrowPos.clear();
					_mainBoxArrowPos.reserve(3);
					_itemNameTextArea.clear();
					_itemNameTextArea.reserve(3);
					for (int i = 0; i < 3; ++i)
					{
						_itemNameTextArea.push_back({ 128, 200 + 64 * i - 14, 472, 232 + 64 * i + 8 });
						_mainBoxArrowPos.push_back({ 66, 200 + 64 * i });
					}
					++_bottomBoxCountForVolume;
					L->CreateLine({ 64, 768 }, "Adjust volume.", "Volume", false);
					for (int i = 0; i < 3; ++i) _volumeRct[i] = { 544, 208 + 64 * i, 952, 228 + 64 * i };
				}
				else
				{
					_volumeRct[3] = { 544, 208, static_cast<int>(roundf(static_cast<float>(952 - 544) * _currMasterVolume) + 544), 228 };
					_volumeRct[4] = { 544, 208 + 64, static_cast<int>(roundf(static_cast<float>(952 - 544) * _currBGMVolume) + 544), 228 + 64 };
					_volumeRct[5] = { 544, 208 + 64 * 2, static_cast<int>(roundf(static_cast<float>(952 - 544) * _currSFXVolume) + 544), 228 + 64 * 2 };
					
					switch (_mainBoxArrowPosIdx)
					{
						case 0:
							if (KEY->down('D'))
							{
								_currMasterVolume = min(1.f, _currMasterVolume + 0.05f);
								SND->updateMasterVolume();
							}
							else if (KEY->down('A'))
							{
								_currMasterVolume = max(0.f, _currMasterVolume - 0.05f);
								SND->updateMasterVolume();
							}
							break;
						case 1:
							if (KEY->down('D'))
							{
								_currBGMVolume = min(1.f, _currBGMVolume + 0.05f);
								SND->updateBGMVolume();
							}
							else if (KEY->down('A'))
							{
								_currBGMVolume = max(0.f, _currBGMVolume - 0.05f);
								SND->updateBGMVolume();
							}
							break;
						case 2:
							if (KEY->down('D'))
							{
								_currSFXVolume = min(1.f, _currSFXVolume + 0.05f);
								SND->updateSFXVolume();
								SND->play("25.wav", _currMasterVolume* _currSFXVolume);
							}
							else if (KEY->down('A'))
							{
								_currSFXVolume = max(0.f, _currSFXVolume - 0.05f);
								SND->updateSFXVolume();
								SND->play("25.wav", _currMasterVolume* _currSFXVolume);
							}
							break;
					}

					L->selectUpdate("Volume");
					_shouldShowMainBoxArrow = true;
				}
			}
			break;
		case 2: // Levels
			{
				// 창 끄기
				if (KEY->down('J'))
				{
					removeMenuScr(2);
					_itemNameTextArea.clear();
					_bottomBoxCountForLevels = 0;
					_yPosForLevels = _prevYPosForLevels;
					_xPosForLevels = _prevXPosForLevels;
					return;
				}
				else if (KEY->down(VK_RETURN))
				{
					removeMenuScr(2);
					_itemNameTextArea.clear();
					_bottomBoxCountForLevels = 0;

					if (_yPosForLevels == 0) _lineSpd = 5;
					else if (_yPosForLevels == 1) _lineSpd = 3;
					else if (_yPosForLevels == 2) _lineSpd = 1;
					_prevYPosForLevels = _yPosForLevels;
					_prevXPosForLevels = _xPosForLevels;
					return;
				}

				// 창 내용 표시하기 준비
				if (_bottomBoxCountForLevels == 0)
				{
					_prevYPosForLevels = _yPosForLevels;
					_prevXPosForLevels = _xPosForLevels;
					L->calmLine(MakePt(287, 160), "MESSAGE SPEED");
					L->calmLine(MakePt(448, 255), "SLOW");
					L->calmLine(MakePt(448, 319), "NORMAL");
					L->calmLine(MakePt(448, 383), "FAST");
					L->calmLine(MakePt(351, 544), "SKILL LEVEL");
					L->calmLine(MakePt(159, 640), "NOVICE");
					L->calmLine(MakePt(608, 640), "ADVANCED");
					++_bottomBoxCountForLevels;
				}
				else if (_bottomBoxCountForLevels > 0 && _bottomBoxCountForLevels < 20)
				{
					++_bottomBoxCountForLevels;
					L->update();
				}
				else if (_bottomBoxCountForLevels == 20)
				{
					L->CreateLine(MakePt(65, 768), "Select message speed & level.", "n", false);
					L->update();
					L->CreateLine(MakePt(384 + 32, 255 + 64), "▶", "화살표1", false);
					L->CreateLine(MakePt(94 + 32, 640), "▶", "화살표2", false);
					++_bottomBoxCountForLevels;
				}
				else if (_bottomBoxCountForLevels > 20)
				{
					if (KEY->down('W') && _yPosForLevels > 0)
					{
						--_yPosForLevels;
					}
					else if (KEY->down('S') && _yPosForLevels < 2)
					{
						++_yPosForLevels;
					}
					if (KEY->down('A') && _xPosForLevels > 0)
					{
						--_xPosForLevels;
					}
					else if (KEY->down('D') && _xPosForLevels < 1)
					{
						++_xPosForLevels;
					}

					if (_yPosForLevels == 0) L->setPosLine("화살표1", MakePt(384 + 32, 255));
					else if (_yPosForLevels == 1) L->setPosLine("화살표1", MakePt(384 + 32, 255 + 64));
					else if (_yPosForLevels == 2) L->setPosLine("화살표1", MakePt(384 + 32, 255 + 128));

					if (_xPosForLevels == 0) L->setPosLine("화살표2", MakePt(94 + 32, 640));
					else if (_xPosForLevels == 1) L->setPosLine("화살표2", MakePt(94 + 449 + 32, 640));

					L->update();
				}

				//if (_bottomBoxCountForVolume == 0)
				//{
				//	_itemNameTextArea.clear();
				//	_itemNameTextArea.reserve(10);
				//	for (int i = 0; i < 9; ++i)
				//	{
				//		_itemNameTextArea.push_back({ 64, 128 + 64 * i - 14, 472, 160 + 64 * i + 8 });
				//	}
				//	_itemNameTextArea.push_back({ 64, 672 - 14, 472, 700 + 8 });

				//	++_bottomBoxCountForLevels;
				//	L->CreateLine({ 64, 36 }, _p->getPlayerChName(), "Status", false);
				//	L->CreateLine({ 64, 768 }, "Present Status.", "Status2", false);
				//}
				//else
				//{
				//	L->selectUpdate("Status");
				//	L->selectUpdate("Status2");
				//}

			}
			break;
		case 3: // Status
			{
				// 창 끄기
				if (KEY->down('J') || KEY->down(VK_RETURN))
				{
					removeMenuScr(3);
					_itemNameTextArea.clear();
					_bottomBoxCountForStatus = 0;
					return;
				}

				// 창 내용 표시하기 준비
				if (_bottomBoxCountForStatus == 0)
				{
					_itemNameTextArea.clear();
					_itemNameTextArea.reserve(10);
					for (int i = 0; i < 9; ++i)
					{
						_itemNameTextArea.push_back({ 64, 128 + 64 * i - 14, 472, 160 + 64 * i + 8 });
					}
					_itemNameTextArea.push_back({ 64, 672 - 14, 472, 700 + 8 });

					++_bottomBoxCountForStatus;
					L->CreateLine({ 64, 36 }, _p->getPlayerChName(), "Status", false);
					L->CreateLine({ 64, 768 }, "Present Status.", "Status2", false);
				}
				else
				{
					L->selectUpdate("Status");
					L->selectUpdate("Status2");
				}

			}
			break;
		case 4: // Help
			{
				// 창 끄기
				if (KEY->down('J') || KEY->down(VK_RETURN) || _bottomBoxCountForHelp > 120 && (KEY->down('K')))
				{
					removeMenuScr(4);
					_itemNameTextArea.clear();
					_bottomBoxCountForHelp = 0;
					return;
				}

				// 창 내용 표시하기 준비
				if (_bottomBoxCountForHelp == 0)
				{
					_itemNameTextArea.clear();
					_itemNameTextArea.reserve(10);
					_itemNameTextArea.push_back({ 64, 128 - 14, 956, 284 + 8 });
					_itemNameTextArea.push_back({ 64, 128 + 64 - 14, 956, 284 + 8 });
					_itemNameTextArea.push_back({ 64, 128 + 128 - 14, 956, 284 + 8 });
					_itemNameTextArea.push_back({ 64, 384 - 14, 956, 544 + 8 });
					_itemNameTextArea.push_back({ 64, 384 + 64 - 14, 960, 544 + 8 });
					_itemNameTextArea.push_back({ 64, 384 + 128 - 14, 956, 544 + 8 });
					_itemNameTextArea.push_back({ 64, 640 - 14, 956, 800 + 8 });
					_itemNameTextArea.push_back({ 64, 640 + 64 - 14, 956, 800 + 8 });
					_itemNameTextArea.push_back({ 64, 640 + 128 - 14, 956, 800 + 8 });
					_itemNameTextArea.push_back({ 896 + 32, 844 - 14, 924 + 32, 860 + 8 });

					_shouldShowMainBoxArrow = true;
					++_bottomBoxCountForHelp;
				}
				else if (_bottomBoxCountForHelp > 0 && _bottomBoxCountForHelp < 41)
				{
					++_bottomBoxCountForHelp;
					if (_bottomBoxCountForHelp == 41) _shouldShowMainBoxArrow = false;
					if (KEY->down('K') && _bottomBoxCountForHelp > 20) _bottomBoxCountForHelp = 100;
				}
				else if (_bottomBoxCountForHelp > 40 && _bottomBoxCountForHelp < 61)
				{
					++_bottomBoxCountForHelp;
					if (_bottomBoxCountForHelp == 61) { _bottomBoxCountForHelp = 20; _shouldShowMainBoxArrow = true; }
					if (KEY->down('K')) _bottomBoxCountForHelp = 100;
				}
				else if (_bottomBoxCountForHelp == 100)
				{
					_shouldShowMainBoxArrow = false;
					_shouldFadeOut = TRUE;
					++_bottomBoxCountForHelp;
				}
				else if (_bottomBoxCountForHelp > 100 && _bottomBoxCountForHelp < 120)
				{
					++_bottomBoxCountForHelp;
				}
				else if (_bottomBoxCountForHelp == 120)
				{
					_itemNameTextArea.clear();
					_itemNameTextArea.reserve(4);
					_itemNameTextArea.push_back({ 64, 128 - 14, 956, 800 + 8 });
					_itemNameTextArea.push_back({ 64, 128 + 64 - 14, 956, 800 + 8 });
					_itemNameTextArea.push_back({ 64, 128 + 128 - 14, 956, 800 + 8 });
					_itemNameTextArea.push_back({ 64, 128 + 192 - 14, 956, 800 + 8 });
					_shouldFadeOut = FALSE;

					++_bottomBoxCountForHelp;
				}
				else if (_bottomBoxCountForHelp > 120 && _bottomBoxCountForHelp < 125)
				{
					++_bottomBoxCountForHelp;
				}
				else if (_bottomBoxCountForHelp == 125)
				{
					SND->stop("6 - Password.mp3");
					SND->play("5 - Menu.mp3", _currMasterVolume* _currBGMVolume);
					++_bottomBoxCountForHelp;
				}
			}
			break;
	}
}

void gameScene::removeMenuScr(BYTE prevBottomBoxArrowPosIdx)
{
	L->AllDeleteLine();
	_bottomBoxArrowPosIdx = prevBottomBoxArrowPosIdx;
	_shouldShowMenuScreen = _shouldShowBottomBoxArrow = _shouldShowMainBoxArrow = false;

	_bottomBoxTextArea.clear();
	_bottomBoxTextArea.reserve(5);
	_bottomBoxTextArea = { {100, 768 - 12, 418, 796 + 8}, {724, 768 - 12, 912, 796 + 8}, {100, 832 - 12, 292, 860 + 8}, {426, 832 - 12, 620, 860 + 8}, {788, 832 - 12, 912, 860 + 8} };
	_bottomBoxArrowPos.clear();
	for (int i = 0; i < _bottomBoxTextArea.size(); ++i)
		_bottomBoxArrowPos.push_back({ _bottomBoxTextArea[i].left - 32, _bottomBoxTextArea[i].top + 14 });
	_shouldFadeOut = TRUE;
}