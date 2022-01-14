#include "stdafx.h"
#include "textManager.h"

void textManager::prepareToUseFonts()
{
	_fP->parseFnt(0, "res/fonts/dialogGlyphs.fnt");
	_fP->parseFnt(1, "res/fonts/monoGlyphs.fnt");
}

void textManager::setCurrFontImage(int searchValue)
{
	switch (searchValue)
	{
		case 0:
			_currFontImage = IMG->find("흰색 대화 글꼴");
			break;
		case 1:
			_currFontImage = IMG->find("하늘색 대화 글꼴");
			break;
		case 2:
			_currFontImage = IMG->find("빨간색 대화 글꼴");
			break;
		case 3:
			_currFontImage = IMG->find("노란색 대화 글꼴");
			break;
		case 4:
			_currFontImage = IMG->find("녹색 대화 글꼴");
			break;
		case 5:
			_currFontImage = IMG->find("비활성 대화 글꼴");
			break;
		case 100:
			_currFontImage = IMG->find("흰색 UI 글꼴");
			break;
		case 101:
			_currFontImage = IMG->find("하늘색 UI 글꼴");
			break;
		case 102:
			_currFontImage = IMG->find("빨간색 UI 글꼴");
			break;
		case 103:
			_currFontImage = IMG->find("노란색 UI 글꼴");
			break;
		case 104:
			_currFontImage = IMG->find("녹색 UI 글꼴");
			break;
		case 105:
			_currFontImage = IMG->find("비활성 UI 글꼴");
			break;
		default:
			return;
	}
}

textManager::textManager():
	_shouldWindowUseTheTopPane(FALSE), _shouldBeAutoClosed(FALSE), _isGoingToBeAutoClosed(FALSE),
	_shouldWindowHaveChoices(FALSE),
	_isWholeCurrStrShown1(FALSE), _isWholeCurrStrShown2(FALSE),
	_textWindowState1(TEXT_WINDOW_STATE::INVISIBLE), _textWindowState2(TEXT_WINDOW_STATE::INVISIBLE),
	_windowClipCount(0), _waitCount(0), _fP(new fontParser), _choiceSelected(0)
{
	HDC hDC = GetDC(_hWnd);

	_hTextWindowDC = CreateCompatibleDC(hDC);
	_hTextWindowBitmap = CreateCompatibleBitmap(hDC, WINW, 320);
	_hOTextWindowBitmap = (HBITMAP)SelectObject(_hTextWindowDC, _hTextWindowBitmap);

	ReleaseDC(_hWnd, hDC);

	_bF.BlendOp = AC_SRC_OVER;
	_bF.BlendFlags = 0;
	_bF.SourceConstantAlpha = _textWindowAlpha;
	_bF.AlphaFormat = 0; // AC_SRC_ALPHA는 32 bpp 소스 비트맵에만 사용할 수 있다.
}

textManager::~textManager()
{
	DeleteObject(SelectObject(_hTextWindowDC, _hOTextWindowBitmap));
	DeleteObject(_hTextWindowBitmap);
	DeleteDC(_hTextWindowDC);
	SAFE_DEL(_fP);
}

HRESULT textManager::init()
{
	return S_OK;
}

void textManager::release()
{
	clearLQ();
	clearBMQ();
	clearCQ();

	iSS.clear();
	iSS.str(string());
}

void textManager::enqueueL(string line, int forWhichChoice)
{
	_dialogLineQ.push(make_pair(forWhichChoice, line));
}

void textManager::enqueueBM(string msg, bool isShownChrByChr)
{
	if (isShownChrByChr) _battleMsgQ1.push(make_pair(msg, _battleMsgSpeed * 30 + 30));
	else _battleMsgQ2.push(make_pair(msg, _battleMsgSpeed * 30 + 30));
}

void textManager::enqueueC(string msg)
{
	_chrByChrQ.push(msg);
}

void textManager::clearLQ()
{
	if (!_dialogLineQ.empty()) _dialogLineQ = {};
}

