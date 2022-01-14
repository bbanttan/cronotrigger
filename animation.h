#pragma once

// 콜백 함수
typedef void(*CALLBACK_FUNCTION)(void);

// 매개 변수가 있는 콜백 함수
typedef void(*CALLBACK_FUNCTION_PARAMETER)(void*);

class animation
{
	public:
	typedef vector<POINT> frameList;	// 프레임 목록
	typedef vector<int> playList;		// 플레이 목록

	private:
	int			_totFrames;		// 총 프레임 수
	frameList	_frameList;
	playList	_playlist;

	int _frameWidth, _frameHeight;

	BOOL _loop;

	float _frameUpdateSec, _elapsedSec;

	DWORD _currPlayIndex;
	BOOL _play;

	void* _obj;
	CALLBACK_FUNCTION			_callbackFunction;
	CALLBACK_FUNCTION_PARAMETER	_callbackFunctionParameter;

	public:
	animation();
	~animation();

	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release();

	// 기본 재생 목록 준비 함수
	void setDefPlaylist(BOOL reverse = FALSE, BOOL loop = FALSE);
	void setDefPlaylist(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setDefPlaylist(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	// 배열 재생 목록 준비 함수
	void setPlaylist(int* playArr, int arrLen, BOOL loop = FALSE);
	void setPlaylist(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlaylist(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	// 구간 재생 목록 준비 함수
	void setPlaylist(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);
	void setPlaylist(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlaylist(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	void setFPS(int framesPerSec);

	void frameUpdate(float elapsedTime);

	void start();
	void stop();
	void pause();
	void resume();

	inline BOOL isPlay() { return _play; }
	inline POINT getFramePos() { return _frameList[_playlist[_currPlayIndex]]; }
	inline int getCurrPlaylistIdx() const { return _currPlayIndex; }
	inline void setCurrPlaylistIdx(const int& newIdx) { if (newIdx >= 0 && static_cast<size_t>(newIdx) < _playlist.size()) _currPlayIndex = newIdx; }
	inline int getFrameWidth() { return _frameWidth; }
	inline int getFrameHeight() { return _frameHeight; }
};

