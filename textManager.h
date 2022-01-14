#pragma once
#include "singleton.h"
//#include <fstream>
#include <queue>
#include "fontParser.h"
#include <sstream>

// 글 출력 창 상태
enum class TEXT_WINDOW_STATE
{
	INVISIBLE, // 화면에 보이지 않는 상태(갱신하지 않는다.)
	OPENING, // 열리고 있는 상태
	VISIBLE, // 화면에 보이는 상태
	CLOSING // 닫히고 있는 상태
};

class textManager : public singleton<textManager>
{
	private:
	fontParser* _fP;
	vector<CD>* fontInfo;
	image* _currFontImage;

	BOOL _shouldWindowUseTheTopPane;
	BOOL _shouldWindowHaveChoices; // 선택지용 변수
	BOOL _isGoingToBeAutoClosed, _shouldBeAutoClosed;
	BOOL _isWholeCurrStrShown1, _isWholeCurrStrShown2;
	string _currStr1, _currStr2;
	istringstream iSS; // 한 번에 한 글자씩 처리할 때 쓰는 변수
	char _character; // 글자 변수
	int _choiceSelected; // 선택지용 변수(1: 처음 답; 2: 두 번째 답)
	int _currPosX, _currPosY, _waitCount; // 글 출력용 변수
	int _windowClipCount; // 창을 열고 닫을 때 쓰는 변수

	HDC _hTextWindowDC;
	HBITMAP _hTextWindowBitmap, _hOTextWindowBitmap;
	BLENDFUNCTION _bF;

	TEXT_WINDOW_STATE _textWindowState1, _textWindowState2;

	queue<pair<int, string>> _dialogLineQ; // int 변수는 선택지를 반영하는 변수이다.
	queue<pair<string, int>> _battleMsgQ1, _battleMsgQ2; // 1, 2는 메시지 종류(1: 한 번에 한 글자 더; 2: 한 번에 한 줄 전부)를 나타낸다. int 변수는 (남은) 메시지 표시 시간을 나타내는 변수이다.
	queue<string> _chrByChrQ;

	void setCurrFontImage(int searchValue);

	public:
	textManager();
	~textManager();

	HRESULT init();
	void release();

	void prepareToUseFonts();

	// 출력할 대사 스트링을 대기열에 추가하는 함수
	void enqueueL(string line, int forWhichChoice = 0);

	// 출력할 전투 메시지를 대기열에 추가하는 함수(isShownChrByChr로 한 번에 한 줄을 전부 출력할지 여부를 지정한다.)
	// 한 번에 한 줄이 출력되는 메시지가 한 번에 한 글자 더 출력되는 메시지보다 위에 출력된다(각 메시지는 다른 창을 사용한다.).
	// 출력 대기 중인 메시지가 없으면 일정 시간(_battleMsgSpeed에 의존한다.)이 경과하였을 때 자동으로 창이 닫힌다.
	void enqueueBM(string msg, bool isShownChrByChr = false);

	// 창 없이 글자 단위로 출력할 스트링을 대기열에 추가하는 함수(메뉴 화면용)
	// 대기열에 스트링이 둘 이상 있으면 제일 나중에 추가되는 스트링 하나만 남게 된다.
	void enqueueC(string msg);

	// 대사 스트링 대기열을 비우는 함수
	void clearLQ();

	// 전투 메시지 스트링 대기열을 비우는 함수
	void clearBMQ();

	// 글자 단위 출력 스트링 대기열을 비우는 함수(메뉴 화면용)
	void clearCQ();

	// 현 상태에 맞게 특정 창에 출력할 대사 스트링을 준비하는 함수(_currStr 사용)
	void updateL();

	// 현 상태에 맞게 특정 창에 출력할 전투 메시지 스트링을 준비하는 함수(_currStr 사용)
	void updateBM();

	// 현 상태에 맞게 창 없이 글자 단위로 출력할 스트링을 준비하는 함수(_currStr 사용)(메뉴 화면용)
	void updateC();

	// 창 없이 특정 스트링을 즉시 출력하는 함수
	// fontIdx: 0 가변 폭, 1 고정 폭(기본)
	// colorIdx: 0 하양(기본), 1 하늘, 2 빨강, 3 노랑, 4 초록, 5 비활성 색
	// 참고(기본 글꼴): \n 개행, \t 세 칸 띄기, { 음표, | 하트, } 줄임표, ` 쌍따옴표 열기, ~ 무한대
	void render(HDC hDC, string text, int destX = 0, int destY = 0, int fontIdx = 1, int colorIdx = 0);

	// 글 출력 창 스킨과 스트링을 출력하는 함수(창 표시 후 일정 간격마다 글자 하나 더 출력한다.)
	// fontIdx: 0 가변 폭(기본), 1 고정 폭
	// colorIdx: 0 하양(기본), 1 하늘, 2 빨강, 3 노랑, 4 초록, 5 비활성 색
	void renderL(HDC hDC, int fontIdx = 0, int colorIdx = 0);

	// 글 출력 창 스킨과 전투 메시지 스트링을 출력하는 함수(각 메시지 창이 닫히는 속도는 _battleMsgSpeed에 의존한다.)
	// fontIdx: 0 가변 폭(기본), 1 고정 폭
	// colorIdx: 0 하양(기본), 1 하늘, 2 빨강, 3 노랑, 4 초록, 5 비활성 색
	void renderBM(HDC hDC, int fontIdx = 0, int colorIdx = 0);

	// 창 없이 특정 스트링을 글자 단위로 출력하는 함수(필요: enqueue 함수 호출 후 update 함수 호출)
	void renderC(HDC hDC, int destX = 0, int destY = 0, int fontIdx = 0, int colorIdx = 0);

	// 현재 선택지 변수 값을 반환하는 함수
	inline int getChoiceSelected() { return _choiceSelected; }

	// 선택지 변수를 초기화하는 함수
	inline void resetChoiceSelected() { _choiceSelected = 0; }

	// 글 출력 창 상태를 설정하는 함수(0 INVISIBLE, 1 OPENING, 2 VISIBLE, 3 CLOSING)
	inline void setTextWindowState1(int value) { _textWindowState1 = static_cast<TEXT_WINDOW_STATE>(value); }
	inline void setTextWindowState2(int value) { _textWindowState2 = static_cast<TEXT_WINDOW_STATE>(value); }

	// 글 출력 창 상태를 반환하는 함수
	inline TEXT_WINDOW_STATE getTextWindowState1() { return _textWindowState1; }
	inline TEXT_WINDOW_STATE getTextWindowState2() { return _textWindowState2; }

	inline BOOL getIfWholeCurrStrIsShown1() { return _isWholeCurrStrShown1; }

	inline void toggleTextWindowPos() { _shouldWindowUseTheTopPane = !_shouldWindowUseTheTopPane; }
	inline BOOL getTextWindowPos() { return _shouldWindowUseTheTopPane; }
};