void textManager::clearBMQ()
{
	if (!_battleMsgQ1.empty()) _battleMsgQ1 = {};
	if (!_battleMsgQ2.empty()) _battleMsgQ2 = {};
}

void textManager::clearCQ()
{
	if (!_chrByChrQ.empty()) _chrByChrQ = {};
}

void textManager::updateL()
{
	if (_textWindowState1 == TEXT_WINDOW_STATE::INVISIBLE)
	{
		if (!_dialogLineQ.empty()) // 출력 (대기) 중인 스트링이 있으면
		{
			_textWindowState1 = TEXT_WINDOW_STATE::OPENING;
			_windowClipCount = 0;
			_waitCount = 0;
			_isWholeCurrStrShown1 = FALSE;
			_shouldWindowHaveChoices = FALSE;
			_shouldBeAutoClosed = FALSE;
			_isGoingToBeAutoClosed = FALSE;
			if (!_currStr1.empty()) _currStr1.clear();

			// istringstream 변수 비우기
			iSS.clear();
			iSS.str(string());
		}
	}
	else if (_textWindowState1 == TEXT_WINDOW_STATE::OPENING)
	{
		++_windowClipCount;
		if (_windowClipCount == 11) _textWindowState1 = TEXT_WINDOW_STATE::VISIBLE;
	}
	else if (_textWindowState1 == TEXT_WINDOW_STATE::VISIBLE)
	{
		if (_dialogLineQ.empty()) // 출력 (대기) 중인 스트링이 없으면
		{
			_textWindowState1 = TEXT_WINDOW_STATE::CLOSING;
			_windowClipCount = 10;
		}
		else if (_waitCount > 0)
		{
			--_waitCount;
		}
		else if (!_isWholeCurrStrShown1)
		{
			if (iSS.rdbuf()->in_avail() == 0 && _currStr1.empty())
			{
				if (_choiceSelected != 0)
				{
					while (!_dialogLineQ.empty() && (_dialogLineQ.front().first != _choiceSelected && _dialogLineQ.front().first != 0))
					{
						_dialogLineQ.pop();
						if (_dialogLineQ.empty()) return;
					}
				}
				iSS.str(_dialogLineQ.front().second);
			}
			
			iSS.get(_character);
			if (iSS.fail())
				_isWholeCurrStrShown1 = TRUE;
			else if (_character == '<')
			{
				iSS.get(_character);
				if (iSS.fail())
					_isWholeCurrStrShown1 = TRUE;
				else
				{
					string tempStr{};
					while (true)
					{
						tempStr += _character;
						iSS.get(_character);
						if (iSS.fail())
						{
							_isWholeCurrStrShown1 = TRUE;
							break;
						}
						else if (_character == '>')
						{
							if (tempStr[0] == 'w') // 기다리기이면(즉 <w정수>가 있으면)
							{
								try
								{
									int count = stoi(tempStr.substr(1));
									if (count > 0) _waitCount = count - 1; // 정수가 양수이면 그만큼 기다리도록 변숫값을 변경한다.
									else if (count < -1) // 정수가 -1보다 작은 음수 n이면(바로 다음 -n 개 문자가 함께 출력되는 조건이면)
									{
										while (count < 0) // count가 0이 될 때까지
										{
											iSS.get(_character);
											if (iSS.fail())
											{
												_isWholeCurrStrShown1 = TRUE;
												break;
											}
											else _currStr1 += _character;
											++count;
										}
										// 주의: 이때에는 <, [ 등 문자가 있는지 검사를 하지 않는다.
									}
								}
								catch (const invalid_argument& e)
								{
									UNREFERENCED_PARAMETER(e);
								}
							}
							else if (tempStr == "acf") // 강제 자동 닫기(시간이 지나야만 닫는다.)이면(즉 <acf>가 있으면)
							{
								_waitCount = 60 - 1; // 60 프레임 기다리도록 변숫값을 변경한다.
								_isGoingToBeAutoClosed = TRUE;
								_shouldBeAutoClosed = TRUE;
							}
							else if (tempStr.substr(0, 3) == "acf") // 강제 자동 닫기(시간이 지나야만 닫는다.)이면(즉 <acf자연수>가 있으면)
							{
								try
								{
									int count = stoi(tempStr.substr(3));
									if (count > 0) _waitCount = count - 1;
									_isGoingToBeAutoClosed = TRUE;
									_shouldBeAutoClosed = TRUE;
								}
								catch (const invalid_argument& e)
								{
									UNREFERENCED_PARAMETER(e);
								}
							}
							else if (tempStr.substr(0,2) == "ac") // 자동 닫기이면(즉 <ac자연수>가 있으면)
							{
								try
								{
									int count = stoi(tempStr.substr(2));
									if (count > 0) _waitCount = count - 1;
									_isGoingToBeAutoClosed = TRUE;
								}
								catch (const invalid_argument& e)
								{
									UNREFERENCED_PARAMETER(e);
								}
							}
							else if (tempStr == "ac") // 자동 닫기이면(즉 <ac>가 있으면)
							{
								_waitCount = 60 - 1;
								_isGoingToBeAutoClosed = TRUE;
							}
							else if (tempStr == "c") // 선택지 손가락 아이콘 보이기이면(즉 <c>가 있으면)
							{
								_shouldWindowHaveChoices = TRUE;
								_choiceSelected = 1; // 기본으로 처음 것이 선택된 상태가 되게 한다.
							}
							else if (tempStr == "zc") // 선택 초기화이면
							{
								_choiceSelected = 0;
							}
							break;
						}
					}
				}
			}
			else if (_character == '[')
			{
				_currStr1 += _character;
				iSS.get(_character);
				if (iSS.fail())
					_isWholeCurrStrShown1 = TRUE;
				else
				{
					string tempStr{};
					while (true)
					{
						tempStr += _character;
						iSS.get(_character);
						if (iSS.fail())
						{
							_isWholeCurrStrShown1 = TRUE;
							break;
						}
						else if (_character == ']')
						{
							_currStr1 += tempStr + ']';
							iSS.get(_character);
							if (iSS.fail()) _isWholeCurrStrShown1 = TRUE;
							else if (_character == '<')
							{
								tempStr.clear();
								iSS.get(_character);
								if (iSS.fail())
									_isWholeCurrStrShown1 = TRUE;
								else
								{
									while (true)
									{
										tempStr += _character;
										iSS.get(_character);
										if (iSS.fail())
										{
											_isWholeCurrStrShown1 = TRUE;
											break;
										}
										else if (_character == '>')
										{
											if (tempStr[0] == 'w') // 기다리기이면(즉 <w정수>가 있으면)
											{
												try
												{
													int count = stoi(tempStr.substr(1));
													if (count > 0) _waitCount = count - 1; // 정수가 양수이면 그만큼 기다리도록 변숫값을 변경한다.
													else if (count < -1) // 정수가 -1보다 작은 음수 n이면(바로 다음 -n 개 문자가 함께 출력되는 조건이면)
													{
														while (count < 0) // count가 0이 될 때까지
														{
															iSS.get(_character);
															if (iSS.fail())
															{
																_isWholeCurrStrShown1 = TRUE;
																break;
															}
															else _currStr1 += _character;
															++count;
														}
														// 주의: 이때에는 <, [ 등 문자가 있는지 검사를 하지 않는다.
													}
												}
												catch (const invalid_argument& e)
												{
													UNREFERENCED_PARAMETER(e);
												}
											}
											else if (tempStr == "acf") // 강제 자동 닫기(시간이 지나야만 닫는다.)이면(즉 <acf>가 있으면)
											{
												_waitCount = 60 - 1; // 60 프레임 기다리도록 변숫값을 변경한다.
												_isGoingToBeAutoClosed = TRUE;
												_shouldBeAutoClosed = TRUE;
											}
											else if (tempStr.substr(0, 3) == "acf") // 강제 자동 닫기(시간이 지나야만 닫는다.)이면(즉 <acf자연수>가 있으면)
											{
												try
												{
													int count = stoi(tempStr.substr(3));
													if (count > 0) _waitCount = count - 1;
													_isGoingToBeAutoClosed = TRUE;
													_shouldBeAutoClosed = TRUE;
												}
												catch (const invalid_argument& e)
												{
													UNREFERENCED_PARAMETER(e);
												}
											}
											else if (tempStr.substr(0, 2) == "ac") // 자동 닫기이면(즉 <ac자연수>가 있으면)
											{
												try
												{
													int count = stoi(tempStr.substr(2));
													if (count > 0) _waitCount = count - 1;
													_isGoingToBeAutoClosed = TRUE;
												}
												catch (const invalid_argument& e)
												{
													UNREFERENCED_PARAMETER(e);
												}
											}
											else if (tempStr == "ac") // 자동 닫기이면(즉 <ac>가 있으면)
											{
												_waitCount = 60 - 1;
												_isGoingToBeAutoClosed = TRUE;
											}
											else if (tempStr == "c") // 선택지 손가락 아이콘 보이기이면(즉 <c>가 있으면)
											{
												_shouldWindowHaveChoices = TRUE;
												_choiceSelected = 1; // 기본으로 처음 것이 선택된 상태가 되게 한다.
											}
											else if (tempStr == "zc") // 선택 초기화이면
											{
												_choiceSelected = 0;
											}
											break;
										}
									}
								}
							}
							else _currStr1 += _character;
							break;
						}
					}
				}
			}
			else if (_character == '\\')
			{
				_currStr1 += _character;
				iSS.get(_character);
				if (iSS.fail())
					_isWholeCurrStrShown1 = TRUE;
				else
					_currStr1 += _character;
			}
			else
				_currStr1 += _character;
		}
		else if (_isWholeCurrStrShown1)
		{
			if (KEY->down('V') && !_shouldBeAutoClosed || _isGoingToBeAutoClosed && _waitCount == 0)
			{
				iSS.clear();
				iSS.str(string());
				_isWholeCurrStrShown1 = FALSE;
				_isGoingToBeAutoClosed = FALSE;
				_shouldBeAutoClosed = FALSE;
				_currStr1.clear();
				_dialogLineQ.pop();
				if (_shouldWindowHaveChoices) _shouldWindowHaveChoices = FALSE;
			}
			else if (_shouldWindowHaveChoices)
			{
				if (KEY->down(VK_DOWN) && _choiceSelected == 1) _choiceSelected = 2;
				else if (KEY->down(VK_UP) && _choiceSelected == 2) _choiceSelected = 1;
			}
		}
	}
	else if (_textWindowState1 == TEXT_WINDOW_STATE::CLOSING)
	{
		--_windowClipCount;
		if (_windowClipCount == -1) _textWindowState1 = TEXT_WINDOW_STATE::INVISIBLE;
	}
}

