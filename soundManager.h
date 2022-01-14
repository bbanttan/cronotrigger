#pragma once
#include "singleton.h"
#include <map>
#include "inc/fmod.hpp"
#pragma comment(lib, "lib/fmodex64_vc.lib")

using namespace FMOD;

constexpr size_t SOUNDBUFFER = 60; // 리마인더: 추가할 음악 파일 개수보다 커야 한다.
constexpr size_t EXTRACHANNELBUFFER = 6;
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager : public singleton<soundManager>
{
private:
	typedef map<string, Sound**>			soundList;
	typedef map<string, Sound**>::iterator	soundListIter;

	typedef map<string, Channel**>				channelList;
	typedef map<string, Channel**>::iterator	channelListIter;

	typedef map<string, bool>				bgmOrNotList;
	//typedef map<string, bool>::iterator		bgmOrNotListIter;

private:
	System*			_system;
	Sound**			_sound;
	Channel**		_channel;

	soundList		_soundList;
	soundListIter   _soundListIter;
	bgmOrNotList	_bgmOrNotList;

public:
	soundManager();
	~soundManager();

	HRESULT init();
	void release();
	void update();

	Sound** findSound(string strKey);

	Channel* findChannel(string strKey);

	// 음악 파일 추가 함수(키 값, 파일명, 배경음인지 여부, 반복 재생 여부)
	void addSound(string strKey, string soundFileName, bool isBGM, bool isLoop);

	// 추가된 음악 파일을 재생하는 함수 (음량: 0.0f부터 1.0f까지)
	void play(string strKey, float volume = 1.0f);
	
	// 특정 음악 재생을 완전히 멈추는 함수
	void stop(string strKey);
	
	// 특정 음악 재생을 일시 정지 하는 함수
	void pause(string strKey);
	
	// 특정 음악 재생을 재개하는 함수
	void resume(string strKey);
	
	// 특정 음악이 재생 중인지 검사하는 함수
	bool isPlaying(string strKey);
	
	// 특정 음악 재생이 일시 정지 된 상태인지 검사하는 함수
	bool isPaused(string strKey);

	// 현재 재생 중인 음악을 전부 일시 정지 하는 함수
	void pauseAll();

	// 현재 일시 정지 중인 음악을 전부 재생하는 함수
	void resumeAll();

	// 현재 재생 중인 음악을 전부 정지하는 함수
	void stopAll();

	// 마스터 음량 변경을 즉시 반영할 때 쓰는 함수
	void updateMasterVolume();

	// 배경음 음량 변경을 즉시 반영할 때 쓰는 함수
	void updateBGMVolume();

	// 효과음 음량 변경을 즉시 반영할 때 쓰는 함수
	void updateSFXVolume();
};