void textManager::updateBM()
{
	// _battleMsgQ1 처리(한 번에 한 글자씩)
	if (_textWindowState1 == TEXT_WINDOW_STATE::INVISIBLE)
	{
		if (!_battleMsgQ1.empty()) // 출력 (대기) 중인 스트링이 있으면
		{
			_textWindowState1 = TEXT_WINDOW_STATE::VISIBLE; // 참고: 전투 메시지 창에 OPENING 상태는 없다.
			_isWholeCurrStrShown1 = FALSE;
			if (!_currStr1.empty()) _currStr1.clear();

			// istringstream 변수 비우기
			iSS.clear();
			iSS.str(string());
		}
	}
	else if (_textWindowState1 == TEXT_WINDOW_STATE::VISIBLE)
	{
		if (_battleMsgQ1.empty()) // 출력 (대기) 중인 스트링이 없으면
			_textWindowState1 = TEXT_WINDOW_STATE::INVISIBLE; // 참고: 전투 메시지 창에 CLOSING 상태는 없다.
		else // 출력 (대기) 중인 스트링이 있으면
		{
			if (_battleMsgQ1.size() > 1)
			{
				while (_battleMsgQ1.size() > 1) _battleMsgQ1.pop(); // 출력 대기 중인 스트링이 둘 이상이면 최근 출력 예약 스트링만 남긴다.

				_isWholeCurrStrShown1 = FALSE;
				_currStr1.clear();
				iSS.clear();
				iSS.str(string());
			}

			if (_isWholeCurrStrShown1 && _battleMsgQ1.front().second > 0)
			{
				--_battleMsgQ1.front().second; // 남은 표시 시간 감소
			}
			else if (!_isWholeCurrStrShown1)
			{
				if (iSS.rdbuf()->in_avail() == 0 && _currStr1.empty())
				{
					iSS.str(_battleMsgQ1.front().first);
				}

				iSS.get(_character);
				if (iSS.fail()) _isWholeCurrStrShown1 = TRUE;
				else if (_character == '[')
				{
					_currStr1 += _character;
					iSS.get(_character);
					if (iSS.fail()) _isWholeCurrStrShown1 = TRUE;
					else
					{
						string tempStr{};
						while (true)
						{
							tempStr += _character;
							iSS.get(_character);
							if (iSS.fail())
							{
								_isWholeCurrStrShown1 = TRUE;
								break;
							}
							else if (_character == ']')
							{
								_currStr1 += tempStr + ']';
								iSS.get(_character);
								if (iSS.fail()) _isWholeCurrStrShown1 = TRUE;
								else _currStr1 += _character;
								break;
							}
						}
					}
				}
				else if (_character == '\\')
				{
					_currStr1 += _character;
					iSS.get(_character);
					if (iSS.fail())
						_isWholeCurrStrShown1 = TRUE;
					else
						_currStr1 += _character;
				}
				else
					_currStr1 += _character;
			}
			else if (_isWholeCurrStrShown1)
			{
				if (_battleMsgQ1.front().second == 0)
				{
					_isWholeCurrStrShown1 = FALSE;
					_currStr1.clear();
					_battleMsgQ1.pop();
					iSS.clear();
					iSS.str(string());
					_textWindowState1 = TEXT_WINDOW_STATE::INVISIBLE;
				}
			}
		}
	}

	// _battleMsgQ2 처리(한 번에 한 줄 전부) - istringstream 비이용(불필요하므로)
	if (_textWindowState2 == TEXT_WINDOW_STATE::INVISIBLE)
	{
		if (!_battleMsgQ2.empty()) // 출력 (대기) 중인 스트링이 있으면
		{
			_textWindowState2 = TEXT_WINDOW_STATE::VISIBLE; // 전투 메시지 창에 OPENING 상태는 없다.
			_isWholeCurrStrShown2 = FALSE;
			if (!_currStr2.empty()) _currStr2.clear();
		}
	}
	else if (_textWindowState2 == TEXT_WINDOW_STATE::VISIBLE)
	{
		if (_battleMsgQ2.empty()) // 출력 (대기) 중인 스트링이 없으면
			_textWindowState2 = TEXT_WINDOW_STATE::INVISIBLE; // 전투 메시지 창에 CLOSING 상태는 없다.
		else // 출력 (대기) 중인 스트링이 있으면
		{
			if (_battleMsgQ2.size() > 1)
			{
				while (_battleMsgQ2.size() > 1) _battleMsgQ2.pop(); // 출력 대기 중인 스트링이 둘 이상이면 최근 출력 예약 스트링만 남긴다.
				_isWholeCurrStrShown2 = FALSE;
				_currStr2.clear();
			}

			if (_isWholeCurrStrShown2 && _battleMsgQ2.front().second > 0)
			{
				--_battleMsgQ2.front().second; // 남은 표시 시간 감소
			}
			else if (!_isWholeCurrStrShown2)
			{
				if (_currStr2.empty())
				{
					_currStr2 = _battleMsgQ2.front().first;
					_isWholeCurrStrShown2 = TRUE;
				}
			}
			else if (_isWholeCurrStrShown2)
			{
				if (_battleMsgQ2.front().second == 0 || KEY->down('V') && _isWholeCurrStrShown2)
				{
					_isWholeCurrStrShown2 = FALSE;
					_currStr2.clear();
					_battleMsgQ2.pop();
					_textWindowState2 = TEXT_WINDOW_STATE::INVISIBLE;
				}
			}
		}
	}
}

void textManager::updateC()
{
	if (_chrByChrQ.size() > 1)
	{
		while (_chrByChrQ.size() > 1) _chrByChrQ.pop(); // 출력 대기 중인 스트링이 둘 이상이면 최근 출력 예약 스트링만 남긴다.
		_isWholeCurrStrShown1 = FALSE;
		_currStr1.clear();
		iSS.clear();
		iSS.str(string());
	}

	if (_chrByChrQ.empty())
	{
		if (!_currStr1.empty())
		{
			_isWholeCurrStrShown1 = FALSE;
			_currStr1.clear();
			iSS.clear();
			iSS.str(string());
		}
	}
	else if (!_isWholeCurrStrShown1)
	{
		if (iSS.rdbuf()->in_avail() == 0 && _currStr1.empty())
		{
			iSS.str(_chrByChrQ.front());
		}

		iSS.get(_character);
		if (iSS.fail()) _isWholeCurrStrShown1 = TRUE;
		else if (_character == '[')
		{
			_currStr1 += _character;
			iSS.get(_character);
			if (iSS.fail()) _isWholeCurrStrShown1 = TRUE;
			else
			{
				string tempStr{};
				while (true)
				{
					tempStr += _character;
					iSS.get(_character);
					if (iSS.fail())
					{
						_isWholeCurrStrShown1 = TRUE;
						break;
					}
					else if (_character == ']')
					{
						_currStr1 += tempStr + ']';
						iSS.get(_character);
						if (iSS.fail()) _isWholeCurrStrShown1 = TRUE;
						else _currStr1 += _character;
						break;
					}
				}
			}
		}
		else if (_character == '\\')
		{
			_currStr1 += _character;
			iSS.get(_character);
			if (iSS.fail())
				_isWholeCurrStrShown1 = TRUE;
			else
				_currStr1 += _character;
		}
		else
			_currStr1 += _character;
	}
}


void textManager::render(HDC hDC, string text, int destX, int destY, int fontIdx, int colorIdx)
{
	if (fontIdx < 2) fontInfo = _fP->getFontInfo(fontIdx);
	else return;

	setCurrFontImage(fontIdx * 100 + colorIdx);

	_currPosX = destX;
	_currPosY = destY;

	for (size_t i = 0; i < text.size(); ++i)
	{
		char chr = text[i];
		if (chr == '\n') // \n: 개행
		{
			_currPosX = destX;
			_currPosY += (*fontInfo)[32].height + 16;
		}
		else if (chr == '\t') // \t: 세 칸 띄기
		{
			_currPosX += (*fontInfo)[32].width * 3;
		}
		else if (chr == '\\' && i < text.size() - 1 && text[i + 1] == 'n') // \\n: raw string literal 개행
		{
			_currPosX = destX;
			_currPosY += (*fontInfo)[32].height + 16;
			i += 1;
			continue;
		}
		else if (chr == '\\' && i < text.size() - 1 && text[i + 1] == 't') // \\t: raw string literal 세 칸 띄기
		{
			_currPosX += (*fontInfo)[32].width * 3;
			i += 1;
			continue;
		}
		else if (chr == '[')
		{
			if (i < text.size() - 2 && text[i + 2] == ']')
			{
				setCurrFontImage(fontIdx * 100 + text[i + 1] - '0');
				i += 2;
				chr = text[i];
			}
		}
		else if ((*fontInfo)[chr].width == 0) continue;
		_currFontImage->render(hDC, _currPosX, _currPosY, (*fontInfo)[chr].x, (*fontInfo)[chr].y, (*fontInfo)[chr].width, (*fontInfo)[chr].height);

		_currPosX += (*fontInfo)[chr].width;
	}
}

void textManager::renderL(HDC hDC, int fontIdx, int colorIdx)
{
	if (_textWindowState1 != TEXT_WINDOW_STATE::INVISIBLE)
	{
		_prevRenderMode = _shouldRenderUsingWindowCoords;
		_shouldRenderUsingWindowCoords = TRUE;
		PatBlt(_hTextWindowDC, 0, 0, WINW, 320, BLACKNESS);

		// 글 출력 창 DC 비트맵 클리핑 영역 지정하기
		if (_textWindowState1 == TEXT_WINDOW_STATE::OPENING || _textWindowState1 == TEXT_WINDOW_STATE::CLOSING)
		{
			IMG->setRctClipRgn(_hTextWindowDC, 0, 16 * (10 - _windowClipCount), WINW, 32 * _windowClipCount);
		}

		// 글 출력 창 DC 비트맵에 창 출력하기
		IMG->render("대사 출력 창 스킨", _hTextWindowDC, 0, 0);

		// 글 출력 창 DC 비트맵에 대사 출력하기
		TXT->render(_hTextWindowDC, _currStr1, 31, 40, fontIdx, colorIdx);

		// 선택지가 있다면 글 출력 창 DC 비트맵에 선택 손가락 아이콘을 출력하기
		if (_shouldWindowHaveChoices)
		{
			switch (_choiceSelected)
			{
				case 1:
					_currPosX = 64;
					_currPosY -= ((*fontInfo)[32].height + 32);
					break;
				case 2:
					_currPosX = 64;
					_currPosY -= 16;
					break;
			}
			IMG->frameRender("위치 표시 타일셋", _hTextWindowDC, _currPosX, _currPosY, 0, 1);
		}

		// 글 출력 창 DC 비트맵에 있는 창과 대사를 hDC 비트맵에 출력하기
		if (_shouldWindowUseTheTopPane)
		{
			if (_textWindowState1 == TEXT_WINDOW_STATE::OPENING || _textWindowState1 == TEXT_WINDOW_STATE::CLOSING)
			{
				if (_textWindowAlpha == 0xFF) BitBlt(hDC, 0, 31 + 16 * (10 - _windowClipCount), WINW, 32 * _windowClipCount, _hTextWindowDC, 0, 16 * (10 - _windowClipCount), SRCCOPY);
				else
				{
					_bF.SourceConstantAlpha = _textWindowAlpha;
					GdiAlphaBlend(hDC, 0, 31 + 16 * (10 - _windowClipCount), WINW, 32 * _windowClipCount, _hTextWindowDC, 0, 16 * (10 - _windowClipCount), WINW, 32 * _windowClipCount, _bF);
				}
			}
			else
			{
				if (_textWindowAlpha == 0xFF) BitBlt(hDC, 0, 31, WINW, 320, _hTextWindowDC, 0, 0, SRCCOPY);
				else
				{
					_bF.SourceConstantAlpha = _textWindowAlpha;
					GdiAlphaBlend(hDC, 0, 31, WINW, 320, _hTextWindowDC, 0, 0, WINW, 320, _bF);
				}
			}
		}
		else
		{
			if (_textWindowState1 == TEXT_WINDOW_STATE::OPENING || _textWindowState1 == TEXT_WINDOW_STATE::CLOSING)
			{
				if (_textWindowAlpha == 0xFF) BitBlt(hDC, 0, WINH - 51 - 320 + 16 * (10 - _windowClipCount), WINW, 32 * _windowClipCount, _hTextWindowDC, 0, 16 * (10 - _windowClipCount), SRCCOPY);
				else
				{
					_bF.SourceConstantAlpha = _textWindowAlpha;
					GdiAlphaBlend(hDC, 0, WINH - 51 - 320 + 16 * (10 - _windowClipCount), WINW, 32 * _windowClipCount, _hTextWindowDC, 0, 16 * (10 - _windowClipCount), WINW, 32 * _windowClipCount, _bF);
				}
			}
			else
			{
				if (_textWindowAlpha == 0xFF) BitBlt(hDC, 0, WINH - 51 - 320, WINW, 320, _hTextWindowDC, 0, 0, SRCCOPY);
				else
				{
					_bF.SourceConstantAlpha = _textWindowAlpha;
					GdiAlphaBlend(hDC, 0, WINH - 51 - 320, WINW, 320, _hTextWindowDC, 0, 0, WINW, 320, _bF);
				}
			}
		}
		IMG->resetClipRgn(_hTextWindowDC);
		_shouldRenderUsingWindowCoords = _prevRenderMode;
	}
}

void textManager::renderBM(HDC hDC, int fontIdx, int colorIdx)
{
	if (_textWindowState2 != TEXT_WINDOW_STATE::INVISIBLE)
	{
		_prevRenderMode = _shouldRenderUsingWindowCoords;
		_shouldRenderUsingWindowCoords = TRUE;
		PatBlt(_hTextWindowDC, 0, 0, WINW, 96, BLACKNESS);

		// 글 출력 창 DC 비트맵에 창 출력하기
		IMG->render("전투 메시지 창 스킨", _hTextWindowDC, 0, 0);

		// 글 출력 창 DC 비트맵에 대사 출력하기
		TXT->render(_hTextWindowDC, _currStr2, 31, 24, fontIdx, colorIdx);

		// 글 출력 창 DC 비트맵에 있는 창과 대사를 hDC 비트맵에 출력하기
		if (!_shouldWindowUseTheTopPane)
			BitBlt(hDC, 0, 31, WINW, 96, _hTextWindowDC, 0, 0, SRCCOPY);
		else
			BitBlt(hDC, 0, WINH - 63 - 96, WINW, 96, _hTextWindowDC, 0, 0, SRCCOPY);
		_shouldRenderUsingWindowCoords = _prevRenderMode;
	}
	else if (_textWindowState1 != TEXT_WINDOW_STATE::INVISIBLE) // 이 else는 메시지 표시 우선순위를 적용하려면 있어야 한다.
	{
		_prevRenderMode = _shouldRenderUsingWindowCoords;
		_shouldRenderUsingWindowCoords = TRUE;
		PatBlt(_hTextWindowDC, 0, 0, WINW, 96, BLACKNESS);
		GdiTransparentBlt(_hTextWindowDC, 0, 0, WINW, 96,
						  _hTextWindowDC, 0, 0, WINW, 96, RGB(0, 0, 0));

		// 글 출력 창 DC 비트맵에 창 출력하기
		IMG->render("전투 메시지 창 스킨", _hTextWindowDC, 0, 0);
				
		// 글 출력 창 DC 비트맵에 대사 출력하기
		TXT->render(_hTextWindowDC, _currStr1, 31, 24, fontIdx, colorIdx);

		// 글 출력 창 DC 비트맵에 있는 창과 대사를 hDC 비트맵에 출력하기
		if (!_shouldWindowUseTheTopPane)
		{
			if (_textWindowAlpha == 0xFF) BitBlt(hDC, 0, 31, WINW, 96, _hTextWindowDC, 0, 0, SRCCOPY);
			else
			{
				_bF.SourceConstantAlpha = _textWindowAlpha;
				GdiAlphaBlend(hDC, 0, 31, WINW, 96, _hTextWindowDC, 0, 0, WINW, 96, _bF);
			}
		}
		else
		{
			if (_textWindowAlpha == 0xFF) BitBlt(hDC, 0, WINH - 63 - 96, WINW, 96, _hTextWindowDC, 0, 0, SRCCOPY);
			else
			{
				_bF.SourceConstantAlpha = _textWindowAlpha;
				GdiAlphaBlend(hDC, 0, WINH - 63 - 96, WINW, 96, _hTextWindowDC, 0, 0, WINW, 96, _bF);
			}
		}
		_shouldRenderUsingWindowCoords = _prevRenderMode;
	}
}

void textManager::renderC(HDC hDC, int destX, int destY, int fontIdx, int colorIdx)
{
	TXT->render(hDC, _currStr1, destX, destY, fontIdx, colorIdx);
}
